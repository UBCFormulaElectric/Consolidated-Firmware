from flask import Flask
from flask_socketio import SocketIO, emit

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

@app.route('/')
def hello_world():
    return "Wireless 2.0"

@socketio.on('connect')
def handle_connect():
    print("Client Connected")
    emit('message', 'You are connected to the server connect')

@socketio.on('data')
def handle_message(message):
    # Do something with the message, e.g., broadcast it to other clients
    print("hi")
    socketio.emit('message_from_server', f'Server received: {message}')


if __name__ == '__main__':
    # Start the Flask app (you can configure host and port)
    app.run(debug=True)

    # Start the Socket.IO server
    socketio.run(app, debug=True)

