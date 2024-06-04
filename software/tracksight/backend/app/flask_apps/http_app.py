"""
Main REST component of the backend
"""

from flask import Blueprint, request
from typing import Tuple, Dict, List
from datetime import datetime, timedelta
import logging

from influx_handler import InfluxHandler

logger = logging.getLogger("telemetry_logger")

# HTTP processes for data that is not live
app = Blueprint("http_app", __name__)


def submit_query(
    measurement: str, signals: List[str], start_epoch: int, end_epoch: int
) -> Dict[str, Dict]:
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
        return InfluxHandler.query(
            measurement=measurement,
            signals=signals,
            time_range=(start_epoch, end_epoch),
        )
    except Exception as e:
        return {"error": str(e)}, 500


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
    return InfluxHandler.get_measurements(), 200


@app.route("/data/measurement/<string:measurement>/signals", methods=["GET"])
def return_signals_for_measurement(measurement: str) -> Tuple[List[str], int]:
    """
    :param measurement: Measurement to fetch fields for.
    :returns Page displaying all fields for a specific measurement.
    """
    return InfluxHandler.get_signals(measurement=measurement), 200


@app.route("/data/measurement/wtf", methods=["GET"])
def return_live_signals() -> Tuple[List[str], int]:
    """
    :param measurement: Measurement to fetch fields for.
    :returns Page displaying all fields for a specific measurement.
    """
    return InfluxHandler.get_signals(measurement="live"), 200


@app.route("/data/query/live", methods=["GET"])
def return_live_query() -> Dict[str, Dict]:
    """
    :returns Page displaying the result of a single query.
    """
    params = request.args
    measurement = "live"
    signals: list[str] | None = params.get("signals")

    start_time = datetime.now() - timedelta(hours=1)
    end_time = datetime.now()

    start_epoch = int(start_time.timestamp())
    end_epoch = int(end_time.timestamp())

    logger.info(f"Start request: {start_epoch}, {end_epoch}")

    return submit_query(
        measurement=measurement,
        signals=signals,
        start_epoch=start_epoch,
        end_epoch=end_epoch,
    )


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

    return submit_query(
        measurement=measurement,
        signals=signals,
        start_epoch=start_epoch,
        end_epoch=end_epoch,
    )
