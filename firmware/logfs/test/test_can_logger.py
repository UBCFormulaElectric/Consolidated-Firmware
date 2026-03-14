import pytest
import os
import sys
from typing import Callable, Optional
import pandas as pd
from logfs import LogFs
from logfs import can_logger
from .conftest import random_data


# Path fuckery so we can import JSONCAN.
script_dir = os.path.dirname(os.path.abspath(__file__))
root_dir = os.path.join(script_dir, "..", "..", "..")
if root_dir not in sys.path:
    sys.path.append(root_dir)

from scripts.code_generation.jsoncan.src.can_database import (
    CanDatabase,
    DecodedMessage,
    DecodedSignal,
)
from scripts.code_generation.jsoncan.src.json_parsing.json_can_parsing import (
    JsonCanParser,
)

JSONCAN_TEST_DIR = os.path.join(
    os.path.dirname(__file__),
    os.pardir,
    os.pardir,
    os.pardir,
    "scripts",
    "code_generation",
    "jsoncan",
    "test",
    "json_configs",
    "valid_json1",
)


@pytest.fixture
def can_db() -> CanDatabase:
    return JsonCanParser(can_data_dir=JSONCAN_TEST_DIR).make_database()


@pytest.fixture
def encoder(can_db: CanDatabase) -> can_logger.Decoder:
    return can_logger.Encoder(db=can_db)


@pytest.fixture
def decoder(can_db: CanDatabase) -> can_logger.Decoder:
    return can_logger.Decoder(db=can_db)


@pytest.fixture
def dummy_timestamp() -> pd.Timestamp:
    return pd.Timestamp(year=2025, month=1, day=1)


def encode_msgs_to_file(
    fs: LogFs,
    file_name: str,
    encoder: can_logger.Encoder,
    msgs: list[DecodedMessage],
    mangle_fn: Optional[Callable] = None,
) -> list[DecodedSignal]:
    # Get a flattened list of the decoded signals.
    signals = []
    for msg in msgs:
        for signal in msg.signals:
            signal.timestamp = msg.timestamp
            signals.append(signal)

    metadata, data = encoder.encode(msgs=msgs, mangle_fn=mangle_fn)

    # Write to disk.
    file = fs.open(file_name, "wx")
    file.write_metadata(metadata)
    file.write(data)
    file.close()

    return signals


def decode_msgs_from_file(
    fs: LogFs, file_name: str, decoder: can_logger.Decoder
) -> list[DecodedSignal]:
    file = fs.open(file_name, "r")
    decoded_signals = decoder.decode(metadata=file.read_metadata(), data=file.read())
    return decoded_signals


def test_encode_decode(
    fs: LogFs,
    encoder: can_logger.Encoder,
    decoder: can_logger.Decoder,
    dummy_timestamp: pd.Timestamp,
):
    msgs = [
        DecodedMessage(
            name="ECU1_BasicSignalTypes",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=0),
            signals=[
                DecodedSignal(name="ECU1_Boolean1", value=1),
                DecodedSignal(name="ECU1_Boolean2", value=0),
                DecodedSignal(name="ECU1_Enum", value=2, label="ENUM_EX_2"),
                DecodedSignal(name="ECU1_UInt8", value=58),
                DecodedSignal(name="ECU1_UInt16", value=1024),
                DecodedSignal(name="ECU1_UInt32", value=0xFFFFFFFF),
            ],
        ),
        DecodedMessage(
            name="ECU1_DecimalNumbers",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=50),
            signals=[
                DecodedSignal(name="ECU1_Decimal1", value=-56.8),
                DecodedSignal(name="ECU1_Decimal2", value=2.45),
            ],
        ),
        DecodedMessage(
            name="ECU1_BasicSignalTypes",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=100),
            signals=[
                DecodedSignal(name="ECU1_Boolean1", value=0),
                DecodedSignal(name="ECU1_Boolean2", value=1),
                DecodedSignal(name="ECU1_Enum", value=1, label="ENUM_EX_1"),
                DecodedSignal(name="ECU1_UInt8", value=24),
                DecodedSignal(name="ECU1_UInt16", value=0xFFFF),
                DecodedSignal(name="ECU1_UInt32", value=0xAB),
            ],
        ),
        DecodedMessage(
            name="ECU1_DecimalNumbers",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=150),
            signals=[
                DecodedSignal(name="ECU1_Decimal1", value=24.5),
                DecodedSignal(name="ECU1_Decimal2", value=2.0),
            ],
        ),
    ]
    expected_signals = encode_msgs_to_file(
        fs=fs, file_name="/test.log", encoder=encoder, msgs=msgs
    )

    decoded_signals = decode_msgs_from_file(
        fs=fs, decoder=decoder, file_name="/test.log"
    )
    assert decoded_signals == expected_signals


def test_encode_with_random_data(
    fs: LogFs,
    encoder: can_logger.Encoder,
    decoder: can_logger.Decoder,
    dummy_timestamp: pd.Timestamp,
):
    msgs = [
        DecodedMessage(
            name="ECU1_BasicSignalTypes",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=0),
            signals=[
                DecodedSignal(name="ECU1_Boolean1", value=1),
                DecodedSignal(name="ECU1_Boolean2", value=0),
                DecodedSignal(name="ECU1_Enum", value=2, label="ENUM_EX_2"),
                DecodedSignal(name="ECU1_UInt8", value=58),
                DecodedSignal(name="ECU1_UInt16", value=1024),
                DecodedSignal(name="ECU1_UInt32", value=0xFFFFFFFF),
            ],
        ),
        DecodedMessage(
            name="ECU1_DecimalNumbers",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=50),
            signals=[
                DecodedSignal(name="ECU1_Decimal1", value=-56.8),
                DecodedSignal(name="ECU1_Decimal2", value=2.45),
            ],
        ),
        DecodedMessage(
            name="ECU1_BasicSignalTypes",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=100),
            signals=[
                DecodedSignal(name="ECU1_Boolean1", value=0),
                DecodedSignal(name="ECU1_Boolean2", value=1),
                DecodedSignal(name="ECU1_Enum", value=1, label="ENUM_EX_1"),
                DecodedSignal(name="ECU1_UInt8", value=24),
                DecodedSignal(name="ECU1_UInt16", value=0xFFFF),
                DecodedSignal(name="ECU1_UInt32", value=0xAB),
            ],
        ),
        DecodedMessage(
            name="ECU1_DecimalNumbers",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=150),
            signals=[
                DecodedSignal(name="ECU1_Decimal1", value=24.5),
                DecodedSignal(name="ECU1_Decimal2", value=2.0),
            ],
        ),
    ]

    # Add some random data between messages and make sure we can still decode them all.
    def add_random_data(_msg: DecodedMessage, encoded_msg: bytes) -> bytes:
        return encoded_msg + bytes([0xAB]) + random_data(size_bytes=32)

    expected_signals = encode_msgs_to_file(
        fs=fs,
        file_name="/test.log",
        encoder=encoder,
        msgs=msgs,
        mangle_fn=add_random_data,
    )

    decoded_signals = decode_msgs_from_file(
        fs=fs, decoder=decoder, file_name="/test.log"
    )
    assert decoded_signals == expected_signals


def test_corrupt_msgs(
    fs: LogFs,
    encoder: can_logger.Encoder,
    decoder: can_logger.Decoder,
    dummy_timestamp: pd.Timestamp,
):
    msgs = [
        DecodedMessage(
            name="ECU1_BasicSignalTypes",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=0),
            signals=[
                DecodedSignal(name="ECU1_Boolean1", value=1),
                DecodedSignal(name="ECU1_Boolean2", value=0),
                DecodedSignal(name="ECU1_Enum", value=2, label="ENUM_EX_2"),
                DecodedSignal(name="ECU1_UInt8", value=58),
                DecodedSignal(name="ECU1_UInt16", value=1024),
                DecodedSignal(name="ECU1_UInt32", value=0xFFFFFFFF),
            ],
        ),
        DecodedMessage(
            name="ECU1_DecimalNumbers",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=50),
            signals=[
                DecodedSignal(name="ECU1_Decimal1", value=-56.8),
                DecodedSignal(name="ECU1_Decimal2", value=2.45),
            ],
        ),
        DecodedMessage(
            name="ECU1_BasicSignalTypes",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=100),
            signals=[
                DecodedSignal(name="ECU1_Boolean1", value=0),
                DecodedSignal(name="ECU1_Boolean2", value=1),
                DecodedSignal(name="ECU1_Enum", value=1, label="ENUM_EX_1"),
                DecodedSignal(name="ECU1_UInt8", value=24),
                DecodedSignal(name="ECU1_UInt16", value=0xFFFF),
                DecodedSignal(name="ECU1_UInt32", value=0xAB),
            ],
        ),
        DecodedMessage(
            name="ECU1_DecimalNumbers",
            timestamp=dummy_timestamp + pd.Timedelta(milliseconds=150),
            signals=[
                DecodedSignal(name="ECU1_Decimal1", value=24.5),
                DecodedSignal(name="ECU1_Decimal2", value=2.0),
            ],
        ),
    ]

    # Corrupt all messages named ECU1_DecimalNumbers
    def corrupt_decimal_numbers(msg: DecodedMessage, encoded_msg: bytes) -> bytes:
        if msg.name == "ECU1_DecimalNumbers":
            # Corrupt by adding 1 to a byte in the middle of the encoded bytes.
            idx_to_corrupt = len(encoded_msg) // 2
            return (
                encoded_msg[:idx_to_corrupt]
                + bytes([encoded_msg[idx_to_corrupt] + 1])
                + encoded_msg[idx_to_corrupt + 1 :]
            )
        else:
            return encoded_msg

    expected_signals = encode_msgs_to_file(
        fs=fs,
        file_name="/test.log",
        encoder=encoder,
        msgs=msgs,
        mangle_fn=corrupt_decimal_numbers,
    )
    expected_signals = [
        signal
        for signal in expected_signals
        # Drop signals from ECU1_DecimalNumbers
        if signal.name not in ["ECU1_Decimal1", "ECU1_Decimal2"]
    ]

    decoded_signals = decode_msgs_from_file(
        fs=fs, decoder=decoder, file_name="/test.log"
    )
    assert decoded_signals == expected_signals
