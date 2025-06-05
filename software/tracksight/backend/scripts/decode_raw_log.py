import argparse
import csv
import logging
import os
import struct
import sys
import crc8

import pandas as pd
from csv_to_mf4 import csv_to_mf4
from logfs import LogFs, LogFsDiskFactory
from tzlocal import get_localzone

logging.basicConfig(level=logging.INFO)

logger = logging.getLogger(__name__)

# Path fuckery so we can import JSONCAN.
script_dir = os.path.dirname(os.path.abspath(__file__))
root_dir = os.path.join(script_dir, "..", "..", "..", "..")
if root_dir not in sys.path:
    sys.path.append(root_dir)

from scripts.code_generation.jsoncan.src.json_parsing.json_can_parsing import (
    JsonCanParser,
)


# 1 byte for magic, 1 byte for CRC, 2 bytes for timestamp, 4 bytes for ID, 1 byte for DLC
CAN_MSG_HEADER_SIZE = 1 + 1 + 2 + 4 + 1

# Magic indicating the start of a logged CAN message
CAN_MSG_LOG_MAGIC = 0xBA

# Number of bytes to unpack before logging an update.
UNPACK_CHUNK_SIZE_BYTES = 1_000_000

# Columns of output CSV file.
CSV_HEADER = ["time", "signal", "value", "label", "unit"]


def extract_bits(data: int, start_bit: int, size: int) -> int:
    """
    Extract the raw bits of length `size`, starting at `start_bit`.
    """
    return (data >> start_bit) & ((1 << size) - 1)


def decode_can_packet(data: bytes):
    """
    Decode a raw CAN packet. The format is defined in `firmware/shared/src/io/io_canLogging.c`.
    """
    magic, expected_crc, timestamp_ms, msg_id, dlc = struct.unpack("<BBHLB", data)
    payload_size = CAN_MSG_HEADER_SIZE + dlc

    if magic != CAN_MSG_LOG_MAGIC:
        raise RuntimeError("Magic is incorrect (not 0xBA)")

    data_bytes = data[CAN_MSG_HEADER_SIZE:payload_size]
    if len(data_bytes) != dlc:
        raise RuntimeError("Insufficient bytes for expected data length")

    hasher = crc8.crc8()
    hasher.update(data[:payload_size])
    calculated_crc = hasher.digest()
    if calculated_crc != expected_crc:
        raise RuntimeError("CRC mismatch while trying to decode a CAN message")

    return timestamp_ms, msg_id, data_bytes, payload_size


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--disk",
        "-d",
        type=str,
        help="Path to disk",
        required=True,
    )
    parser.add_argument(
        "--file",
        "-f",
        type=str,
        help="Files to decode (pass multiple with a comma-seperated string)",
        default=None,
    )
    parser.add_argument(
        "--time",
        "-t",
        type=str,
        help="Time that this log was collected from, ex: `2024-06-1T12:30` is June 1, 2024 at 12:30PM.",
        required=True,
    )
    parser.add_argument(
        "--block_size", "-b", type=int, help="Block size in bytes", default=512
    )
    parser.add_argument(
        "--block_count",
        "-N",
        type=int,
        help="Number of blocks",
        default=1024 * 1024 * 15,
    )
    parser.add_argument(
        "--output",
        "-o",
        type=str,
        help="Path to output directory.",
        default=os.path.join(script_dir, "..", "data"),
    )
    parser.add_argument(
        "--can-json",
        type=str,
        help="Path to JSONCAN source files",
        default=os.path.join(root_dir, "can_bus", "quadruna"),
    )
    parser.add_argument(
        "--name",
        "-n",
        type=str,
        help="Descriptive name of this session.",
        required=True,
    )
    parser.add_argument(
        "--file_range",
        "-r",
        type=str,
        help="Range of file numbers to decode, e.g., '1-200'",
        default=None,
    )
    parser.add_argument(
        "--mf4", action="store_true", help="call csv_to_mf4 script", default=True
    )

    args = parser.parse_args()

    files_to_decode = []
    if args.file:
        files_to_decode = args.file.split(",")
    elif args.file_range:
        start, end = map(int, args.file_range.split("-"))
        files_to_decode = [f"/{i}.txt" for i in range(start, end + 1)]
    else:
        files_to_decode = None

    start_timestamp = pd.Timestamp(args.time, tz=get_localzone())

    # Fix: windows does not allow ':' in file names
    start_timestamp_no_spaces = start_timestamp.strftime("%Y-%m-%d_%H_%M")

    # Open filesystem.
    logfs = LogFs(
        block_size=args.block_size,
        block_count=args.block_count,
        disk=LogFsDiskFactory.create_disk(
            block_size=args.block_size,
            block_count=args.block_count,
            disk_path=args.disk,
        ),
        rd_only=True,
    )

    # Parse JSONCAN source files to create CAN database.
    can_db = JsonCanParser(can_data_dir=args.can_json).make_database()

    # Read and decode all files.
    files = logfs.list_dir()
    for file_path in files:
        if file_path == "/bootcount.txt":
            # This isn't a log file, ignore.
            continue

        if (
            files_to_decode is not None
            and file_path not in files_to_decode
            and file_path.lstrip("/") not in files_to_decode
        ):
            # Doesn't match the files we want to decode, ignore.
            logger.info(f"Skipping file '{file_path}', doesn't match file flag.")
            continue

        logger.info(f"Opening log file '{file_path}'.")
        with logfs.open(path=file_path) as file:
            raw_data = file.read()

            if raw_data == b"":
                logger.info(f"Skipping file '{file_path}', no data found.")
                continue

            file_path, _ = file_path.lstrip("/").split(".")
            out_name = f"{start_timestamp_no_spaces}_{args.name}_{file_path}"
            out_path = os.path.join(args.output, out_name + ".csv")

            if not os.path.exists(os.path.dirname(out_path)):
                # Create output path if it doesn't already exist.
                os.makedirs(os.path.dirname(out_path))

            last_timestamp_ms = pd.Timedelta(milliseconds=0)
            overflow_fix_delta_ms = pd.Timedelta(milliseconds=0)

            with open(file=out_path, mode="w+", newline="") as out_file:
                logger.info(f"Decoding file '{file_path}' to '{out_path}'.")
                csv_writer = csv.writer(out_file)
                csv_writer.writerow(CSV_HEADER)

                i = 0
                while i < len(raw_data):
                    if i % UNPACK_CHUNK_SIZE_BYTES == 0:
                        percent_unpacked = i / len(raw_data) * 100
                        logger.info(
                            f"Unpacked {int(percent_unpacked)}% of bytes in file."
                        )

                    # Scan for magic indicating the start of the next log
                    if raw_data[i] != CAN_MSG_LOG_MAGIC:
                        i += 1
                        continue

                    # Parse raw CAN packet.
                    packet_data = raw_data[i:]

                    try:
                        timestamp_ms, msg_id, data_bytes, size_bytes = (
                            decode_can_packet(data=packet_data)
                        )
                        i += size_bytes
                    except Exception as err:
                        logger.warning(
                            f"Exception raised while trying to decode a CAN message: {err}"
                        )
                        continue

                    delta_timestamp = (
                        pd.Timedelta(milliseconds=timestamp_ms) + overflow_fix_delta_ms
                    )

                    if delta_timestamp < last_timestamp_ms - pd.Timedelta(seconds=30):
                        # We currently allocate 16 bits for timestamps, so we need to add 2^16 to undo the overflow.
                        delta = pd.Timedelta(milliseconds=2**16)
                        overflow_fix_delta_ms += delta
                        delta_timestamp += delta

                    last_timestamp_ms = delta_timestamp
                    timestamp = start_timestamp + delta_timestamp

                    # Decode CAN packet with JSONCAN.
                    parsed_signals = can_db.unpack(id=msg_id, data=data_bytes)

                    # Write signals to parsed CSV file.
                    for signal in parsed_signals:
                        signal_name = signal["name"]
                        signal_value = signal["value"]
                        signal_unit = signal.get("unit", "")
                        signal_label = signal.get("label", "")
                        csv_writer.writerow(
                            [
                                timestamp,
                                signal_name,
                                signal_value,
                                signal_label,
                                signal_unit,
                            ]
                        )

    if args.mf4:
        csv_dir = args.output
        logger.info("Converting CSV files to MDF format.")
        csv_to_mf4(input=args.output)
