from flask import Blueprint
from candb import can_db

api = Blueprint('api', __name__)

@api.route("/health")
def hello_world():
    return "<p>Hello, World!</p>"

@api.route("/signal")
def get_signal_metadata():
    return [{
        "name": signal.name,
        "min_val": signal.min_val,
        "max_val": signal.max_val,
        "unit": signal.unit,
        "enum": signal.enum,
        "tx_node": msg.tx_node,
        "cycle_time_ms": msg.cycle_time
    } for msg in can_db.msgs.values() for signal in msg.signals]