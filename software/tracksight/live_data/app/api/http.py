from flask import Blueprint, Response, request
import influxdb_client

# ours
from env import INFLUX_ORG, INFLUX_BUCKET, INFLUX_TOKEN, INFLUX_URL, CAR_NAME
from candb import live_can_db
from logger import logger

# api blueprints
from api.historical_handler import historical_api
from api.files_handler import sd_api

api = Blueprint('api', __name__)
api.register_blueprint(historical_api)
api.register_blueprint(sd_api)

@api.route("/health", methods=["GET"])
def hello_world():
    return "<p>Hello, World!</p>"

# Viewing Live Data
# this technically shouldn't be inline but who cares

@api.route("/signal", methods=["GET"])
def get_signal_metadata():
    """
    Gets all the signals of the current parser
    """
    return [{
        "name": signal.name,
        "min_val": signal.min_val,
        "max_val": signal.max_val,
        "unit": signal.unit,
        "enum": signal.enum,
        "tx_node": msg.tx_node,
        "cycle_time_ms": msg.cycle_time
    } for msg in live_can_db.msgs.values() for signal in msg.signals]

@api.route("/signal/<signal_name>", methods=["GET"])
def get_cached_signals(signal_name: str):
    """
    Gets the signal name data (from the last 5 minutes) from the influx DB
    """
    # query for the signal on the CURRENT LIVE CAR_live for all data points in the last prev_time
    with influxdb_client.InfluxDBClient(
        url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG, debug = False
    ) as client:
        query = f"""from(bucket:"{INFLUX_BUCKET}")
            |> range(start: -1m)
            |> filter(fn: (r) => r._measurement == "{CAR_NAME}_live" and r._field == "{signal_name}")
            |> tail(n: {6000})"""
        logger.info(query)
        table = client.query_api().query(query)
    signals = table.to_json(columns=["_time", "_value"], indent=1)
    return Response(signals, status=200, mimetype='application/json')
