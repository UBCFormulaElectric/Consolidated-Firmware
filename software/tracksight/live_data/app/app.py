"""
Entrypoint to the telemetry backend
"""

import logging

# tasks
import tasks.influx_logger as InfluxHandler
from api.http import api
from api.socket import sio

# apis
from flask_app import app
from logger import log_path, logger
from mDNS import register_mdns_service
from settings import *
from tasks.broadcaster import get_websocket_broadcast
from tasks.read_task.mock import get_mock_task
from tasks.read_task.wireless import get_wireless_task

# register blueprint for python
app.register_blueprint(api, url_prefix="/api")

# Note this must be done first as there are static level os.env gets

logger.setLevel(level=logging.DEBUG if DEBUG else logging.INFO)
if DEBUG:
    logging.basicConfig(level=logging.DEBUG)
else:
    logging.basicConfig(filename=log_path, level=logging.INFO)

    # Setup the Message Populate Thread

InfluxHandler.setup()

wireless_thread = None
if ENABLE_WIRELESS:
    wireless_thread = get_wireless_task(SERIAL_PORT)

mock_thread = None
if ENABLE_MOCK:
    InfluxHandler.setup()
    mock_thread = get_mock_task()

# Reading Thread
broadcast_thread = get_websocket_broadcast()
influx_logger_task = InfluxHandler.get_influx_logger_task()

# Initialize the Socket.IO app with the main app.
if ENABLE_WIRELESS:
    wireless_thread.start()
if ENABLE_MOCK:
    mock_thread.start()

broadcast_thread.start()
influx_logger_task.start()

if(not DEBUG): # only when debug is off because it in debug mode it will create a subprocess and run this again
    register_mdns_service(SERVER_IP, SERVER_DOMAIN_NAME)

# please be adviced, that the 0.0.0.0 is strictly mandatory
sio.run(
    app,
    debug=bool(DEBUG),
    host="0.0.0.0",
    port=5000,
)
# on keyboard interrupt, the above handles killing
