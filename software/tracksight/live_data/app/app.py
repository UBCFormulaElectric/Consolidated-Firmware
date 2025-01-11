"""
Entrypoint to the telemetry backend
"""

import logging
import os
import threading
from datetime import datetime
from argparse import ArgumentParser
from flask_cors import CORS
from dotenv import load_dotenv
from queue import Queue


from initilize_app import create_app, create_sio
from influx_handler import InfluxHandler
from signal_util import SignalUtil
from api import WebSocket

def setupLogger(debug):
    # Setup logging.
    time_now = datetime.now().strftime("%Y-%m-%d_%H_%M_%S")
    app_dir = os.path.dirname(os.path.abspath(__file__))
    log_path = os.path.join(app_dir, "..", "logs", f"Live-Data--{time_now}.log")

    if not os.path.exists(os.path.dirname(log_path)):
        os.makedirs(os.path.dirname(log_path))

    logger = logging.getLogger("live_data_logger")

    # Set the logging level to DEBUG
    logger.setLevel(level=logging.DEBUG if debug else logging.INFO)

    # Create a file handler
    file_handler = logging.FileHandler(log_path)
    
    # Create a formatter and set it for the file handler
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    file_handler.setFormatter(formatter)

    # Add the file handler to the logger
    logger.addHandler(file_handler)

    return logger, log_path

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

    logger, log_path = setupLogger(args.debug)

    if args.debug:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(filename=log_path, level=logging.INFO)

    if args.mode == "wireless" and args.serial_port is None:
        raise RuntimeError(
            "If running telemetry in wireless mode, you must specify the radio serial port!"
        )
    elif args.mode != "wireless" and args.mode != "mock":
        raise RuntimeError("Mode must be either 'wireless' or 'log' or 'mock'")
    
    # Setup Influx DB
    setupInflux()

    _shared_queue = Queue()

    # Setup Flask app and SocketIO.
    app = create_app()
    sio = create_sio(app)
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

    elif args.mode == "mock":
        if args.data_file is None:
            raise RuntimeError("In 'mock' mode, you must specify the data file to read from")
        
        SignalUtil.setup(app=app, data_file=args.data_file)
        WebSocket.setup(sio)
        stop_event = threading.Event()

        write_thread = threading.Thread(
            target=SignalUtil.read_messages_from_file,
            args=(_shared_queue,),
            daemon=True,
        )

        read_thread = threading.Thread(
            target=WebSocket.send_data,
            args=(_shared_queue, stop_event),
            daemon=True
        )

        try:
            write_thread.start()
            read_thread.start()
            # Initialize the Socket.IO app with the main app.
            # app.run(debug=False, host="0.0.0.0")
            sio.run(app, debug = False)
        except KeyboardInterrupt:
            logger.info("Exiting")

            if write_thread is not None:
                write_thread.join()
                
            stop_event.set()
            if read_thread is not None:
                read_thread.join()

            logger.info("Thread stopped")
