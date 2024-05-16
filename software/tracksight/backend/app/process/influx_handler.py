"""
Influx Handler
"""

import requests
from dateutil.parser import parse

INFLUX_DB_URL = "https://us-east-1-1.aws.cloud2.influxdata.com"
# INFLUX_DB_URL = "http://localhost:8086"
BUCKET = "testing"
TEMP_TOKEN = "pyh_P66tpmkqnfB6IL73p1GVSyiSK_o5_fmt-1KhZ8eYu_WVoyUMddNsHDlozlstS8gZ0WVyuycQtQOCKIIWJQ=="


class NoDataForQueryException(Exception):
    """Raised when no data was found for a specific query"""

    pass


class InfluxHandler:
    """
    Class for handling influxdb queries
    This requires the influx dbrc mapping to have db name == bucket name
    TODO: Implement proper error handling for things like no data available.
    """

    @staticmethod
    def _gen_headers():
        headers = {
            "Authorization": f"Token {TEMP_TOKEN}",
            "Content-type": "application/json",
        }
        return headers

    # NOTE unused
    @staticmethod
    def get_bucket_names_and_ids():
        """

        :return:
        """
        headers = InfluxHandler._gen_headers()
        params = {}
        response = requests.get(
            f"{INFLUX_DB_URL}/api/v2/buckets", headers=headers, params=params
        )

        response_json = response.json()
        return [
            {"name": bucket["name"], "id": bucket["id"]}
            for bucket in response_json["buckets"]
            if not bucket["name"].startswith("_")
        ]

    @staticmethod
    def get_measurements(db=BUCKET):
        """

        :param db:
        :return:
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
    def get_fields(measurement, db=BUCKET):
        """

        :param measurement:
        :param db:
        :return:
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
        measurement, fields, time_range, db=BUCKET, max_points=8000, ms_resolution=500
    ):
        """

        :param measurement:
        :param fields:
        :param time_range:
        :param db:
        :param max_points:
        :param ms_resolution:
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
        prevTime = parse(values[0][0])
        max_val = 0  # TODO rename

        for value in values:
            nextTime = parse(value[0])
            timeDelta = nextTime - prevTime

            if max_val > max_points:
                break

            if timeDelta.total_seconds() <= ms_resolution / 1000:
                continue

            for i in range(1, len(value)):
                column = columns[i]
                data[column]["time"].append(value[0])
                data[column]["value"].append(value[i])

            prevTime = nextTime
            max_val += 1

        return data
