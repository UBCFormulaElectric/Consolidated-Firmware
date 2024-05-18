"""
Entrypoint to the telemetry backend
"""

import logging
import threading
import time

from flask import Flask

from process.flask_apps.database_app import app as database_app
from process.flask_apps.http_app import app as http_app
from process.flask_apps.socket_app import socketio

# from flask_cors import CORS

app = Flask(__name__)
app.register_blueprint(http_app)
app.register_blueprint(database_app)
# CORS(app)

logger = logging.getLogger("telemetry_logger")
logging.basicConfig(filename=f"telemetry.{time.time()}.log", level=logging.INFO)

def compile_proto_files():
    # protoc --proto_path=software/tracksight/backend --python_out=software/tracksight/backend/app/process  software/tracksight/backend/telem2.proto
    proto_source = 'software/tracksight/backend/telem2.proto'
    proto_dest = 'software/tracksight/backend/app/process'
    command = [
        'protoc',
        f'--proto_path=software/tracksight/backend/',
        f'--python_out={proto_dest}',
        proto_source
    ]
    try:
        # Run the command and wait for it to complete
        subprocess.run(command, check=True)
        print("Proto files compiled successfully.")
    except subprocess.CalledProcessError as e:
        print("Failed to compile proto files:", e)


def thread_function(a):
    logger.info(f"Thread {a} starting")


modem_thread = threading.Thread(
    target=thread_function, args=(1,), daemon=True
)  # TODO for Lara: Make this the function that is monitoring the UART
messages_thread = threading.Thread(
    target=thread_function, args=(2,), daemon=True
)  # TODO for Lara: Make this the function that is monitoring the JSONCAN file
try:
    modem_thread.start()
    socketio.init_app(app)  # Initialize the Socket.IO app with the main app
    socketio.run(app, debug=True, allow_unsafe_werkzeug=True, host="0.0.0.0")
except KeyboardInterrupt:
    print("Exiting")
    if modem_thread is not None:
        modem_thread.join()
    print("Thread stopped")
