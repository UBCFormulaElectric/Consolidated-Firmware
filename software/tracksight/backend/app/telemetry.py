from flask import Flask
from process.http_app import app as http_app
from process.socket_app import socketio as socket_app


app = Flask(__name__)

app.register_blueprint(http_app)

if __name__ == '__main__':
    socket_app.init_app(app)  # Initialize the Socket.IO app with the main app
    # Different port to support apple fellas
    socket_app.run(app, debug=True, allow_unsafe_werkzeug=True, port=4999)
