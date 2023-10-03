from flask import Flask, jsonify, Response
from flask_socketio import SocketIO, emit
from process import SignalUtil  # Assuming SignalUtil is in a module named 'process'

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")
signal_util = SignalUtil.SignalUtil()  # Initialize your SignalUtil class

@app.route('/')
def hello_world():
    return "Wireless 2.0"

# test route to get all signals from the SignalUtil class
@app.route('/get_mock_data')
def get_mock_data():
    return signal_util.get_all_signals().to_json()  # This will return a JSON string of the data

@socketio.on('connect')
def handle_connect():
    print("Client Connected")
    emit('message', 'You are connected to the server connect')

@socketio.on('data')
def handle_message(message):
    socketio.emit('message_from_server', f'Server received: {message}')

@socketio.on('signals')
def handle_signal_message(message):
    ids = message["ids"]  # Extract the IDs from the incoming message
    signals = signal_util.get_all_signals()  # Get the signals using your SignalUtil class
    socketio.emit("signal_response", signals)  # Emit the JSON data to the client

if __name__ == '__main__':
    # Note: You should only call one of these, as they both start the server
    # app.run(debug=True)  # This starts another server, which you probably don't want

    # Start the Socket.IO server
    socketio.run(app, debug=True)
