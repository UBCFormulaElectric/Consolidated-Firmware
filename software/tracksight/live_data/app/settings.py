import os
from pathlib import Path
from typing import LiteralString

from dotenv import load_dotenv

_DOTENV_PATH = Path(__file__).resolve().parent.parent / ".env"
if _DOTENV_PATH.exists():
    load_dotenv(_DOTENV_PATH)
else:
    raise FileNotFoundError(f"Environment file not found at {_DOTENV_PATH}")


def str2bool(v: str | None):
    assert v is not None, "Environment variable cannot be None"
    return v.lower() in ("yes", "true", "t", "1")


DOCKERIZED: bool = os.environ.get("DOCKERIZED") == "1"

INFLUX_BUCKET: str = os.environ.get("INFLUXDB_BUCKET", "can_data")
INFLUX_URL: LiteralString = os.environ.get(
    "INFLUXDB_URL", f"http://{'influx' if DOCKERIZED else 'localhost'}:8086"
)
INFLUX_ORG: str | None = os.environ.get("INFLUXDB_ORG")
INFLUX_TOKEN: str | None = os.environ.get("ADMIN_TOKEN")

if "CAR_NAME" not in os.environ:
    raise KeyError("Environment variable 'CAR_NAME' is not set")
CAR_NAME: str = os.environ["CAR_NAME"]

# booting configure
DATA_SOURCE = os.environ.get("DATA_SOURCE", "mock").upper()

SERIAL_PORT = os.environ.get("SERIAL_PORT")
DATA_FILE = os.environ.get("DATA_FILE")
DEBUG = str2bool(os.environ.get("DEBUG"))

SERVER_IP = os.environ.get("SERVER_IP", "192.168.137.1")
SERVER_DOMAIN_NAME = os.environ.get("SERVER_DOMAIN_NAME", "telem")
