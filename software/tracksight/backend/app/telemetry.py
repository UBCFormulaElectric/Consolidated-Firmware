"""
Entrypoint to the telemetry backend
"""

import logging
import os
import threading
import time
from argparse import ArgumentParser
from flask import Flask
from flask_cors import CORS

import signal_util
from flask_apps.database_app import app as database_app
from flask_apps.http_app import app as http_app
from flask_apps.socket_app import socketio

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument(
        "--mode",
        "-m",
        type=str,
        required=True,
        help="Mode to run telemetry in, either 'wireless' or 'local'.",
    )
    parser.add_argument(
        "--serial-port",
        "-p",
        type=str,
        required=False,
        default=None,
        help="Serial port to receive data wirelessly from.",
    )
    parser.add_argument(
        "--log-file",
        "-f",
        type=str,
        required=False,
        default=None,
        help="Path to the log file to read from.",
    )
    args = parser.parse_args()

    if args.mode == "wireless" and args.serial_port is None:
        raise RuntimeError(
            "If running telemetry in wireless mode, you must specify the radio serial port!"
        )
    elif args.mode == "local" and args.log_file is None:
        raise RuntimeError(
            "If running telemetry in log file mode, you must specify a path to the logging file!"
        )
    elif args.mode != "wireless" and args.mode != "local":
        raise RuntimeError("Mode must be either 'wireless' or 'local'")

    # Setup logging.
    logger = logging.getLogger("telemetry_logger")
    if not os.path.exists("logs"):
        os.makedirs("logs")

    logging.basicConfig(
        filename=f"logs/telemetry.{time.time()}.log", level=logging.INFO
    )

    # Setup Flask app.
    app = Flask(__name__)
    app.register_blueprint(http_app)
    app.register_blueprint(database_app)
    CORS(app)

    if args.mode == "wireless":
        wireless_thread = threading.Thread(
            target=signal_util.read_messages,
            daemon=True,
        )

        try:
            wireless_thread.start()
            socketio.init_app(app)  # Initialize the Socket.IO app with the main app
            socketio.run(app, debug=True, allow_unsafe_werkzeug=True, host="0.0.0.0")
        except KeyboardInterrupt:
            logger.log("Exiting")

            if wireless_thread is not None:
                wireless_thread.join()

            logger.log("Thread stopped")

    elif args.mode == "local":
        try:
            socketio.init_app(app)  # Initialize the Socket.IO app with the main app
            socketio.run(app, debug=True, allow_unsafe_werkzeug=True, host="0.0.0.0")
        except KeyboardInterrupt:
            logger.log("Exiting")
