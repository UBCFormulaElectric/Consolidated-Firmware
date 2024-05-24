"""
Socket app for handling live data
"""

import logging

import flask_socketio
from flask import request

logger = logging.getLogger("telemetry_logger")

from ..signal_util import SignalUtil

signal_util = SignalUtil()

# SocketIO processes for live data
socketio = flask_socketio.SocketIO(cors_allowed_origins="*")


@socketio.on("connect")
def handle_connect():
    """
    Handles the socket connection when the client connects
    """
    logger.info(f"Client {request.sid} Connected")
    flask_socketio.emit("message", "Successfully connected to the server")


@socketio.on("disconnect")
def handle_disconnect():
    """
    Handles the socket connection when the client disconnects
    """
    signal_util.disconnect_client(request.sid)
    logger.info(f"Client {request.sid} Disconnected")
    flask_socketio.emit("message", "Successfully Disconnected from the server")


@socketio.on("available_signals_sub")
def handle_available_signals():
    """
    Handles the "available signals" request from the client
    :returns all available signals to the client
    """
    client_id = request.sid 
    available_signals = signal_util.connect_client_to_available_signals(client_id)
    logger.info(f"Client {request.sid} requested available signals")
    socketio.emit("available_signals_sub", available_signals)


@socketio.on("signal_sub")
def handle_signal_subscription(message):
    """
    Handles the "signal" request from the client
    :param message:
    :returns signal data to client based on signal name
    """
    signal_util.connect_client_to_signal(request.sid, message.signal_name)
    logger.info(f"Client {request.sid} requested signal {message.signal_name}")
    


@socketio.on("signal_unsub")
def handle_signal_unsubscrption(message):
    """
    Handles the signal unsubscription
    """
    signal_util.disconnect_client_from_signal(request.sid, message.signal_name)
    logger.info(f"Client {request.sid} unsubscribed from signal {message.signal_name}")


if __name__ == "__main__":
    socketio.run(app=None, debug=True)
