"""
Entrypoint to the telemetry backend
"""
from dotenv import load_dotenv
from influx_handler import InfluxHandler

# VERIFIED USEFUL
import logging
import os
from argparse import ArgumentParser
from flask_app import app
from api import api, sio
from write_task.log import get_logging_task
from write_task.mock import get_mock_task
from write_task.wireless import get_wireless_task
from logger import logger, log_path
from broadcaster import get_websocket_broadcast

def setupInflux():
    dockerized = os.environ.get("IN_DOCKER_CONTAINER") is not None
    if not dockerized:
        influx_env_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "influx.env")
        load_dotenv(dotenv_path=influx_env_path)
    #Configs for Influx DB instance.
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

if __name__ == "__main__":
    app.register_blueprint(api, url_prefix='/api')
    # ARGPARSER
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

    # Set the logging level to DEBUG
    logger.setLevel(level=logging.DEBUG if args.debug else logging.INFO)
    if args.debug:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(filename=log_path, level=logging.INFO)

    # INFLUX DB
    # setupInflux()

    # Setup the Message Populate Thread
    if args.mode == "wireless":
        write_thread = get_wireless_task()
    elif args.mode == "mock":
        write_thread = get_mock_task(args.data_file)
    elif args.mode == "log":
        write_thread = get_logging_task()
    else:
        raise RuntimeError("should be caught by parser")
    # Reading Thread
    read_thread = get_websocket_broadcast()

    # Initialize the Socket.IO app with the main app.
    write_thread.start()
    read_thread.start()
    sio.run(app, debug = bool(args.debug), port=5000)
    # on keyboard interrupt, the above handles killing
