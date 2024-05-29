"""
Influx database handler class.

File for handling influxdb queries.
This requires the influx dbrc mapping to have db name == bucket name
TODO: Implement proper error handling for things like no data available.
"""

import os
from typing import List, Tuple, TypedDict
import influxdb_client

REQUIRED_ENV_VARS = {
    "org": "DOCKER_INFLUXDB_INIT_ORG",
    "bucket": "DOCKER_INFLUXDB_INIT_BUCKET",
    "token": "DOCKER_INFLUXDB_INIT_ADMIN_TOKEN",
}
for env_var in REQUIRED_ENV_VARS.values():
    if os.environ.get(env_var) is None:
        raise RuntimeError(f"Required environment variable not set: {env_var}")

# Configs for Influx DB instance.
INFLUX_DB_URL = "http://influx:8086"
INFLUX_DB_ORG = os.environ.get(REQUIRED_ENV_VARS["org"])
INFLUX_DB_BUCKET = os.environ.get(REQUIRED_ENV_VARS["bucket"])
INFLUX_DB_TOKEN = os.environ.get(REQUIRED_ENV_VARS["token"])

print(f"Using URL {INFLUX_DB_URL} with token {INFLUX_DB_TOKEN}.")

# Checks if the vehicle bucket exists, and if not, creates it
with influxdb_client.InfluxDBClient(
    url=INFLUX_DB_URL, token=INFLUX_DB_TOKEN, org=INFLUX_DB_ORG
) as client:
    if client.buckets_api().find_bucket_by_name(INFLUX_DB_BUCKET) is None:
        client.buckets_api().create_bucket(bucket_name=INFLUX_DB_BUCKET)


def get_measurements(bucket=INFLUX_DB_BUCKET) -> list[str]:
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


def get_fields(measurement: str, bucket: str = INFLUX_DB_BUCKET) -> list[str]:
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
    time_range: Tuple[str, str],
    bucket: str = INFLUX_DB_BUCKET,
    max_points: int = 8000,  # TODO implement
    ms_resolution: int = 100,  # TODO implement
) -> dict[str, TimeValue]:
    """
    Make a general query to the database.
    :param measurement: Measurement to pull data from.
    :param fields: Fields to fetch.
    :param time_range: Tuple like (time start, time end) to specify the time interval.
    :param bucket: Name of bucket to fetch data from.
    :param max_points: Maximum number of datapoints to fetch.
    :param ms_resolution: Minimum time delta required before grabbing a new datapoint.
    :return: A dictionary where the keys are the fields and the values are TimeValue objects.
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
