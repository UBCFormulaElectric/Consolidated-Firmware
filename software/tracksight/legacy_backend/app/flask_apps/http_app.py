"""
Main REST component of the backend
"""

from flask import Blueprint, request
from typing import Tuple, Dict, List
from datetime import datetime, timedelta
import logging
from datetime import datetime

from influx_handler import InfluxHandler

logger = logging.getLogger("telemetry_logger")

# HTTP processes for data that is not live
app = Blueprint("http_app", __name__)


MAX_POINTS_HISTORIC = 1000
MAX_POINTS_LIVE = 1000


def submit_query(
    measurement: str,
    signals: List[str],
    start_epoch: int,
    end_epoch: int,
    max_points: int,
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
            max_points=max_points,
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


@app.route("/measurements", methods=["GET"])
def return_all_measurements() -> Tuple[List[str], int]:
    """
    :returns Page displaying all measurements in the database.
    """
    return InfluxHandler.get_measurements(), 200


@app.route("/signals/<string:measurement>", methods=["GET"])
def return_signals_for_measurement(measurement: str) -> Tuple[List[str], int]:
    """
    :param measurement: Measurement to fetch fields for.
    :returns Page displaying all fields for a specific measurement.
    """
    return InfluxHandler.get_signals(measurement=measurement), 200


@app.route("/query/live", methods=["GET"])
def return_live_query() -> Dict[str, Dict]:
    """
    :returns Page displaying the result of a single query.
    """
    params = request.args
    signals: list[str] | None = params.get("signals")

    start_time = datetime.now() - timedelta(hours=1)
    end_time = datetime.now()

    start_epoch = int(start_time.timestamp())
    end_epoch = int(end_time.timestamp())

    return submit_query(
        measurement="live",
        signals=signals,
        start_epoch=start_epoch,
        end_epoch=end_epoch,
        max_points=MAX_POINTS_HISTORIC,
    )


@app.route("/query", methods=["GET"])
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
        max_points=MAX_POINTS_LIVE,
    )
