import re
import influxdb_client
from influxdb_client.client.influxdb_client import InfluxDBClient
from settings import INFLUX_URL, INFLUX_TOKEN, INFLUX_ORG, CAR_NAME

if INFLUX_URL is None:
    raise Exception("No Influx URL Provided")
if INFLUX_ORG is None:
    raise Exception("No Influx Organization Provided")
if INFLUX_TOKEN is None:
    raise Exception("No Token Provided for Influx")
if CAR_NAME is None:
    raise Exception("No Car Name is Provided")

DEFAULT_TIME_INTERVAL = "5m"

class BadTimeFormat(Exception):
    pass

# verify date format to prevent arbitrary queries
# something like <YYYY>-<MM>-<DD>T<HH>:<MM>:<SS>Z
# thanks chatgpt
RFC3339_REGEX = re.compile(
    r"^\d{4}-\d{2}-\d{2}"
    r"(T\d{2}:\d{2}:\d{2}(?:\.\d+)?(?:Z|[+-]\d{2}:\d{2})?)?$"
)

def get_timed_query(bucket:str, start_time: str = None, end_time: str = None) -> str:
    """
    Returns InfluxDB time range query string for given start and end time.

    By default, 5 minute interval of data is returned unless both `start` and `end` parameters are provided.

    Throws BadTimeFormat if time format is invalid
    """

    # unless both start and end are provided,
    # return data in 5 minute interval relative to whichever is given or now
    # check end time first
    if not end_time:
        end_time = f"{start_time} + 5m" if start_time else "now()" #TODO fix the time
    elif not RFC3339_REGEX.match(end_time):
        raise BadTimeFormat(f"Invalid end time format, must be RFC3339: {end_time}")

    # check start time
    if not start_time:
        start_time = "-5m"
    elif not RFC3339_REGEX.match(start_time):
        raise BadTimeFormat(f"Invalid start time format, must be RFC3339: {start_time}")

    return f'from(bucket:"{bucket}")\
        |> range(start: {start_time}, stop: {end_time})\
        |> drop(columns: ["_start", "_stop"])'

def get_influxdb_client() -> InfluxDBClient:
    """
    Returns InfluxDB client with configured settings
    Use with context window, i.e. `with get_influxdb_client() as client:...`
    """
    return influxdb_client.InfluxDBClient(
        url=INFLUX_URL,
        token=INFLUX_TOKEN,
        org=INFLUX_ORG,
        debug=False,
    )