from flask import Blueprint, Response, stream_with_context
from influxdb_client.rest import ApiException
from influxdb import get_influxdb_client, get_timed_query
from settings import INFLUX_BUCKET

influx_handler = Blueprint("influx_handler", __name__)

@influx_handler.route("/influx/csv/<start_time>/<end_time>/", methods=["GET"])
def get_csv_file(start_time: str, end_time: str):
    """
    Given start and end time, returns csv file of all data in time frame from `INFLUX_BUCKET`
    """
    separator = ','
    query = get_timed_query(
        bucket=INFLUX_BUCKET,
        start_time=start_time,
        end_time=end_time,
    )

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