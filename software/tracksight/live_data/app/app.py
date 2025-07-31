"""
Entrypoint to the telemetry backend
"""
from settings import *

# Very import this is first
import logging
from logger import log_path, logger
# if DEBUG:
#     logging.basicConfig(level=logging.DEBUG)
# else:
#     logging.basicConfig(filename=log_path, level=logging.INFO)

from threading import Thread
import time
from typing import Optional

# tasks
import tasks.influx_logger as InfluxHandler
from tasks.broadcaster import get_websocket_broadcast
from tasks.read_task.mock import get_mock_task
from tasks.read_task.wireless import get_wireless_task

# apis
from api.historical_handler import historical_api
from api.files_handler import sd_api
from api.rtc_handler import rtc
from api.http import http

from api.socket import sio
from flask_app import app

# infra
from mDNS import register_mdns_service


# register blueprint for python
app.register_blueprint(sd_api)
app.register_blueprint(historical_api)
app.register_blueprint(http)
app.register_blueprint(rtc)

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

    influx_start_time = time.time()
    # Setup the Message Populate Thread
    InfluxHandler.setup()
    logger.info(f"InfluxDB setup took {time.time() - influx_start_time:.2f} seconds")

    if DATA_SOURCE == "WIRELESS":
        wireless_thread = get_wireless_task(SERIAL_PORT)
        wireless_thread.start()
    elif DATA_SOURCE == "MOCK":
        mock_thread = get_mock_task()
        mock_thread.start()
    else:
        raise RuntimeError("Data source is not valid")
    
    # Reading Thread
    ws_broadcast_thread = get_websocket_broadcast()
    ws_broadcast_thread.start()

    influx_logger_task = InfluxHandler.get_influx_logger_task()
    influx_logger_task.start()

    if not DEBUG:  # only when debug is off because it in debug mode it will create a subprocess and run this again
        register_mdns_service(SERVER_IP, SERVER_DOMAIN_NAME)

    sio.init_app(app, cors_allowed_origins="*")  # Allow all origins for CORS
    return app

if __name__ == "__main__":
    create_app() # cast to void :) we already have a reference
    try:
        sio.run(
            app,
            debug=bool(DEBUG),
            host="0.0.0.0", # 0.0.0.0 means to listen on all network interfaces (ethernet, wifi, etc.)
            port=5000,
        )
    except KeyboardInterrupt:
        # on keyboard interrupt, the above handles killing
        logger.info("Keyboard Interrupt received, shutting down...")
        if wireless_thread:
            wireless_thread.join()
            logger.trace("Wireless thread stopped.")
        if mock_thread:
            mock_thread.join()
            logger.trace("Mock thread stopped.")
        if ws_broadcast_thread:
            ws_broadcast_thread.join()
            logger.trace("Broadcast thread stopped.")
        if influx_logger_task:
            influx_logger_task.join()
            logger.trace("InfluxDB logger thread stopped.")