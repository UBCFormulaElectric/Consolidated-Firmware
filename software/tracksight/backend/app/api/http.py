# api blueprints
import re
from flask import Blueprint, jsonify, request
from middleware.candb import live_can_db
from influxdb import BadTimeFormat, get_influxdb_client, get_timed_query
from settings import INFLUX_BUCKET
from influxdb_client.rest import ApiException
http = Blueprint("util", __name__)

@http.route("/")
def index():
    return "<p>Welcome to the UBC Formula Electric Telemetry Backend</p>"

@http.route("/health", methods=["GET"])
def hello_world():
    return "<p>Hello, World!</p>"

@http.route("/signal/nodes", methods=["GET"])
def get_nodes():
    """
    Gets the list of all nodes (str) in the current parser.
    """
    return jsonify(list(live_can_db.nodes.keys())), 200

@http.route("/signal/metadata", methods=["GET"])
def get_signal_metadata():
    """
    Gets metadata of the signal of the current parser.
    Pass signal names (regex) in `name` parameter.
    E.g. `/signal?name=INVFR_bError,*_bReserve`
    """
    name_arg = request.args.get("name")

    def condition(signal_name):
        return not name_arg or re.search(name_arg, signal_name)

    return jsonify([
        {
            "name": signal.name,
            "min_val": signal.min_val,
            "max_val": signal.max_val,
            "unit": signal.unit,
            "enum": signal.enum,
            "tx_node": msg.tx_node_name,
            "cycle_time_ms": msg.cycle_time,
            "id": msg.id,
            "msg_name": msg.name,
        }
        for msg in live_can_db.msgs.values()
        for signal in msg.signals
        if condition(signal.name)
    ]), 200

@http.route("/signal", methods=["GET"])
def get_signal():
    """
    Gets the signals of the current parser. 
    
    By default, 5 minute interval of data is returned unless both `start` and `end` parameters are provided.

    Optional parameters:
    `name` - Pass signal name (regex).
    `start` - Start time for query (RFC3339 format)
    `end` - End time for query (RFC3339 format)

    E.g. `/signal?name=INVFR_bError&start=2025-11-01&end=2025-11-01T01:00:00`
    - Matches exactly `INVFR_bError` between Nov 1, 2025 00:00 to Nov 1, 2025 01:00
    """
    # parsing args
    name_arg = request.args.get("name")
    start = request.args.get("start")
    end = request.args.get("end")

    query: str
    try:
        query = get_timed_query(
            bucket=INFLUX_BUCKET,
            start=start,
            end=end
        )
    except BadTimeFormat as btf:
        return f"bad time: {btf}", 400

    if name_arg:
        # i love arbitrary query execution!!!!
        query += f'|> filter(fn: (r) => r._field =~ /^{name_arg}$/)'

    print(query)

    results = None
    with get_influxdb_client() as client:
        try:
            results = client.query_api().query(query=query)
        except ApiException as ae:
            return f"bad query: {ae}", 400

    measurement_names = []
    for table in results:
        for record in table.records:
            values = record.values
            values['_time'] = str(values['_time'])
            measurement_names.append(values)

    return jsonify(measurement_names), 200

# @http.route("/signal/<signal_name>", methods=["GET"])
# def get_cached_signals(signal_name: str):
#     """
#     Gets the signal name data (from the last 5 minutes) from the influx DB
#     """
#     msg = live_can_db.signals_to_msgs.get(signal_name)
#     if msg is None:
#         return jsonify({"error": "Signal not found"}), 404
#     matching_signals = [
#         signal for signal in msg.signals if signal.name == signal_name
#     ]
#     if len(matching_signals) == 0:
#         return jsonify({"error": "Signal not found"}), 404
#     if len(matching_signals) > 1:
#         return jsonify({"error": "Multiple signals with the same name found"}), 500
#     signal = matching_signals[0]
#     return jsonify({
#         "name": signal.name,
#         "description": signal.description,
#         "tx_node": msg.tx_node_name,
#         "cycle_time_ms": msg.cycle_time,
#         # TODO figure out what we need from the individual signal metadata
#     }), 200