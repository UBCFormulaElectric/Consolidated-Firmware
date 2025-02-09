"""
Entrypoint to the telemetry backend
"""
from dotenv import load_dotenv
from influx_handler import InfluxHandler

# VERIFIED USEFUL
import logging
import os
import threading
from argparse import ArgumentParser
from initilize_app import app, sio
from write_task.log import run_logging_task
from write_task.mock import run_mock_mode_task
from write_task.wireless import run_wireless_mode_task
from api import run_broadcast_thread
from logger import logger, log_path

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

    # STOP EVENT FOR Synchronization
    stop_event = threading.Event()

    # Setup the Message Populate Thread
    if args.mode == "wireless":
        write_thread = run_wireless_mode_task()
    elif args.mode == "mock":
        write_thread = run_mock_mode_task(args.data_file)
    elif args.mode == "log":
        write_thread = run_logging_task()
    else:
        raise RuntimeError("should be caught by parser")

    # Reading Thread
    read_thread = run_broadcast_thread(stop_event)

    try:
        # Initialize the Socket.IO app with the main app.
        sio.run(app, debug = False)
    except KeyboardInterrupt:
        logger.info("Exiting")
        stop_event.set()
        write_thread.join()
        read_thread.join()
        logger.info("Thread stopped")
