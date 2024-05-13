"""
Socket app for handling live data
"""

import flask_socketio

from .. import signal_util

# SocketIO processes for live data
socketio = flask_socketio.SocketIO(cors_allowed_origins="*")
signal_util = signal_util.SignalUtil()


@socketio.on("connect")
def handle_connect():
    """
    Handles the socket connection when the client connects
    """
    print("Client Connected")
    flask_socketio.emit("message", "You are connected to the server connect")


@socketio.on("disconnect")
def handle_disconnect():
    """
    Handles the socket connection when the client disconnects
    """
    print("Client Disconnected")
    flask_socketio.emit("message", "You are disconnected from the server disconnect")


@socketio.on("data")
def handle_message(message):
    """
    Handles the socket connection when the client sends a message
    :param message: Message that the client sent
    :return: None
    """
    socketio.emit("message_from_server", f"Server received: {message}")


@socketio.on("available_signals")
def handle_available_signals(_message):
    """
    Handles the "available signals" request from the client
    :returns all available signals to the client
    """
    signals = signal_util.get_all_signals()
    signal_names = list(signals.keys())  # returns list of keys
    socketio.emit(
        "available_signals_response", signal_names
    )  # Emit the signal names to the client


@socketio.on("signal")
def handle_signal_message(message):
    """
    Handles the "signal" request from the client
    :param message:
    :returns signal data to client based on signal name
    """
    signals = {}
    for id_ in message["ids"]:
        signal_data = signal_util.get_signal(id_).to_dict()
        signals[id_] = signal_data
    ret = {"id": message["graph"], "signals": signals}
    flask_socketio.emit("signal_response", ret)


if __name__ == "__main__":
    socketio.run(app=None, debug=True)
