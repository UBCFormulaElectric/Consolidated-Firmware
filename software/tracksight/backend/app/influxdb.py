from datetime import datetime, date, timedelta
import influxdb_client
from influxdb_client.client.influxdb_client import InfluxDBClient
from settings import INFLUX_ON, INFLUX_URL, INFLUX_TOKEN, INFLUX_ORG, CAR_NAME

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

FLUX_TIME_FORMAT = "%Y-%m-%dT%H:%M:%SZ"
DATE_FORMATS = [
    "%Y-%m-%dT%H:%M:%S",
    "%Y-%m-%dT%H:%M", 
    "%Y-%m-%dT%H", 
    "%Y-%m-%d", 
]

def parse_time(time: str) -> date:
    """
    Takes a string and attempts to parse into the formats
    """
    for f in DATE_FORMATS:
        try:
            return datetime.strptime(time, f)
        except:
            continue
    raise BadTimeFormat(f"Unable to parse time: {time}")

def get_timed_query(bucket:str, start: str = None, end: str = None) -> str:
    """
    Returns InfluxDB time range query string for given start and end time.

    By default, 5 minute interval of data is returned unless both `start` and `end` parameters are provided.

    Throws BadTimeFormat if time format is invalid
    """

    start_time: date | None = parse_time(start) if start else None
    end_time: date | None = parse_time(end) if end else None
    default_interval = timedelta(minutes=5)

    # unless both start and end are provided,
    # return data in 5 minute interval relative to whichever is given or now
    # check end time first
    if not end_time:
        end_time = start_time + default_interval if start_time else datetime.now()

    if not start_time:
        start_time = end_time - default_interval

    return f'from(bucket:"{bucket}")\
        |> range(start: {start_time.strftime(FLUX_TIME_FORMAT)}, stop: {end_time.strftime(FLUX_TIME_FORMAT)})\
        |> drop(columns: ["_start", "_stop"])'

def get_influxdb_client() -> InfluxDBClient:
    """
    Returns InfluxDB client with configured settings
    Use with context window, i.e. `with get_influxdb_client() as client:...`
    """
    if not INFLUX_ON:
        raise Exception("InfluxDB is turned off in settings")
    return influxdb_client.InfluxDBClient(
        url=INFLUX_URL,
        token=INFLUX_TOKEN,
        org=INFLUX_ORG,
        debug=False,
    )