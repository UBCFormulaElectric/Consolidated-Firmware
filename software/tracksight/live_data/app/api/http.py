from flask import Blueprint
from candb import can_db
from api.historical_handler import historical_api
from api.files_handler import sd_api

api = Blueprint('api', __name__)
api.register_blueprint(historical_api)
api.register_blueprint(sd_api)

@api.route("/health", methods=["GET"])
def hello_world():
    return "<p>Hello, World!</p>"

# Viewing Live Data
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
    } for msg in can_db.msgs.values() for signal in msg.signals]
