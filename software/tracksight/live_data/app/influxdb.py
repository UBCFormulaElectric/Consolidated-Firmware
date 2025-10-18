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