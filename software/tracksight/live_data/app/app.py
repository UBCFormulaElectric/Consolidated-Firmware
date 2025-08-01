"""
Entrypoint to the telemetry backend
"""
print("Starting app...")
import time

import_start_time = time.time()
from gevent import monkey
monkey.patch_all() # very important :)

from settings import *
from logger import logger

from flask_app import app
from sio import sio

from threading import Thread
from typing import Optional

# tasks
import tasks.influx_logger as InfluxHandler
from tasks.broadcaster import get_websocket_broadcast
from tasks.read_task.mock import get_mock_task
from tasks.read_task.wireless import get_wireless_task
from tasks.stop_signal import stop_run_signal

# apis
from api.historical_handler import historical_api
from api.files_handler import sd_api
from api.rtc_handler import rtc
from api.http import http
from api.subtable_handler import sub_handler

from mDNS import register_mdns_service

import_end_time = time.time()
logger.debug(f"Importing took {import_end_time - import_start_time:.2f} seconds")

# this thread populates the message queue(s) with real data from the car
wireless_thread: Optional[Thread] = None
# this thread populates the message queue(s) with mock data
mock_thread: Optional[Thread] = None

# this thread broadcasts the data to the frontend
ws_broadcast_thread: Optional[Thread] = None
# this thread logs the data to influxdb
influx_logger_task: Optional[Thread] = None

def create_app():
    global wireless_thread, mock_thread, ws_broadcast_thread, influx_logger_task

    # register blueprint for python
    app.register_blueprint(sd_api)
    app.register_blueprint(historical_api)
    app.register_blueprint(http)
    app.register_blueprint(rtc)
    app.register_blueprint(sub_handler)

    influx_start_time = time.time()
    # Setup the Message Populate Thread
    InfluxHandler.setup()
    logger.debug(f"InfluxDB setup took {time.time() - influx_start_time:.2f} seconds")

    if DATA_SOURCE == "WIRELESS":
        wireless_thread = get_wireless_task(sio)
    elif DATA_SOURCE == "MOCK":
        mock_thread = get_mock_task(sio)
    else:
        raise RuntimeError("Data source is not valid")
    
    # Reading Thread
    ws_broadcast_thread = get_websocket_broadcast(sio)
    influx_logger_task = InfluxHandler.get_influx_logger_task(sio)

    if not DEBUG and SERVER_IP:  # only when debug is off because it in debug mode it will create a subprocess and run this again
        register_mdns_service(SERVER_IP, SERVER_DOMAIN_NAME)
    return app

if __name__ == "__main__":
    create_app() # cast to void :) we already have a reference
    try:
        logger.info("Starting Flask app with SocketIO...")
        sio.run(
            app,
            debug=False,
            host="0.0.0.0", # 0.0.0.0 means to listen on all network interfaces (ethernet, wifi, etc.)
            port=5000,
        )
    except KeyboardInterrupt:
        # on keyboard interrupt, the above handles killing
        logger.info("Keyboard Interrupt received, shutting down...")
    finally:
        stop_run_signal()
        if wireless_thread: wireless_thread.join()
        if mock_thread: mock_thread.join()
        if ws_broadcast_thread: ws_broadcast_thread.join()
        if influx_logger_task: influx_logger_task.join()