import os
from pathlib import Path
from typing import LiteralString

from dotenv import load_dotenv

load_dotenv(Path(__file__).resolve().parent.parent / ".env")

DOCKERIZED: bool = os.environ.get("DOCKERIZED") == "1"

INFLUX_BUCKET: str = os.environ.get("INFLUXDB_BUCKET", "can_data")
INFLUX_URL: LiteralString = os.environ.get(
    "INFLUXDB_URL", f"http://{'influx' if DOCKERIZED else 'localhost'}:8086"
)
INFLUX_ORG: str | None = os.environ.get("INFLUXDB_ORG")
INFLUX_TOKEN: str | None = os.environ.get("ADMIN_TOKEN")


CAR_NAME: str | None = os.environ.get("CAR_NAME")

# booting configure
ENABLE_MORK = bool(os.environ.get("ENABLE_MORK"))
ENABLE_WIRELESS = bool(os.environ.get("ENABLE_WIRELESS"))

SERIAL_PORT = os.environ.get("SERIAL_PORT")
DATA_FILE = os.environ.get("DATA_FILE")
DEBUG = bool(os.environ.get("DEBUG"))
