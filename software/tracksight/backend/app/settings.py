import os
from pathlib import Path

from dotenv import load_dotenv

def get_env_bool(e: str):
    val = os.environ.get(e)
    return False if val is None else val.lower() in ("yes", "true", "t", "1")

DOCKERIZED: bool = get_env_bool("DOCKERIZED")

# App running on docker should have existing environment variables set up
if not DOCKERIZED:
    _DOTENV_PATH = Path(__file__).resolve().parent.parent / "local_backend.env"
    if _DOTENV_PATH.exists():
        load_dotenv(_DOTENV_PATH)
    else:
        raise FileNotFoundError(f"Environment file not found at {_DOTENV_PATH}")

INFLUX_ON: bool = os.environ.get("INFLUX_ON", "TRUE").upper() == "TRUE"
INFLUX_BUCKET: str = os.environ.get("INFLUXDB_BUCKET", "can_data")
INFLUX_URL: str | None = os.environ.get("INFLUXDB_URL")
INFLUX_ORG: str | None = os.environ.get("INFLUXDB_ORG")
INFLUX_TOKEN: str | None = os.environ.get("INFLUX_TOKEN")

if "CAR_NAME" not in os.environ:
    raise KeyError("Environment variable 'CAR_NAME' is not set")
CAR_NAME: str = os.environ["CAR_NAME"]

# booting configure
DATA_SOURCE = os.environ.get("DATA_SOURCE", "mock").upper()
ENABLE_MOCK = get_env_bool("ENABLE_MOCK")
ENABLE_WIRELESS = get_env_bool("ENABLE_WIRELESS")

BACKEND_PORT = int(os.environ.get("BACKEND_PORT", "5000"))
SERIAL_PORT = os.environ.get("SERIAL_PORT")
DATA_FILE = os.environ.get("DATA_FILE")
DEBUG = get_env_bool("DEBUG")

SERVER_IP = os.environ.get("SERVER_IP", None)
SERVER_DOMAIN_NAME = os.environ.get("SERVER_DOMAIN_NAME", "telem")

LOG_LEVEL = os.environ.get("LOG_LEVEL", "CRITICAL").upper()