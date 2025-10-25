import re
from flask import Blueprint, Response, stream_with_context
from influxdb_client.rest import ApiException
from influxdb import get_influxdb_client
from settings import INFLUX_BUCKET

influx_handler = Blueprint("influx_handler", __name__)

@influx_handler.route("/influx/csv/<start_time>/<end_time>/", methods=["GET"])
def get_csv_file(start_time: str, end_time: str):
    """
    Given start and end time, returns csv file of all data in time frame from `INFLUX_BUCKET`
    """
    # verify date format to prevent arbitrary queries
    # something like <YYYY>-<MM>-<DD>T<HH>:<MM>:<SS>Z
    # thanks chatgpt
    RFC3339_REGEX = re.compile(
        r"^\d{4}-\d{2}-\d{2}"
        r"(T\d{2}:\d{2}:\d{2}(?:\.\d+)?(?:Z|[+-]\d{2}:\d{2})?)?$"
    )
    if (not RFC3339_REGEX.match(start_time)) or (not RFC3339_REGEX.match(end_time)):
        return "bad date format", 400

    separator = ','
    query = f'from(bucket:"{INFLUX_BUCKET}")\
        |> range(start: {start_time}, stop: {end_time})\
        |> drop(columns: ["_start", "_stop"])'

    csv_results = None
    with get_influxdb_client() as client:
        try:
            csv_results = client.query_api().query_csv(query=query)
        except ApiException:
            return "bad date query", 400
    csv_results = list(map(lambda x: separator.join(x) + '\n', csv_results))

    return Response(
        stream_with_context(iter(csv_results)),
        mimetype="text/csv",
        headers={"Content-Disposition": f"attachment; filename={start_time}_{end_time}.csv"}
    )