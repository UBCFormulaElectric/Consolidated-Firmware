from flask import request
from flask_app import app
from flask_socketio import SocketIO
from logger import logger
from subtable import SUB_TABLE

sio = SocketIO(app, cors_allowed_origins="*")


@sio.on('connect')
def connect():
    logger.info(f'{request.sid} connected!')
    SUB_TABLE[request.sid] = set()


@sio.on('disconnect')
def disconnect():
    del SUB_TABLE[request.sid]
    logger.info(f'{request.sid} diconnected')


@sio.on("sub")
def subscribe(signal):
    # check if signal is a valid signal
    valid_signal = True
    if valid_signal:
        SUB_TABLE[request.sid].add(signal)
        logger.info(f"{request.sid} subscribed to {signal}")
        sio.emit("sub_ack", {"signal": signal,
                 "status": "subscribed"}, to=request.sid)
        return
    else:
        logger.error(f"{request.sid} failed to subscribe to {signal}")
        sio.emit("sub_ack", {"signal": signal,
                 "status": "failed"}, to=request.sid)
        return


@sio.on('unsub')
def unsubscribe(signal):
    if signal not in SUB_TABLE[request.sid]:
        sio.emit("unsub_err", signal, to=request.sid)
        logger.warning(f"{request.sid} failed to unsubscribe to {signal}")
        return
    SUB_TABLE[request.sid].remove(signal)
    logger.info(f"{signal} removed from {request.sid}")
