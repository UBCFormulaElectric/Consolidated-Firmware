"""
Influx database handler class.

File for handling influxdb queries.
This requires the influx dbrc mapping to have db name == bucket name
TODO: Implement proper error handling for things like no data available.
"""

import pandas as pd
from typing import List, Tuple
import influxdb_client
import logging


logger = logging.getLogger("telemetry_logger")


class InfluxHandler:
    is_setup = False

    @classmethod
    def setup(
        cls,
        url: str,
        token: str,
        bucket: str = "quadruna",
        org: str = "ubcformulaelectric",
    ):
        cls.url = url
        cls.token = token
        cls.bucket = bucket
        cls.org = org
        cls.is_setup = True

        logger.info(f"Connecting to InfluxDB database at '{url}' with token '{token}'.")

        # Checks if the vehicle bucket exists, and if not, creates it
        with influxdb_client.InfluxDBClient(url=url, token=token, org=org) as client:
            if client.buckets_api().find_bucket_by_name(bucket_name=bucket) is None:
                client.buckets_api().create_bucket(bucket_name=bucket)

    @classmethod
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
        schema.measurements(bucket: \"{cls.bucket}\")"""

        with influxdb_client.InfluxDBClient(
            url=cls.url, token=cls.token, org=cls.org
        ) as client:
            return [
                str(i[0])
                for i in client.query_api().query(query).to_values(columns=["_value"])
            ]

    @classmethod
    def get_signals(cls, measurement: str) -> list[str]:
        """
        Get all signals from the database.
        :param bucket: Name of bucket to fetch data from.
        :returns List of all measurements.
        """
        if not cls.is_setup:
            raise RuntimeError("InfluxHandler not initialized.")

        query = f"""
        import "influxdata/influxdb/schema"
        schema.tagValues(
            bucket: "{cls.bucket}", 
            predicate: (r) => r._measurement == "{measurement}",
            tag: "signal"
        )"""

        with influxdb_client.InfluxDBClient(
            url=cls.url, token=cls.token, org=cls.org
        ) as client:
            return [
                str(i[0])
                for i in client.query_api()
                .query(query=query)
                .to_values(columns=["_value"])
            ]

    @classmethod
    def query(
        cls,
        measurement: str,
        signals: List[str],
        time_range: Tuple[str, str],
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
        if not cls.is_setup:
            raise RuntimeError("InfluxHandler not initialized.")

        query = f"""
        from(bucket:"{cls.bucket}")
            |> range(start: {time_range[0]}, stop: {time_range[1]})
            |> filter(fn: (r) => 
                r._measurement == "{measurement}" and 
                r._field == "value" and
                contains(value: r.signal, set: {str(signals).replace("'", '"')}))
        """

        query_result = {signal: {"times": [], "values": []} for signal in signals}
        with influxdb_client.InfluxDBClient(
            url=cls.url, token=cls.token, org=cls.org
        ) as client:
            for signal, value, time in (
                client.query_api()
                .query(query=query)
                .to_values(columns=["signal", "_value", "_time"])
            ):
                query_result[signal]["times"].append(str(time))
                query_result[signal]["values"].append(value)

        return query_result

    @classmethod
    def write(cls, df: pd.DataFrame, measurement: str) -> None:
        """
        Write a pandas dataframe to the Influx database. The dataframe should have the columns
        time, value, unit, and signal.
        :param db: Dataframe to upload.
        """
        if not cls.is_setup:
            raise RuntimeError("InfluxHandler not initialized.")

        with influxdb_client.InfluxDBClient(
            url=cls.url, token=cls.token, org=cls.org
        ) as client:
            # Index is used as source for time.
            df.set_index("time", inplace=True)

            write_api = client.write_api()
            write_api.write(
                bucket=cls.bucket,
                org=cls.org,
                record=df,
                data_frame_measurement_name=measurement,
                data_frame_tag_columns=["signal"],
                write_precision=influxdb_client.WritePrecision.NS,
            )
