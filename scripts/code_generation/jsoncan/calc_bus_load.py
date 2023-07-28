import argparse
from src.json_parsing.json_can_parsing import JsonCanParser

FRAME_BITS = 1 + 2 + 7  # SOF + ACK + EOF
ID_BITS = 11
CONTROL_BITS = 1 + 6  # RTR + control
DATA_BYTE_BITS = 8
CRC_BITS = 16


def msg_payload_bits(msg):
    return sum(
        [
            FRAME_BITS,
            ID_BITS,
            CONTROL_BITS,
            DATA_BYTE_BITS * msg.bytes(),
            CRC_BITS,
        ]
    )


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--can_data_dir", help="Path to JSON CAN data")
    parser.add_argument("--can_bit_rate", help="CAN bit rate in bps, e.g. 500000 bps")
    args = parser.parse_args()

    # Parse JSON
    can_db = JsonCanParser(can_data_dir=args.can_data_dir).make_database()

    # Estimate bits per second
    bits_per_s = 0
    for msg in can_db.msgs:
        if msg.is_periodic():
            freq_hz = 1000 / msg.cycle_time
            bits_per_s += msg_payload_bits(msg) * freq_hz

    estimated_bus_load = bits_per_s / float(args.can_bit_rate) * 100
    print(f"Estimated nominal bus load: {round(estimated_bus_load)}%")
