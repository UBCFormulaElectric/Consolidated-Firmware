from flask import request, Blueprint
from flask_socketio import SocketIO
from logger import logger
from flask_app import app
from candb import can_db
from subtable import SUB_TABLE

api = Blueprint('api', __name__)
sio = SocketIO(app, cors_allowed_origins="*")

@sio.on('connect')
def connect():
    logger.info(f'{request.sid} connected!')
    SUB_TABLE[request.sid] = set()

@sio.on('disconnect')
def disconnect():
    del SUB_TABLE[request.sid]
    logger.info(f'{request.sid} diconnected')

@api.route("/health")
def hello_world():
    return "<p>Hello, World!</p>"

@sio.on("sub")
def subscribe(signal):
    # check if signal is a valid signal
    valid_signal = True
    if valid_signal:
        SUB_TABLE[request.sid].add(signal)
        logger.info(f"{request.sid} subscribed to {signal}")
        return
    else:
        logger.error(f"{request.sid} failed to subscribe to {signal}")
        return

@sio.on('unsub')
def unsubscribe(signal):
    if signal not in SUB_TABLE[request.sid]:
        sio.emit("unsub_err", signal, to=request.sid)
        logger.warning(f"{request.sid} failed to unsubscribe to {signal}")
        return
    SUB_TABLE[request.sid].remove(signal)
    logger.info(f"{signal} removed from {request.sid}")

@api.route("/signal")
def get_signal_metadata():
    msgs = can_db.msgs
    return [{
        "name": signal.name,
        "min_val": signal.min_val,
        "max_val": signal.max_val,
        "unit": signal.unit,
        "enum": signal.enum,
        "tx_node": msg.tx_node,
        "cycle_time_ms": msg.cycle_time
    } for msg in msgs.values() for signal in msg.signals]