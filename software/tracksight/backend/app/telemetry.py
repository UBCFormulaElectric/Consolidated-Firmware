"""
Entrypoint to the telemetry backend
"""

from flask import Flask

from process.flask_apps.database_app import app as database_app
from process.flask_apps.http_app import app as http_app
from process.flask_apps.socket_app import socket_app

# from flask_cors import CORS

app = Flask(__name__)
app.register_blueprint(http_app)
app.register_blueprint(database_app)
# CORS(app)

socket_app.init_app(app)  # Initialize the Socket.IO app with the main app
socket_app.run(app, debug=True, allow_unsafe_werkzeug=True, host="0.0.0.0")
