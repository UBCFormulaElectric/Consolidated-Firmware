import pytest
import os
import sys
import pandas as pd
from logfs import LogFs
from logfs.can_logger import Decoder, Encoder
from .conftest import random_data


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
def encoder(can_db: CanDatabase) -> Decoder:
    return Encoder(db=can_db)


@pytest.fixture
def decoder(can_db: CanDatabase) -> Decoder:
    return Decoder(db=can_db)


def test_basic(encoder: Encoder, decoder: Decoder):
    metadata, data = encoder.encode(
        msgs=[
            DecodedMessage(
                name="ECU1_BasicSignalTypes",
                msg_id=106,
                timestamp=pd.Timestamp.now(),
                signals=[
                    DecodedSignal(
                        name="ECU1_Boolean1",
                        value=1,
                        timestamp=pd.Timestamp.now(),
                        label=None,
                        unit=None,
                    )
                ],
            )
        ]
    )

    print(metadata, data)

    signals = decoder.decode(metadata=metadata, data=data)
    assert signals == []
