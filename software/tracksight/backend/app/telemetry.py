from flask import Flask, request
from process.http_app import app as http_app
from process.socket_app import socketio as socket_app

from influx_handler import InfluxHandler as influx


app = Flask(__name__)

app.register_blueprint(http_app)

if __name__ == '__main__':
    socket_app.init_app(app)  # Initialize the Socket.IO app with the main app
    socket_app.run(app, debug=True)
