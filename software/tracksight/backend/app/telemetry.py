"""
Entrypoint to the telemetry backend
"""

import threading
from flask import Flask
from flask_cors import CORS
import logging
import time

from process.flask_apps.database_app import app as database_app
from process.flask_apps.http_app import app as http_app
from process.flask_apps.socket_app import socketio

app = Flask(__name__)
app.register_blueprint(http_app)
app.register_blueprint(database_app)
CORS(app)

logger = logging.getLogger("telemetry_logger")
logging.basicConfig(filename=f'telemetry.{time.time()}.log', level=logging.INFO)

def thread_function(a):
    logger.info(f"Thread {a} starting")


modem_thread = threading.Thread(
    target=thread_function, args=(1,), daemon=True
)  # TODO for Lara: Make this the function that is monitoring the UART
messages_thread = threading.Thread(
    target=thread_function, args=(2,), daemon=True
)  # TODO for Lara: Make this the function that is monitoring the UART
try:
    modem_thread.start()
    socketio.init_app(app)  # Initialize the Socket.IO app with the main app
    socketio.run(app, debug=True, allow_unsafe_werkzeug=True, host="0.0.0.0")
except KeyboardInterrupt:
    print("Exiting")
    if modem_thread is not None:
        modem_thread.join()
    print("Thread stopped")
