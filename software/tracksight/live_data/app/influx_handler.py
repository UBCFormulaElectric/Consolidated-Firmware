"""
Influx database handler class.

File for handling influxdb queries.
This requires the influx dbrc mapping to have db name == bucket name
TODO: Implement proper error handling for things like no data available.
"""

import pandas as pd
from typing import List, Tuple
import influxdb_client
from logger import logger

BASIC_TIMEOUT_MS = 10_000
QUERY_TIMEOUT_MS = 100_000

_client: influxdb_client.InfluxDBClient
_bucket: str
_org: str

def setup(
    url: str,
    token: str,
    bucket: str = "quadruna",
    org: str = "ubcformulaelectric",
):
    global _bucket
    global _org
    global _client

    _bucket = bucket
    _org = org

    # Checks if the vehicle bucket exists, and if not, creates it
    logger.info(f"Connecting to InfluxDB database at '{url}' with token '{token}'.")
    _client = influxdb_client.InfluxDBClient(
        url=url, token=token, org=org, timeout=BASIC_TIMEOUT_MS
    )
    if _client.buckets_api().find_bucket_by_name(bucket_name=bucket) is None:
        _client.buckets_api().create_bucket(bucket_name=bucket)

def get_measurements(cls) -> list[str]:
    """
    Get all measurements from the database.
    :param bucket: Name of bucket to fetch data from.
    :returns List of all measurements.
    """
    if not cls.is_setup:
        raise RuntimeError("InfluxHandler not initialized.")

    query = f"""
    import "influxdata/influxdb/schema"
    schema.measurements(bucket: \"{_bucket}\")"""

    return [
        str(i[0])
        for i in _client.query_api().query(query).to_values(columns=["_value"])
    ]

def get_signals(measurement: str) -> list[str]:
    """
    Get all signals from the database.
    :param bucket: Name of bucket to fetch data from.
    :returns List of all measurements.
    """
    query = f"""
    import "influxdata/influxdb/schema"
    schema.tagValues(
        bucket: "{_bucket}", 
        predicate: (r) => r._measurement == "{measurement}",
        tag: "signal"
    )"""

    return [
        str(i[0])
        for i in _client.query_api()
        .query(query=query)
        .to_values(columns=["_value"])
    ]

def query(
    measurement: str,
    signals: List[str],
    time_range: Tuple[str, str],
    max_points: int,
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
    from(bucket:"{_bucket}")
        |> range(start: {time_range[0]}, stop: {time_range[1]})
        |> filter(fn: (r) => 
            r._measurement == "{measurement}" and 
            r._field == "value" and
            contains(value: r.signal, set: {str(signals).replace("'", '"')}))
        |> tail(n: {max_points})
    """

    query_result = {signal: {"times": [], "values": []} for signal in signals}
    for signal, value, time in (
        _client.query_api()
        .query(query=query)
        .to_values(columns=["signal", "_value", "_time"])
    ):
        query_result[signal]["times"].append(str(time))
        query_result[signal]["values"].append(value)

    return query_result

@classmethod
def write(df: pd.DataFrame, measurement: str) -> None:
    """
    Write a pandas dataframe to the Influx database. The dataframe should have the columns
    time, value, unit, and signal.
    :param db: Dataframe to upload.
    """
    # Index is used as source for time.
    df.set_index("time", inplace=True)

    write_api = _client.write_api()
    write_api.write(
        bucket=_bucket,
        org=_org,
        record=df,
        data_frame_measurement_name=measurement,
        data_frame_tag_columns=["signal"],
        write_precision=influxdb_client.WritePrecision.NS,
    )
