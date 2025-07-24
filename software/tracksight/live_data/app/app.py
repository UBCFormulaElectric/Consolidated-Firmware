"""
Entrypoint to the telemetry backend
"""

import logging
from threading import Thread
from typing import Optional

# tasks
import tasks.influx_logger as InfluxHandler
from tasks.broadcaster import get_websocket_broadcast
from tasks.read_task.mock import get_mock_task
from tasks.read_task.wireless import get_wireless_task

# apis
from flask_app import app
from api.http import api
from api.socket import sio

# infra
from logger import log_path, logger
from mDNS import register_mdns_service
from settings import *


# register blueprint for python
app.register_blueprint(api, url_prefix="/api")

@app.route("/", methods=["GET"])
def index():
    return "<p>Welcome to the UBC Formula Electric Telemetry Backend</p>"

if DEBUG:
    logging.basicConfig(level=logging.DEBUG)
else:
    logging.basicConfig(filename=log_path, level=logging.INFO)

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
    # Setup the Message Populate Thread
    InfluxHandler.setup()

    if ENABLE_WIRELESS:
        wireless_thread = get_wireless_task(SERIAL_PORT)
        wireless_thread.start()
    if ENABLE_MOCK:
        mock_thread = get_mock_task()
        mock_thread.start()

    if ENABLE_MOCK and ENABLE_WIRELESS:
        logger.warning("Both mock and wireless tasks are enabled. This may lead to unexpected behavior.")

    if not ENABLE_MOCK and not ENABLE_WIRELESS:
        raise RuntimeError("No data source enabled. Please enable either mock or wireless data source.")

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