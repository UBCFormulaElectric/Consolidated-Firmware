"""
Main REST component of the backend
"""

from flask import Blueprint, request
import influx_handler
from typing import Tuple, Dict, List

# HTTP processes for data that is not live
app = Blueprint("http_app", __name__)


@app.route("/")
def hello_world() -> str:
    """
    :returns Hello world page for backend.
    """
    return "Telemetry Backend is running!"


@app.route("/health")
def health() -> Tuple[Dict, int]:
    """
    :returns Health check page for backend.
    """
    return {"status": "healthy"}, 200


@app.route("/data/measurements", methods=["GET"])
def return_all_measurements() -> Tuple[List[str], int]:
    """
    :returns Page displaying all measurements in the database.
    """
    return influx_handler.get_measurements(), 200


@app.route("/data/measurement/<string:measurement>/signals", methods=["GET"])
def return_signals_for_measurement(measurement: str) -> Tuple[List[str], int]:
    """
    :param measurement: Measurement to fetch fields for.
    :returns Page displaying all fields for a specific measurement.
    """
    return influx_handler.get_signals(measurement=measurement), 200


@app.route("/data/query", methods=["GET"])
def return_query() -> Dict[str, Dict]:
    """
    :returns Page displaying the result of a single query.
    """
    params = request.args
    measurement = params.get("measurement")
    signals: list[str] | None = params.get("signals")
    start_epoch = params.get("start_epoch")
    end_epoch = params.get("end_epoch")

    if (
        measurement is None
        or signals is None
        or start_epoch is None
        or end_epoch is None
    ):
        missing_keys = [
            k
            for k, v in [
                ("measurement", measurement),
                ("signals", signals),
                ("start_epoch", start_epoch),
                ("end_epoch", end_epoch),
            ]
            if v is None
        ]
        return {"error": f"Missing parameters: {missing_keys}"}, 400
    try:
        signals = signals.split(",")
        return influx_handler.query(
            measurement=measurement,
            signals=signals,
            time_range=(start_epoch, end_epoch),
        )
    except Exception as e:
        return {"error": str(e)}, 500
