from flask import Flask
from flask_socketio import SocketIO, emit
from process import SignalUtil 

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")
signal_util = SignalUtil.SignalUtil()

@app.route('/')
def hello_world():
    return "Wireless 2.0"

@socketio.on('connect')
def handle_connect():
    print("Client Connected")
    emit('message', 'You are connected to the server connect')

@socketio.on('data')
def handle_message(message):
    socketio.emit('message_from_server', f'Server received: {message}')

@socketio.on('signals')
def handle_signal_message(message):
    res = message["ids"]
    result = signal_util.get_signals(res)
    socketio.emit("signal_response", result)

@socketio.on('available_signals')
def handle_available_signals(message):
    available_signals = signal_util.get_available_signals()

if __name__ == '__main__':
    # Start the Flask app (you can configure host and port)
    app.run(debug=True)

    # Start the Socket.IO server
    socketio.run(app, debug=True)

