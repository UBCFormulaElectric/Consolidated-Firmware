import requests

URL = "https://us-east-1-1.aws.cloud2.influxdata.com"
BUCKET = "testing"

TEMP_TOKEN = "pyh_P66tpmkqnfB6IL73p1GVSyiSK_o5_fmt-1KhZ8eYu_WVoyUMddNsHDlozlstS8gZ0WVyuycQtQOCKIIWJQ=="

# This requires the influx dbrc mapping to have db name == bucket name
class InfluxHandler:
    @staticmethod
    def _gen_headers():
        headers = {
            "Authorization": f"Token {TEMP_TOKEN}",
            "Content-type": "application/json",
        }
        return headers

    @staticmethod
    def get_bucket_names_and_ids():
        headers = InfluxHandler._gen_headers()
        params = {
        }
        response = requests.get(
            f"{URL}/api/v2/buckets",
            headers=headers,
            params=params
        )

        response_json = response.json()
        return [
            {"name": bucket["name"], "id": bucket["id"]} 
            for bucket in response_json["buckets"] 
            if not bucket["name"].startswith("_")
        ]

    @staticmethod
    def get_measurements(db):
        headers = InfluxHandler._gen_headers()
        params = {
            "db": db,
            "q": f"SHOW MEASUREMENTS ON {db}",
        }
        response = requests.get(
            f"{URL}/query",
            headers=headers,
            params=params
        )

        results = response.json()["results"][0]
        # Very jank, not sure if this is correct
        return [
            measurement["values"][0][0] for measurement in results["series"]
        ]

    @staticmethod
    def get_fields(db, measurement):
        headers = InfluxHandler._gen_headers()
        params = {
            "db": db,
            "q": f"SHOW FIELD KEYS ON {db} FROM {measurement}",
        }
        response = requests.get(
            f"{URL}/query",
            headers=headers,
            params=params
        )

        # lol
        results = response.json()["results"][0]["series"][0]["values"]
        return [
            field_pair[0] for field_pair in results
        ]
