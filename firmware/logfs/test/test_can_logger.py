import pytest
import os
import sys
import pandas as pd
from logfs import LogFs
from logfs import can_logger


# Path fuckery so we can import JSONCAN.
script_dir = os.path.dirname(os.path.abspath(__file__))
root_dir = os.path.join(script_dir, "..", "..", "..", "..")
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


def test_round_trip(
    fs: LogFs, encoder: can_logger.Encoder, decoder: can_logger.Decoder
):
    start_timestamp = pd.Timestamp.fromisocalendar(year=2025, week=2, day=7)
    msgs = [
        DecodedMessage(
            name="ECU1_BasicSignalTypes",
            timestamp=start_timestamp + pd.Timedelta(milliseconds=0),
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
            name="ECU1_BasicSignalTypes",
            timestamp=start_timestamp + pd.Timedelta(milliseconds=100),
            signals=[
                DecodedSignal(name="ECU1_Boolean1", value=0),
                DecodedSignal(name="ECU1_Boolean2", value=1),
                DecodedSignal(name="ECU1_Enum", value=1, label="ENUM_EX_1"),
                DecodedSignal(name="ECU1_UInt8", value=24),
                DecodedSignal(name="ECU1_UInt16", value=0xFFFF),
                DecodedSignal(name="ECU1_UInt32", value=0xAB),
            ],
        ),
    ]
    expected_signals = []
    for msg in msgs:
        for signal in msg.signals:
            signal.timestamp = msg.timestamp
            expected_signals.append(signal)

    metadata, data = encoder.encode(msgs=msgs)
    file = fs.open("/test.txt", "wx")
    file.write_metadata(metadata)
    file.write(data)
    file.close()
    del file, metadata, data

    file = fs.open("/test.txt", "r")
    decoded_signals = decoder.decode(metadata=file.read_metadata(), data=file.read())
    assert decoded_signals == expected_signals
