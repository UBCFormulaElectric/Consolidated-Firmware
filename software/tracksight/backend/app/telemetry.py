"""
Entrypoint to the telemetry backend
"""

import logging
import os
import threading
from datetime import datetime
from argparse import ArgumentParser
from flask import Flask
from flask_cors import CORS
import influx_handler
import pandas as pd

import signal_util
from flask_apps.database_app import app as database_app
from flask_apps.http_app import app as http_app
from flask_apps.socket_app import socketio


# Setup logging.
time_now = datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
logging.basicConfig(filename=f"logs/telemetry_{time_now}.log", level=logging.DEBUG)
if not os.path.exists("logs"):
    os.makedirs("logs")

logger = logging.getLogger("telemetry_logger")


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
        "--data-file",
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
    elif args.mode != "wireless" and args.mode != "local":
        raise RuntimeError("Mode must be either 'wireless' or 'local'")

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

            # Initialize the Socket.IO app with the main app.
            socketio.init_app(app)
            socketio.run(app, debug=True, allow_unsafe_werkzeug=True, host="0.0.0.0")
        except KeyboardInterrupt:
            logger.info("Exiting")

            if wireless_thread is not None:
                wireless_thread.join()

            logger.info("Thread stopped")

    elif args.mode == "local":
        try:
            if args.data_file is not None and args.data_file.strip() != "":
                # Read log files and upload to the influx database.
                for file in args.data_file.split(","):
                    file = file.strip()
                    logger.info(f"Uploading data file: {file}")

                    path = os.path.join("data", file)
                    df = pd.read_csv(path)
                    influx_handler.write(df=df, measurement=file)

            # Initialize the Socket.IO app with the main app.
            logger.info("Starting Flask app.")
            socketio.init_app(app)
            socketio.run(app, debug=True, allow_unsafe_werkzeug=True, host="0.0.0.0")

        except KeyboardInterrupt:
            logger.info("Exiting")
