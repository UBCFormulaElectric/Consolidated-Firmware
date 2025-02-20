"""
Entrypoint to the telemetry backend
"""

# Note this must be done first as there are static level os.env gets
import os
from threading import Thread
from dotenv import load_dotenv
dockerized = os.environ.get("DOCKERIZED") == "1"
if not dockerized:
    # this is only on developer machines
    influx_env_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "docker", "live_data.env")
    load_dotenv(dotenv_path=influx_env_path)

from argparse import ArgumentParser
import logging
from logger import logger, log_path

# apis
from flask_app import app
from api.http import api
from api.socket import sio

# tasks
import tasks.influx_logger as InfluxHandler
from tasks.read_task.log import get_log_read_task
from tasks.read_task.mock import get_mock_task
from tasks.read_task.wireless import get_wireless_task
from tasks.broadcaster import get_websocket_broadcast

if __name__ == "__main__": 
    # register blueprint for python
    app.register_blueprint(api, url_prefix='/api')

    # ARGPARSER
    parser = ArgumentParser()
    parser.add_argument(
        "--mode",
        "-m",
        type=str,
        required=False,
        help="Mode to run telemetry in, either 'mock', 'wireless' or 'log'.",
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

    # Setup the Message Populate Thread
    if args.mode == "wireless":
        InfluxHandler.setup()
        read_thread = get_wireless_task(args.serial_port)
    elif args.mode == "mock":
        InfluxHandler.setup()
        read_thread = get_mock_task(args.data_file)
    elif args.mode == "log":
        read_thread = get_log_read_task()
    else:
        logger.critical("Read thread is empty: No Data will be populated. To select a data source, please specify the \"mode\" flag")
        read_thread = Thread()
    # Reading Thread
    broadcast_thread = get_websocket_broadcast()
    influx_logger_task = InfluxHandler.get_influx_logger_task()

    # Initialize the Socket.IO app with the main app.
    read_thread.start()
    broadcast_thread.start()
    influx_logger_task.start()
    
    # please be adviced, that the 0.0.0.0 is strictly mandatory
    sio.run(app, debug = bool(args.debug), host="0.0.0.0", port=5000, allow_unsafe_werkzeug=True)
    # on keyboard interrupt, the above handles killing
