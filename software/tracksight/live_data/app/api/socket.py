from flask import request
from flask_app import app
from flask_socketio import SocketIO
from logger import logger
from api.subtable_handler import SUB_TABLE

sio = SocketIO(app, cors_allowed_origins="*")

@sio.on('connect')
def connect():
    logger.info(f'{request.sid} connected!')
    SUB_TABLE[request.sid] = set()


@sio.on('disconnect')
def disconnect():
    del SUB_TABLE[request.sid]
    logger.info(f'{request.sid} diconnected')
