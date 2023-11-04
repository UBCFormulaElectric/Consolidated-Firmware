from flask_socketio import SocketIO, emit
from process import SignalUtil


# SocketIO processes for live data
socketio = SocketIO(cors_allowed_origins="*")
signal_util = SignalUtil.SignalUtil()

@socketio.on('connect')
def handle_connect():
    print("Client Connected")
    emit('message', 'You are connected to the server connect')

# returns all available signals to the client
@socketio.on('available_signals')
def handle_available_signals(message):
    signals = signal_util.get_all_signals() 
    signal_names = list(signals.keys())  # returns list of keys 
    socketio.emit('available_signals_response', signal_names)  # Emit the signal names to the client

# returns signal data to client based on signal name
@socketio.on('signal')
def handle_signal_message(message):
    ids = message["ids"]
    signals = {}
    for id_ in ids:
        signal_data = signal_util.get_signal(id_).to_dict()
        signals[id_] = signal_data
    emit("signal_response", signals)

if __name__ == '__main__':
    socketio.run(debug=True)
