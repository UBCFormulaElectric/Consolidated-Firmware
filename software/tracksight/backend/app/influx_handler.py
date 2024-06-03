"""
Influx database handler class.

File for handling influxdb queries.
This requires the influx dbrc mapping to have db name == bucket name
TODO: Implement proper error handling for things like no data available.
"""

import os
import pandas as pd
from typing import List, Tuple
import influxdb_client
import logging


logger = logging.getLogger("telemetry_logger")


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
    query = f"""
    import "influxdata/influxdb/schema"
    schema.measurements(bucket: \"{bucket}\")"""
    with influxdb_client.InfluxDBClient(
        url=INFLUX_DB_URL, token=INFLUX_DB_TOKEN, org=INFLUX_DB_ORG
    ) as client:
        return [
            str(i[0])
            for i in client.query_api().query(query).to_values(columns=["_value"])
        ]


def get_signals(measurement: str = None, bucket: str = INFLUX_DB_BUCKET) -> list[str]:
    """
    Get all signals from the database.
    :param bucket: Name of bucket to fetch data from.
    :returns List of all measurements.
    """
    query = f"""
    import "influxdata/influxdb/schema"
    schema.tagValues(
        bucket: "{bucket}", 
        predicate: (r) => r._measurement == "{measurement}",
        tag: "signal"
    )"""

    with influxdb_client.InfluxDBClient(
        url=INFLUX_DB_URL, token=INFLUX_DB_TOKEN, org=INFLUX_DB_ORG
    ) as client:
        return [
            str(i[0])
            for i in client.query_api().query(query=query).to_values(columns=["_value"])
        ]


def query(
    measurement: str,
    signals: List[str],
    time_range: Tuple[str, str],
    bucket: str = INFLUX_DB_BUCKET,
    max_points: int = 8000,  # TODO implement
    ms_resolution: int = 100,  # TODO implement
) -> dict[str, dict]:
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

    query = f"""
    from(bucket:"{bucket}")
        |> range(start: {time_range[0]}, stop: {time_range[1]})
        |> filter(fn: (r) => 
            r._measurement == "{measurement}" and 
            r._field == "value" and
            contains(value: r.signal, set: {str(signals).replace("'", '"')}))
    """

    query_result = {signal: {"times": [], "values": []} for signal in signals}
    with influxdb_client.InfluxDBClient(
        url=INFLUX_DB_URL, token=INFLUX_DB_TOKEN, org=INFLUX_DB_ORG
    ) as client:
        for signal, value, time in (
            client.query_api()
            .query(query=query)
            .to_values(columns=["signal", "_value", "_time"])
        ):
            query_result[signal]["times"].append(time)
            query_result[signal]["values"].append(value)

    return query_result


def write(df: pd.DataFrame, measurement: str) -> None:
    """
    Write a pandas dataframe to the Influx database. The dataframe should have the columns
    time, signal, value, and unit.
    :param db: Dataframe to upload.
    """
    with influxdb_client.InfluxDBClient(
        url=INFLUX_DB_URL, token=INFLUX_DB_TOKEN, org=INFLUX_DB_ORG
    ) as client:
        # Index is used as source for time.
        df.set_index("time", inplace=True)

        write_api = client.write_api()
        write_api.write(
            bucket=INFLUX_DB_BUCKET,
            org=INFLUX_DB_ORG,
            record=df,
            data_frame_measurement_name=measurement,
            data_frame_tag_columns=["signal"],
        )
