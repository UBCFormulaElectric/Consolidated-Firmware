"""
Socket app for handling live data
"""

import logging

import flask_socketio
from flask import request

logger = logging.getLogger("telemetry_logger")

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
    # TODO SignalUtil.disconnectClient(request.sid)
    logger.info(f"Client {request.sid} Disconnected")
    flask_socketio.emit("message", "Successfully Disconnected from the server")


@socketio.on("available_signals_sub")
def handle_available_signals():
    """
    Handles the "available signals" request from the client
    :returns all available signals to the client
    """
    # TODO SignalUtil.connectClientToAvailableSignals(client)
    logger.info(f"Client {request.sid} requested available signals")


@socketio.on("signal_sub")
def handle_signal_subscription(message):
    """
    Handles the "signal" request from the client
    :param message:
    :returns signal data to client based on signal name
    """
    # TODO SignalUtil.connectClientToSignal(request.sid, message.signal_name)
    logger.info(f"Client {request.sid} requested signal {message.signal_name}")


@socketio.on("signal_unsub")
def handle_signal_unsubscrption(message):
    # TODO SignalUtil.disconnectClientFromSignal(request.sid, message.signal_name)
    logger.info(f"Client {request.sid} unsubscribed from signal {message.signal_name}")


if __name__ == "__main__":
    socketio.run(app=None, debug=True)
