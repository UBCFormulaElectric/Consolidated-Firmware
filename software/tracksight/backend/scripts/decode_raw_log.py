import os
import sys
import argparse
import struct
import csv
import pandas as pd
from tzlocal import get_localzone
import logging
from logfs import LogFs, LogFsUnixDisk
import subprocess

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

# Size of an individual packet.
CAN_PACKET_SIZE_BYTES = 16

# Number of CAN msgs to unpack before logging an update.
CAN_MSGS_CHUNK_SIZE = 100_000

# Columns of output CSV file.
CSV_HEADER = ["time", "signal", "value", "label", "unit"]


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
        "--block_count", "-N", type=int, help="Number of blocks", default=1024 * 1024
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
        default=None
    )
    parser.add_argument(
        "--mf4",
        action="store_true",
        help="call csv_to_mf4 script",
        default=None
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
    start_timestamp_no_spaces = start_timestamp.strftime("%Y-%m-%d_%H:%M")

    # Open filesystem.
    logfs = LogFs(
        block_size=args.block_size,
        block_count=args.block_count,
        disk=LogFsUnixDisk(
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

            with open(file=out_path, mode="w", newline="") as out_file:
                logger.info(f"Decoding file '{file_path}' to '{out_path}'.")
                csv_writer = csv.writer(out_file)
                csv_writer.writerow(CSV_HEADER)

                total_msgs = len(raw_data) // CAN_PACKET_SIZE_BYTES

                for i in range(0, len(raw_data), CAN_PACKET_SIZE_BYTES):
                    msgs_unpacked = i // CAN_PACKET_SIZE_BYTES
                    if msgs_unpacked % CAN_MSGS_CHUNK_SIZE == 0 and msgs_unpacked > 0:
                        percent_unpacked = msgs_unpacked / total_msgs * 100
                        logger.info(
                            f"Unpacked {int(percent_unpacked)}% of messages in file."
                        )

                    # Parse raw CAN packet.
                    packet_data = raw_data[i : i + CAN_PACKET_SIZE_BYTES]
                    if len(packet_data) != CAN_PACKET_SIZE_BYTES:
                        break

                    timestamp_ms, msg_id, data_bytes = decode_can_packet(
                        data=packet_data
                    )
                    delta_timestamp = (
                        pd.Timedelta(milliseconds=timestamp_ms) + overflow_fix_delta_ms
                    )

                    if delta_timestamp < last_timestamp_ms - pd.Timedelta(minutes=1):
                        # We currently allocate 17 bits for timestamps, so we need to add 2^17 to undo the overflow.
                        delta = pd.Timedelta(milliseconds=2**17)
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
        subprocess_args = ["python3", "csv_to_mf4.py", "--input", csv_dir]
        subprocess.run(subprocess_args)