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
import pandas as pd
from dotenv import load_dotenv

from signal_util import SignalUtil
from influx_handler import InfluxHandler
from flask_apps.database_app import app as database_app
from flask_apps.http_app import app as http_app

# Setup logging.
time_now = datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
app_dir = os.path.dirname(os.path.abspath(__file__))
log_path = os.path.join(app_dir, "..", "logs", f"telemetry_{time_now}.log")

if not os.path.exists(os.path.dirname(log_path)):
    os.makedirs(os.path.dirname(log_path))

logger = logging.getLogger("telemetry_logger")

dockerized = os.environ.get("IN_DOCKER_CONTAINER") is not None
if not dockerized:
    influx_env_path = os.path.join(app_dir, "..", "influx.env")
    load_dotenv(dotenv_path=influx_env_path)

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument(
        "--mode",
        "-m",
        type=str,
        required=True,
        help="Mode to run telemetry in, either 'wireless' or 'log'.",
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
        default=None,
        help="Path to the log file to read from.",
    )
    parser.add_argument(
        "--debug",
        "-d",
        action="store_true",
        default=False,
        help="Pass to run in debug mode (logs to console).",
    )
    args = parser.parse_args()

    if args.debug:
        logging.basicConfig(level=logging.INFO)
    else:
        logging.basicConfig(filename=log_path, level=logging.INFO)

    if args.mode == "wireless" and args.serial_port is None:
        raise RuntimeError(
            "If running telemetry in wireless mode, you must specify the radio serial port!"
        )
    elif args.mode != "wireless" and args.mode != "log":
        raise RuntimeError("Mode must be either 'wireless' or 'log'")

    # Configs for Influx DB instance.
    required_env_vars = {
        "org": "DOCKER_INFLUXDB_INIT_ORG",
        "bucket": "DOCKER_INFLUXDB_INIT_BUCKET",
        "token": "DOCKER_INFLUXDB_INIT_ADMIN_TOKEN",
    }
    for env_var in required_env_vars.values():
        if os.environ.get(env_var) is None:
            raise RuntimeError(f"Required environment variable not set: {env_var}")

    # Setup InfluxDB database.
    influx_url = f"http://{'influx' if dockerized else 'localhost'}:8086"
    influx_org = os.environ.get(required_env_vars["org"])
    influx_bucket = os.environ.get(required_env_vars["bucket"])
    influx_token = os.environ.get(required_env_vars["token"])

    InfluxHandler.setup(
        url=influx_url, org=influx_org, bucket=influx_bucket, token=influx_token
    )

    # Setup Flask app.
    app = Flask(__name__)
    app.register_blueprint(http_app)
    app.register_blueprint(database_app)
    CORS(app)

    if args.mode == "wireless":
        SignalUtil.setup(port=args.serial_port, app=app)

        wireless_thread = threading.Thread(
            target=SignalUtil.read_messages,
            daemon=True,
        )

        try:
            wireless_thread.start()

            # Initialize the Socket.IO app with the main app.
            app.run(debug=False, host="0.0.0.0")
        except KeyboardInterrupt:
            logger.info("Exiting")

            if wireless_thread is not None:
                wireless_thread.join()

            logger.info("Thread stopped")

    elif args.mode == "log":
        try:
            if args.data_file is not None and args.data_file.strip() != "":
                # Read log files and upload to the influx database.
                for file in args.data_file.split(","):
                    file = file.strip()
                    logger.info(f"Uploading data file: {file}")

                    path = os.path.join("data", file)
                    df = pd.read_csv(path)
                    InfluxHandler.write(df=df, measurement=file)

            # Initialize the Socket.IO app with the main app.
            logger.info("Starting Flask app.")
            app.run(debug=False, host="0.0.0.0")

        except KeyboardInterrupt:
            logger.info("Exiting")
