"""
Main REST component of the backend
"""

from flask import Blueprint, jsonify, request

from .. import influx_handler as influx

# HTTP processes for data that is not live
app = Blueprint("http_app", __name__)


@app.route("/")
def hello_world():
    """
    :returns Hello world page for backend.
    """
    return "Telemetry Backend is running!"


@app.route("/health")
def health():
    """
    :returns Health check page for backend.
    """
    return jsonify(status="healthy"), 200


@app.route("/signal/measurements", methods=["GET"])
def return_all_measurements():
    """
    :returns Page displaying all measurements in the database.
    """
    measurements = influx.get_measurements()
    return jsonify(measurements), 200


@app.route("/signal/measurement/<string:measurement>/fields", methods=["GET"])
def return_all_fields_for_measurement(measurement: str):
    """
    :param measurement: Measurement to fetch fields for.
    :returns Page displaying all fields for a specific measurement.
    """
    fields = influx.get_fields(measurement)
    return jsonify(fields), 200


@app.route("/signal/query", methods=["GET"])
def return_query():
    """
    :returns Page displaying the result of a single query.
    """
    params = request.args
    measurement = params.get("measurement")
    fields: list[str] | None = params.get("fields").split(",")
    start_epoch = params.get("start_epoch")
    end_epoch = params.get("end_epoch")
    if (
        measurement is None
        or fields is None
        or start_epoch is None
        or end_epoch is None
    ):
        return {"error": "Missing parameters."}
    try:
        return influx.query(measurement, fields, (int(start_epoch), int(end_epoch)))
    except Exception as e:
        return {"error": str(e)}, 500
