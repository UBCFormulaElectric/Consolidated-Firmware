from flask import Flask, jsonify, Response
from flask_socketio import SocketIO, emit
from process import SignalUtil  # Assuming SignalUtil is in a module named 'process'

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")
signal_util = SignalUtil.SignalUtil()  # Initialize your SignalUtil class

@app.route('/')
def hello_world():
    return "Wireless 2.0"

# for each function, there is a duplicate function for the HTTP/websocket version.
@socketio.on('connect')
def handle_connect():
    print("Client Connected")
    emit('message', 'You are connected to the server connect')

# function set for returning signal names
@app.route('/signal', methods=['GET'])
def return_all_available_signals():
    signals = signal_util.get_all_signals() 
    signal_names = list(signals.keys())  # returns list of keys 

    response = jsonify(signal_names)
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response

# function set for return signals 
@app.route('/signal/<string:name>', methods=['GET'])
def return_signal(name):
    signal_data = signal_util.get_signal(name).to_dict()
    response = jsonify(signal_data)
    response.headers.add("Access-Control-Allow-Origin", "*")
    return response

@socketio.on('signal')
def handle_signal_message(message):
    ids = message["ids"]
    signals = {}
    for id_ in ids:
        signal_data = signal_util.get_signal(id_).to_dict()
        signals[id_] = signal_data
    socketio.emit("signal_response", signals)


    
if __name__ == '__main__':
    # app.run(debug=True)  # This starts another server

    # Start the Socket.IO server
    socketio.run(app, debug=True)
