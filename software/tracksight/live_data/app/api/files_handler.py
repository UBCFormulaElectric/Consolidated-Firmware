"""
Assortment of API endpoints used to manage SD cards attached to the computer,
"""

# import sqlite3

import psutil
from flask import Blueprint, send_file
from logfs import LogFs, LogFsDiskFactory
from logger import logger
from io import BytesIO
import pandas as pd
import struct
from tasks.influx_logger import influx_queue
from CanMsg import CanSignal
from middleware.candb import live_can_db as can_db


sd_api = Blueprint('sd', __name__)


## Helpers to decode logfs ##
# Size of an individual packet.
CAN_PACKET_SIZE_BYTES = 16

# Number of CAN msgs to unpack before logging an update.
CAN_MSGS_CHUNK_SIZE = 100_000


def extract_bits(data: int, start_bit: int, size: int) -> int:
    """
    Extract the raw bits of length `size`, starting at `start_bit`.
    """
    return (data >> start_bit) & ((1 << size) - 1)


def decode_can_packet(data: bytes):
    """
    Decode a raw CAN packet. The format is defined in `firmware/shared/src/io/io_canLogging.h`.
    """
    # Packet header (message ID, data length code, and timestamp) is first 4 bytes.
    # Raw CAN data bytes is the next 8 bytes.
    packet_header, data_bytes, _unused = struct.unpack("<LQL", data)

    # Parse the packet header.
    msg_id = extract_bits(data=packet_header, start_bit=0, size=11)
    dlc = extract_bits(data=packet_header, start_bit=11, size=4)
    timestamp_ms = extract_bits(data=packet_header, start_bit=15, size=17)

    return timestamp_ms, msg_id, data_bytes.to_bytes(length=8, byteorder="little")[:dlc]


def decode_raw_log_to_signals(
    raw_data: bytes,
    start_timestamp: pd.Timestamp,
) -> list:
    """
    Decode raw CAN log data into a list of signals.
    Each signal is represented as a tuple:
    (timestamp, signal_name, signal_value, signal_label, signal_unit)
    """
    signals = []
    last_timestamp_ms = pd.Timedelta(milliseconds=0)
    overflow_fix_delta_ms = pd.Timedelta(milliseconds=0)

    for i in range(0, len(raw_data), CAN_PACKET_SIZE_BYTES):
        packet_data = raw_data[i: i + CAN_PACKET_SIZE_BYTES]
        if len(packet_data) != CAN_PACKET_SIZE_BYTES:
            break

        timestamp_ms, msg_id, data_bytes = decode_can_packet(data=packet_data)
        delta_timestamp = pd.Timedelta(
            milliseconds=timestamp_ms) + overflow_fix_delta_ms

        if delta_timestamp < last_timestamp_ms - pd.Timedelta(minutes=1):
            # Undo timestamp overflow.
            delta = pd.Timedelta(milliseconds=2**17)
            overflow_fix_delta_ms += delta
            delta_timestamp += delta

        last_timestamp_ms = delta_timestamp
        timestamp = start_timestamp + delta_timestamp

        # Decode CAN packet with JSONCAN.
        parsed_signals = can_db.unpack(msg_id=msg_id, data=data_bytes)

        for signal in parsed_signals:
            signal_name = signal.name
            signal_value = signal.value
            signals.append(
                (signal_name, signal_value, timestamp)
            )
    return signals


# Endpoints
class NoSDCard(Exception):
    pass


@sd_api.errorhandler(NoSDCard)
def no_sd_card_error_handler(err):
    return {"error": "No such SD Card"}, 400


class TooManySDCards(Exception):
    pass


@sd_api.errorhandler(TooManySDCards)
def too_many_sd_card_error_handler(err):
    return {"error": "Many SD Cards Matched This Description"}, 500


def find_sd_card(sd_device: str):
    """
    Finds an SD card based on the device name
    Raises NoSDCard if no such device exists
    Raises TooManySDCards if multiple of such device exists (this should not happen)
    """

    candidate_partitions = list(filter(
        lambda d: d.device == sd_device and "removable" in d.opts.split(","), psutil.disk_partitions()))
    if len(candidate_partitions) != 1:
        if len(candidate_partitions) == 0:
            raise NoSDCard()
        else:
            logger.critical(
                f"Found multiple for {sd_device}, namely {candidate_partitions}")
            raise TooManySDCards()
    return candidate_partitions[0]


def create_logfs(mountpoint: str):
    return LogFs(
        block_size=512,
        block_count=1024 * 1024 * 15,
        disk=LogFsDiskFactory.create_disk(
            block_size=512,
            block_count=1024 * 1024 * 15,
            disk_path=mountpoint,
        ),
        rd_only=True,
    )


@sd_api.route("/sd/list", methods=["GET"])
def list_sd_cards():
    removable_cards = [i for i in psutil.disk_partitions(
    ) if "removable" in i.opts.split(",")]
    return [card.device for card in removable_cards]

# IMPORTANT NOTE: sd_device must be adequately escaped when placed in a url string (use %2F)


# seems legit
@sd_api.route("/sd/<sd_device>/files/list", methods=["GET"])
def list_files_in_card(sd_device: str):
    """
    Lists all files within an SD card's filesystem
    """
    sd_card = find_sd_card(sd_device)
    logfs = create_logfs(sd_card.mountpoint)
    file_names: list[str] = [file for file in logfs.list_dir() if file not in [
        "/bootcount.txt"]]
    return [{
        "name": file_name,
        "start_iso_time": "2025-01-06"
    } for file_name in file_names]

# Todo
@sd_api.route("/sd/<sd_device>/files/<file_id>/dump", methods=["POST"])
def dump_file(sd_device: str, file_id: str):
    """
    Dumps a given file to the influx database (historical bucket)
    Streams progress
    """
    sd_card = find_sd_card(sd_device)
    logfs = create_logfs(sd_card.mountpoint)
    try:
        with logfs.open(file_id) as file:
            raw_data = file.read()
            metadata = file.read_metadata()
    except Exception:
        return {"error": "File Does Not Exist"}, 400

    # take each signal in this file and
    # 1. convert to mf4 and cache to disk
    # 2. add to influx database
    # make sure to in the process stream the progress

    # get signals from raw_data
    # TODO: get proper timestamp 
    signals = decode_raw_log_to_signals(
        raw_data=raw_data,
        start_timestamp=pd.Timestamp.now(),
    )

    for signal in signals:
        influx_queue.put(
            CanSignal(signal[0], signal[1], signal[2].to_pydatetime())
        )

    # finally, add the file to the historical sqlite db
    # with sqlite3.connect("historical.db") as historical_db:
    #     historical_db.execute("INSERT INTO files (file_name, commit_sha, start_iso_time, duration_iso) VALUES (:file_name, :commit_sha, :start_iso_time, :duration_iso)", {
    #         "file_name": "",
    #         "commit_sha": "",
    #         "start_iso_time": "",
    #         "duration_iso": ""
    #     })
    #     historical_db.commit()

    return "", 200


# Todo
@sd_api.route("/sd/<sd_device>/files/<file_id>/get", methods=["GET"])
def download_file(sd_device: str, file_id: str):
    """
    Sends the user the file requested
    """
    sd_card = find_sd_card(sd_device)
    logfs = create_logfs(sd_card.mountpoint)
    try:
        with logfs.open(file_id) as file:
            raw_data = file.read()
            return send_file(
                BytesIO(raw_data),
                mimetype="application/octet-stream",
                as_attachment=True,
                download_name="data.bin"
            )
    except Exception:
        return {"error": "File Does Not Exist"}, 400
