from flask_socketio import SocketIO, emit
from flask import request
from api.subtable_handler import SUB_TABLE
from flask_app import app
from logger import logger

sio = SocketIO(app, cors_allowed_origins="*", logger=True)

@sio.on("test")
def test(payload):
    emit("ack", "test")

@sio.on('connect')
def connect():
    logger.info(f'{request.sid} connected!')
    SUB_TABLE[request.sid] = set()

@sio.on('disconnect')
def disconnect():
    del SUB_TABLE[request.sid]
    logger.info(f'{request.sid} diconnected')

