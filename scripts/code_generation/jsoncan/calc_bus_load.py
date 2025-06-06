import argparse
import sys

from jsoncan.src.can_database import CanDatabase
from jsoncan.src.json_parsing.json_can_parsing import JsonCanParser

FRAME_BITS = 1 + 2 + 7  # SOF + ACK + EOF
ID_BITS = 11
CONTROL_BITS = 1 + 6  # RTR + control
DATA_BYTE_BITS = 8
CRC_BITS = 16

# https://www.chiefdelphi.com/t/is-70-can-bus-utilization-bad/392683/2
BAD_BUS_LOAD = 80


sys.stdout.reconfigure(encoding="utf-8")
# https://www.chiefdelphi.com/t/is-70-can-bus-utilization-bad/392683/2
BAD_BUS_LOAD = 80


sys.stdout.reconfigure(encoding="utf-8")


def msg_payload_bits(msg):
    return sum(
        [
            FRAME_BITS,
            ID_BITS,
            CONTROL_BITS,
            DATA_BYTE_BITS * msg.dlc(),
            CRC_BITS,
        ]
    )


def calculate_bus_load(canDatabase: CanDatabase, can_bit_rate):
    bits_per_s = 0
    for msg in canDatabase.msgs.items():
        msg = msg[1]
        if msg.is_periodic():
            freq_hz = 1000 / msg.cycle_time
            bits_per_s += msg_payload_bits(msg) * freq_hz

    return bits_per_s / float(can_bit_rate) * 100


def report_bus_load(canDatabase: CanDatabase, can_bit_rate):
    estimated_bus_load = calculate_bus_load(canDatabase, can_bit_rate)
    if estimated_bus_load > BAD_BUS_LOAD:
        print(f"⚠️: Estimated bus load is {round(estimated_bus_load)}%")
    else:
        print(f"👍 Estimated bus load: {round(estimated_bus_load)}%")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--can_data_dir", help="Path to JSON CAN data")
    parser.add_argument(
        "--can_bit_rate",
        help="CAN bit rate in bps, e.g. 500000 bps",
        default=500000,
        type=int,
    )
    args = parser.parse_args()
    # Parse JSON
    can_db = JsonCanParser(can_data_dir=args.can_data_dir).make_database()
    # Estimate bits per second
    report_bus_load(can_db, args.can_bit_rate)
