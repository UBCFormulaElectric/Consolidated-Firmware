import os
from typing import Optional, Callable
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

# Number of bytes to unpack before logging an update
UNPACK_CHUNK_SIZE_BYTES = 1_000_000

# Timestamp of bootup is stored in metadata section. Fields are:
# 1. Second (byte)
# 2. Minute (byte)
# 3. Hour (byte)
# 4. Day (byte)
# 5. Weekday (byte)
# 6. Month (byte)
# 7. Year, since 2000 (byte)
TIMESTAMP_FMT = "<BBBBBBB"

# Every logged CAN message has a header. Fields are:
# 1. Magic, 0xAB (byte)
# 2. DLC (byte)
# 3. Timestamp, in ms (2 bytes)
# 4. Message ID (4 bytes)
MSG_HDR_FMT = "<BBHL"


class Encoder:
    def __init__(self, db: CanDatabase):
        self.db = db

    def _encode_start_timestamp(self, timestamp: pd.Timestamp) -> bytes:
        return struct.pack(
            TIMESTAMP_FMT,
            timestamp.second,
            timestamp.minute,
            timestamp.hour,
            timestamp.day,
            timestamp.weekday(),
            timestamp.month,
            timestamp.year - 2000,
        )

    def _encode_msg(self, start_timestamp: pd.Timestamp, msg: DecodedMessage) -> bytes:
        timestamp_ms = (msg.timestamp - start_timestamp).microseconds // 1000
        timestamp_ms &= 2**16 - 1

        msg_id, payload = self.db.pack(decoded_msg=msg)
        header = struct.pack(MSG_HDR_FMT, MSG_MAGIC, len(payload), timestamp_ms, msg_id)
        crc = crc8.crc8().update(header).update(payload).digest()
        assert len(crc) == 1

        return header + payload + crc

    def encode(
        self,
        msgs: list[DecodedMessage],
        mangle_fn: Optional[Callable] = None,
    ) -> tuple[bytes, bytes]:
        if len(msgs) == 0:
            return bytes(), bytes()

        start_timestamp = msgs[0].timestamp
        metadata = self._encode_start_timestamp(timestamp=start_timestamp)

        data = bytes()
        for msg in msgs:
            encoded_msg = self._encode_msg(start_timestamp=start_timestamp, msg=msg)
            if mangle_fn is not None:
                data += mangle_fn(msg, encoded_msg)
            else:
                data += encoded_msg

        return metadata, data


class Decoder:
    def __init__(self, db: CanDatabase):
        self.db = db

    def _decode_start_timestamp(self, metadata: bytes) -> pd.Timestamp:
        second, minute, hour, day, _weekday, month, year = struct.unpack(
            TIMESTAMP_FMT, metadata[:MSG_HDR_SIZE]
        )
        year += 2000
        start_timestamp = pd.Timestamp(
            year=year, month=month, day=day, hour=hour, minute=minute, second=second
        )
        return start_timestamp

    def _decode_msg(self, data: bytes):
        """
        Decode a raw CAN packet. The format is defined in `firmware/shared/src/io/io_canLogging.c`.
        """
        magic, dlc, timestamp_ms, msg_id = struct.unpack(
            MSG_HDR_FMT, data[:MSG_HDR_SIZE]
        )

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
        """
        start_timestamp = self._decode_start_timestamp(metadata=metadata)

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
                timestamp_ms, msg_id, data_bytes, size = self._decode_msg(
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
            signals.extend(parsed_signals)

        return signals
