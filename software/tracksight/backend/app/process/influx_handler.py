"""
Influx database handler class.

File for handling influxdb queries.
This requires the influx dbrc mapping to have db name == bucket name
TODO: Implement proper error handling for things like no data available.
"""

import os
from typing import List, Tuple, TypedDict

import influxdb_client

# Configs for InfluxDBClient
INFLUX_DB_ORG = "org1"
# "https://us-east-1-1.aws.cloud2.influxdata.com"
INFLUX_DB_URL = "http://localhost:8086"
# "pyh_P66tpmkqnfB6IL73p1GVSyiSK_o5_fmt-1KhZ8eYu_WVoyUMddNsHDlozlstS8gZ0WVyuycQtQOCKIIWJQ=="
INFLUX_DB_TOKEN = os.environ.get("INFLUXDB_TOKEN")
if INFLUX_DB_TOKEN is None:
    raise ValueError("INFLUXDB_TOKEN environment variable not set")


# "testing"
INFLUX_DB_DEFAULT_BUCKET_ID = "example-bucket"

# TODO consider if we should use the context manager as a wrapper for the client
# with InfluxDBClient(url=INFLUX_DB_URL, token=INFLUX_DB_TOKEN, org=INFLUX_DB_ORG) as client:
# INFLUX_DB_CLIENT = influxdb_client.InfluxDBClient(
#     url=INFLUX_DB_URL, token=INFLUX_DB_TOKEN, org=INFLUX_DB_ORG
# )


# Checks if the vehicle bucket exists, and if not, creates it
with influxdb_client.InfluxDBClient(
    url=INFLUX_DB_URL, token=INFLUX_DB_TOKEN, org=INFLUX_DB_ORG
) as _client:
    if _client.buckets_api().find_bucket_by_name(INFLUX_DB_DEFAULT_BUCKET_ID) is None:
        _client.buckets_api().create_bucket(bucket_name=INFLUX_DB_DEFAULT_BUCKET_ID)


def get_measurements(bucket=INFLUX_DB_DEFAULT_BUCKET_ID) -> list[str]:
    """
    Get all measurements from the database.
    :param bucket: Name of bucket to fetch data from.
    :returns List of all measurements.
    """
    with influxdb_client.InfluxDBClient(
        url=INFLUX_DB_URL, token=INFLUX_DB_TOKEN, org=INFLUX_DB_ORG
    ) as client:
        return [
            str(i[0])  # "i" is an array of form ["_result", 0, <measurement_name>]
            for i in client.query_api()
            .query(
                f"""
        import \"influxdata/influxdb/schema\"
        schema.measurements(bucket: \"{bucket}\")
        """
            )
            .to_values(columns=["_value"])
        ]


def get_fields(
    measurement: str, bucket: str = INFLUX_DB_DEFAULT_BUCKET_ID
) -> list[str]:
    """
    Get all fields from a measurement.
    :param measurement: Measurement to fetch fields from.
    :param bucket: Name of bucket to fetch data from.
    :return: List of all fields.
    """
    with influxdb_client.InfluxDBClient(
        url=INFLUX_DB_URL, token=INFLUX_DB_TOKEN, org=INFLUX_DB_ORG
    ) as client:
        return [
            str(i[0])  # "i" is an array of form ["_result", 0, <measurement_name>]
            for i in client.query_api()
            .query(
                f"""
        import \"influxdata/influxdb/schema\"
        schema.measurementFieldKeys(bucket: \"{bucket}\", measurement: \"{measurement}\")
        """
            )
            .to_values(columns=["_value"])
        ]


class TimeValue(TypedDict):
    """
    TypedDict for the time and value fields in a query response.
    """

    times: list[str]
    values: list[str]


def query(
    measurement: str,
    fields: List[str],
    time_range: Tuple[int, int],
    bucket: str = INFLUX_DB_DEFAULT_BUCKET_ID,
    max_points: int = 8000,
    ms_resolution: int = 100,
) -> dict[str, TimeValue]:
    """
    Make a general query to the database.
    :param measurement: Measurement to pull data from.
    :param fields: Fields to fetch.
    :param time_range: Tuple like (time start, time end) to specify the time interval.
    :param bucket: Name of bucket to fetch data from.
    :param max_points: Maximum number of datapoints to fetch.
    :param ms_resolution: Minimum time delta required before grabbing a new datapoint.
    :return:
    """
    out: dict[str, TimeValue] = {field: {"times": [], "values": []} for field in fields}
    with influxdb_client.InfluxDBClient(
        url=INFLUX_DB_URL, token=INFLUX_DB_TOKEN, org=INFLUX_DB_ORG
    ) as client:
        for field, value, time in (
            client.query_api()
            .query(
                f"""
    from(bucket:"{bucket}")
            |> range(start: {time_range[0]}, stop: {time_range[1]})
            |> filter(fn: (r) => r._measurement == "{measurement}" and contains(value: r._field, set: {str(fields).replace("'", '"')}))
        """
            )
            .to_values(columns=["_field", "_value", "_time"])
        ):
            out[field]["times"].append(time)
            out[field]["values"].append(value)
    return out
