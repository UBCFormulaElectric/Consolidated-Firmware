from flask_socketio import SocketIO, emit
from flask import request
from flask_app import app
from logger import logger
from middleware.subtable import add_sid, remove_sid

sio = SocketIO(app, cors_allowed_origins="*")

@sio.on("test")
def test(payload):
    emit("ack", "test")

@sio.on('connect')
def connect():
    logger.info(f'{request.sid} connected!')
    add_sid(request.sid)

@sio.on('disconnect')
def disconnect():
    remove_sid(request.sid)
    logger.info(f'{request.sid} diconnected')

