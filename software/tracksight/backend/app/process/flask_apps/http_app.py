"""
Main REST component of the backend
"""

from flask import Blueprint, jsonify, request

from ..influx_handler import (
    InfluxHandler as influx,
)
from ..influx_handler import (
    NoDataForQueryException,
)

# HTTP processes for data that is not live
app = Blueprint("http_app", __name__)

# helpers
from ..signal_util import SignalUtil

signal_util = SignalUtil()


def responsify(data):
    """
    Manual CORS?????? very bad idea
    :param data:
    :return:
    """
    response = jsonify(data)
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response


@app.route("/")
def hello_world():
    """

    :return:
    """
    return "Telemetry Backend is running!"


@app.route("/health")
def health():
    """

    :return:
    """
    return jsonify(status="healthy"), 200


@app.route("/signal/measurements", methods=["GET"])
def return_all_measurements():
    """

    :return:
    """
    measurements = influx.get_measurements()
    return responsify(measurements)


@app.route("/signal/measurement/<string:measurement>/fields", methods=["GET"])
def return_all_fields_for_measurement(measurement):
    """

    :param measurement:
    :return:
    """
    fields = influx.get_fields(measurement)
    return responsify(fields)


@app.route("/signal/query", methods=["GET"])
def return_query():
    """

    :return:
    """
    params = request.args
    measurement = params.get("measurement")
    fields = params.get("fields").split(",")
    start_epoch = params.get("start_epoch")
    end_epoch = params.get("end_epoch")

    try:
        data = influx.query(measurement, fields, [start_epoch, end_epoch])
    except NoDataForQueryException as e:
        return responsify({"error": str(e)}), 400
    return responsify(data)
