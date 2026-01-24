import argparse
import csv
import logging
import os
import sys
import pandas as pd

from tzlocal import get_localzone
from csv_to_mf4 import csv_to_mf4
from logfs import LogFs, LogFsDiskFactory
from logfs import can_logger

# Path fuckery so we can import JSONCAN.
script_dir = os.path.dirname(os.path.abspath(__file__))
root_dir = os.path.join(script_dir, "..", "..", "..", "..")
if root_dir not in sys.path:
    sys.path.append(root_dir)

from scripts.code_generation.jsoncan.src.json_parsing.json_can_parsing import (
    JsonCanParser,
)

logging.basicConfig(level=logging.INFO)

logger = logging.getLogger(__name__)

# Columns of output CSV file.
CSV_HEADER = ["time", "signal", "value", "label", "unit"]

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--disk",
        "-d",
        type=str,
        help="Path to disk",
        # required=True,
        default="E:",
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
        # required=True,
        default="2024-09-28T12:00",
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
        default=os.path.join(root_dir, "can_bus", "quintuna"),
    )
    parser.add_argument(
        "--name",
        "-n",
        type=str,
        help="Descriptive name of this session.",
        default="test-drive",
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
    decoder = can_logger.Decoder(db=can_db)

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
            file_path_no_ext, _ = file_path.lstrip("/").split(".")
            out_name = f"{args.name}_{file_path_no_ext}"
            out_path = os.path.join(args.output, out_name + ".csv")

            if not os.path.exists(os.path.dirname(out_path)):
                # Create output path if it doesn't already exist.
                os.makedirs(os.path.dirname(out_path))

            logger.info(f"Decoding file '{file_path}' to '{out_path}'.")
            signals = decoder.decode(metadata=file.read_metadata(), data=file.read())

            with open(file=out_path, mode="w+", newline="") as out_file:
                csv_writer = csv.writer(out_file)
                csv_writer.writerow(CSV_HEADER)
                for signal in signals:
                    row_signal = [
                        signal.timestamp,
                        signal.name,
                        signal.value,
                        signal.label,
                        signal.unit,
                    ]
                    csv_writer.writerow(signal)

    if args.mf4:
        csv_dir = args.output
        logger.info("Converting CSV files to MDF format.")
        csv_to_mf4(input=args.output)
