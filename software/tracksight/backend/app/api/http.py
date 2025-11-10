# api blueprints
from fnmatch import fnmatch
from flask import Blueprint, jsonify, reqeust
from middleware.candb import live_can_db
http = Blueprint("util", __name__)

@http.route("/")
def index():
    return "<p>Welcome to the UBC Formula Electric Telemetry Backend</p>"

@http.route("/health", methods=["GET"])
def hello_world():
    return "<p>Hello, World!</p>"

@http.route("/signal", methods=["GET"])
def get_signal_metadata():
    """
    Gets all the signals of the current parser.
    Pass signal names (glob) in `name` parameter separated by ','.
    Inverse signal name selection with `exclude` parameter.

    E.g. `/signal?name=INVFR_bError,*_bReserve&exclude`
    - Excludes INVFR_bError. and anything that ends in _bReserve
    """
    # parsing args
    exclude = request.args.get("exclude") is not None
    name_arg = request.args.get("name")
    names = [n.strip() for n in name_arg.split(',')] if name_arg is not None else None

    def condition(signal_name):
        con = not names or any(fnmatch(signal_name, n) for n in names)
        # xor
        return con != exclude

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

@http.route("/signal/<signal_name>", methods=["GET"])
def get_cached_signals(signal_name: str):
    """
    Gets the signal name data (from the last 5 minutes) from the influx DB
    """
    msg = live_can_db.signals_to_msgs.get(signal_name)
    if msg is None:
        return jsonify({"error": "Signal not found"}), 404
    matching_signals = [
        signal for signal in msg.signals if signal.name == signal_name
    ]
    if len(matching_signals) == 0:
        return jsonify({"error": "Signal not found"}), 404
    if len(matching_signals) > 1:
        return jsonify({"error": "Multiple signals with the same name found"}), 500
    signal = matching_signals[0]
    return jsonify({
        "name": signal.name,
        "description": signal.description,
        "tx_node": msg.tx_node_name,
        "cycle_time_ms": msg.cycle_time,
        # TODO figure out what we need from the individual signal metadata
    }), 200