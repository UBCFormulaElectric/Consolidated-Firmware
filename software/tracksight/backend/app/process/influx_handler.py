"""
Influx database handler class.
"""

from typing import List, Tuple, Dict
import requests
from dateutil.parser import parse

INFLUX_DB_URL = "https://us-east-1-1.aws.cloud2.influxdata.com"
# INFLUX_DB_URL = "http://localhost:8086"
BUCKET = "testing"
TEMP_TOKEN = "pyh_P66tpmkqnfB6IL73p1GVSyiSK_o5_fmt-1KhZ8eYu_WVoyUMddNsHDlozlstS8gZ0WVyuycQtQOCKIIWJQ=="


class NoDataForQueryException(Exception):
    """
    Raised when no data was found for a specific query
    """

    pass


class InfluxHandler:
    """
    Class for handling influxdb queries.
    This requires the influx dbrc mapping to have db name == bucket name
    TODO: Implement proper error handling for things like no data available.
    """

    @staticmethod
    def _gen_headers() -> Dict:
        """
        :returns Required headers for proper database querying.
        """
        headers = {
            "Authorization": f"Token {TEMP_TOKEN}",
            "Content-type": "application/json",
        }
        return headers

    @staticmethod
    def get_measurements(db: str = BUCKET) -> List:
        """
        Get all measurements from the database.
        :param db: Name of bucket to fetch data from.
        :returns List of all measurements.
        """
        headers = InfluxHandler._gen_headers()
        params = {
            "db": db,
            "q": f"SHOW MEASUREMENTS ON {db}",
        }
        response = requests.get(
            f"{INFLUX_DB_URL}/query", headers=headers, params=params
        )

        results = response.json()["results"][0]
        # Very jank, not sure if this is correct
        return [measurement["values"][0][0] for measurement in results["series"]]

    @staticmethod
    def get_fields(measurement: str, db: str = BUCKET) -> List:
        """
        Get all fields from a measurement.
        :param measurement: Measurement to fetch fields from.
        :param db: Name of bucket to fetch data from.
        :return: List of all fields.
        """
        headers = InfluxHandler._gen_headers()
        params = {
            "db": db,
            "q": f"SHOW FIELD KEYS ON {db} FROM {measurement}",
        }
        response = requests.get(
            f"{INFLUX_DB_URL}/query", headers=headers, params=params
        )

        # lol
        results = response.json()["results"][0]["series"][0]["values"]
        return [field_pair[0] for field_pair in results]

    @staticmethod
    def query(
        measurement: str,
        fields: List[str],
        time_range: Tuple[int, int],
        db: str = BUCKET,
        max_points: int = 8000,
        ms_resolution: int = 100,
    ) -> Dict:
        """
        Make a general query to the database.
        :param measurement: Measurement to pull data from.
        :param fields: Fields to fetch.
        :param time_range: Tuple like (time start, time end) to specify the time interval.
        :param db: Name of bucket to fetch data from.
        :param max_points: Maximum number of datapoints to fetch.
        :param ms_resolution: Minimum time delta required before grabbing a new datapoint.
        :return:
        """
        headers = InfluxHandler._gen_headers()
        fields_string = ",".join(fields)
        query = f"SELECT {fields_string} FROM {measurement} WHERE time >= {time_range[0]} AND time <= {time_range[1]} tz('America/Los_Angeles')"
        params = {
            "db": db,
            "q": query,
        }
        response = requests.get(
            f"{INFLUX_DB_URL}/query", headers=headers, params=params
        )

        results = response.json()["results"][0]
        if "series" not in results:
            raise NoDataForQueryException("No data found for this query")

        results = results["series"][0]
        columns = results["columns"]
        values = results["values"]

        data = {column: {"time": [], "value": []} for column in columns[1:]}
        prev_time = parse(values[0][0])
        cur_point = 0

        for value in values:
            nextTime = parse(value[0])
            time_delta = nextTime - prev_time

            if cur_point > max_points:
                break

            if time_delta.total_seconds() <= ms_resolution / 1000:
                continue

            for i in range(1, len(value)):
                column = columns[i]
                data[column]["time"].append(value[0])
                data[column]["value"].append(value[i])

            prev_time = nextTime
            cur_point += 1

        return data
