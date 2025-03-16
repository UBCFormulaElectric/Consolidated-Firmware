import os
from typing import LiteralString
from dotenv import load_dotenv

DOCKERIZED: bool = os.environ.get("DOCKERIZED") == "1"
if not DOCKERIZED:
    # this is only on developer machines
    influx_env_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "docker", "live_data.env")
    load_dotenv(dotenv_path=influx_env_path)

INFLUX_BUCKET: str = "can_data"
INFLUX_URL: LiteralString = f"http://{'influx' if DOCKERIZED else 'localhost'}:8086"
INFLUX_ORG: str | None = os.environ.get("INFLUXDB_ORG")
INFLUX_TOKEN: str | None = os.environ.get("ADMIN_TOKEN")
CAR_NAME: str | None = os.environ.get("CAR_NAME")