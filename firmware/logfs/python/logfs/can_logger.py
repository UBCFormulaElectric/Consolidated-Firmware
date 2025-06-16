import os
import sys
import pandas as pd
import struct
import crc8
import logging

# Path fuckery so we can import JSONCAN.
script_dir = os.path.dirname(os.path.abspath(__file__))
root_dir = os.path.join(script_dir, "..", "..", "..", "..")
if root_dir not in sys.path:
    sys.path.append(root_dir)

from scripts.code_generation.jsoncan.src.can_database import (
    CanDatabase,
    DecodedSignal,
    DecodedMessage,
)

logger = logging.getLogger(__name__)


# 1 byte for magic, 1 byte for DLC, 2 bytes for timestamp, 4 bytes for ID
MSG_HDR_SIZE = 1 + 1 + 2 + 4

# Magic indicating the start of a logged CAN message
MSG_MAGIC = 0xBA

# Number of bytes to unpack before logging an update.
UNPACK_CHUNK_SIZE_BYTES = 1_000_000


class Encoder:
    def __init__(self, db: CanDatabase):
        self.db = db

    def encode_msg(self, msg_id, payload: bytes, timestamp_ms: int) -> bytes():
        header = struct.pack("<BBHL", MSG_MAGIC, len(payload), timestamp_ms, msg_id)
        crc = crc8.crc8().update(header).update(payload).digest()
        assert len(crc) == 1
        return header + payload + crc

    def encode(self, msgs: list[DecodedMessage]) -> tuple[bytes, bytes]:
        if len(msgs) == 0:
            return bytes(), bytes()

        start_timestamp = msgs[0].timestamp
        data = bytes()
        for msg in msgs:
            payload = self.db.pack(decoded_msg=msg)

            timestamp_ms = (msg.timestamp - start_timestamp).microseconds // 1000
            timestamp_ms &= 2**16 - 1
            encoded_msg = self.encode_msg(
                msg_id=msg.msg_id, payload=payload, timestamp_ms=timestamp_ms
            )
            data += encoded_msg

        return bytes(), data


class Decoder:
    def __init__(self, db: CanDatabase):
        self.db = db

    def decode_msg(self, data: bytes):
        """
        Decode a raw CAN packet. The format is defined in `firmware/shared/src/io/io_canLogging.c`.
        """
        magic, dlc, timestamp_ms, msg_id = struct.unpack("<BBHL", data[:MSG_HDR_SIZE])

        if magic != MSG_MAGIC:
            raise RuntimeError("Magic is incorrect (not 0xBA)")

        if len(data) < MSG_HDR_SIZE + dlc + 1:
            raise RuntimeError("Insufficient bytes for expected data length")

        size = MSG_HDR_SIZE + dlc
        data_bytes = data[MSG_HDR_SIZE:size]
        rxed_crc = data[size]  # Last byte is CRC8

        calculated_crc = crc8.crc8().update(data[:size]).digest()
        calculated_crc = int.from_bytes(calculated_crc, byteorder="little")

        if calculated_crc != rxed_crc:
            raise RuntimeError(f"CRC mismatch while trying to decode a CAN message")

        return timestamp_ms, msg_id, data_bytes, size

    def decode(self, metadata: bytes, data: bytes) -> list[DecodedSignal]:
        """
        Decode raw CAN log data into a list of signals.
        Each signal is represented as a tuple:
        (timestamp, signal_name, signal_value, signal_label, signal_unit)
        """
        start_timestamp = pd.Timestamp.now()  # TODO

        signals = []
        last_timestamp_ms = pd.Timedelta(milliseconds=0)
        overflow_fix_delta_ms = pd.Timedelta(milliseconds=0)

        i = 0
        while i < len(data):
            if i % UNPACK_CHUNK_SIZE_BYTES == 0:
                percent_unpacked = i / len(data) * 100
                logger.info(f"Unpacked {int(percent_unpacked)}% of bytes in file.")

            # Scan for magic indicating the start of the next log
            if data[i] != MSG_MAGIC:
                i += 1
                continue

            # Parse raw CAN packet.
            packet_data = data[i:]

            try:
                timestamp_ms, msg_id, data_bytes, size = self.decode_msg(
                    data=packet_data
                )
                i += size
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
            parsed_signals = self.db.unpack(
                msg_id=msg_id, data=data_bytes, timestamp=timestamp
            )
            signals.append(parsed_signals)

        return signals
