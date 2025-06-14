import os
import sys
import pandas as pd
import struct
import crc8
import logging
from typing import Optional
from dataclasses import dataclass

logger = logging.getLogger(__name__)


from scripts.code_generation.jsoncan.src.json_parsing.json_can_parsing import (
    JsonCanParser,
)

# Path fuckery so we can import JSONCAN.
script_dir = os.path.dirname(os.path.abspath(__file__))
root_dir = os.path.join(script_dir, "..", "..", "..", "..")
if root_dir not in sys.path:
    sys.path.append(root_dir)

# 1 byte for magic, 1 byte for DLC, 2 bytes for timestamp, 4 bytes for ID
CAN_MSG_HEADER_SIZE = 1 + 1 + 2 + 4

# Magic indicating the start of a logged CAN message
CAN_MSG_LOG_MAGIC = 0xBA

# Number of bytes to unpack before logging an update.
UNPACK_CHUNK_SIZE_BYTES = 1_000_000


@dataclass
class DecodedSignal:
    timestamp: pd.Timestamp
    name: str
    value: int | float
    label: Optional[str]
    unit: Optional[str]

    def __str__(self) -> str:
        if self.label is not None:
            return f"{self.timestamp}: {self.name} = {self.label}"
        else:
            return f"{self.timestamp}: {self.name} = {self.value}{'' if self.unit is None else self.unit }"


def decode_can_packet(data: bytes):
    """
    Decode a raw CAN packet. The format is defined in `firmware/shared/src/io/io_canLogging.c`.
    """
    magic, dlc, timestamp_ms, msg_id = struct.unpack(
        "<BBHL", data[:CAN_MSG_HEADER_SIZE]
    )

    if magic != CAN_MSG_LOG_MAGIC:
        raise RuntimeError("Magic is incorrect (not 0xBA)")

    payload_size = CAN_MSG_HEADER_SIZE + dlc
    data_bytes = data[CAN_MSG_HEADER_SIZE:payload_size]
    expected_crc = data[payload_size]  # Last byte is CRC8

    if len(data_bytes) != dlc:
        raise RuntimeError("Insufficient bytes for expected data length")

    hasher = crc8.crc8()
    hasher.update(data[:payload_size])
    hexer = hasher.digest()
    calculated_crc = int.from_bytes(hexer, byteorder="little")

    if calculated_crc != expected_crc:
        raise RuntimeError(
            f"CRC mismatch while trying to decode a CAN message: {hexer} == {expected_crc}"
        )

    return timestamp_ms, msg_id, data_bytes, payload_size


def decode(raw_data: bytes, start_timestamp: pd.Timestamp, jsoncan_dir: str) -> list:
    """
    Decode raw CAN log data into a list of signals.
    Each signal is represented as a tuple:
    (timestamp, signal_name, signal_value, signal_label, signal_unit)
    """
    signals = []
    last_timestamp_ms = pd.Timedelta(milliseconds=0)
    overflow_fix_delta_ms = pd.Timedelta(milliseconds=0)

    # Parse JSONCAN source files to create CAN database.
    can_db = JsonCanParser(can_data_dir=jsoncan_dir).make_database()

    i = 0
    while i < len(raw_data):
        if i % UNPACK_CHUNK_SIZE_BYTES == 0:
            percent_unpacked = i / len(raw_data) * 100
            logger.info(f"Unpacked {int(percent_unpacked)}% of bytes in file.")

        # Scan for magic indicating the start of the next log
        if raw_data[i] != CAN_MSG_LOG_MAGIC:
            i += 1
            continue

        # Parse raw CAN packet.
        packet_data = raw_data[i:]

        try:
            timestamp_ms, msg_id, data_bytes, size_bytes = decode_can_packet(
                data=packet_data
            )
            i += size_bytes
        except Exception as err:
            logger.warning(
                f"Exception raised while trying to decode a CAN message: {err}"
            )
            i += 1
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
        parsed_signals = can_db.unpack(msg_id=msg_id, data=data_bytes)

        for signal in parsed_signals:
            signals.append(
                DecodedSignal(
                    timestamp=timestamp,
                    name=signal.name,
                    value=signal.value,
                    label=signal.label,
                    unit=signal.unit,
                )
            )

    return signals
