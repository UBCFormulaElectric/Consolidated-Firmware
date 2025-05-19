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
from settings import *
from tasks.broadcaster import get_websocket_broadcast
from tasks.read_task.mock import get_mock_task
from tasks.read_task.wireless import get_wireless_task

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
if ENABLE_MORK == "mock":
    InfluxHandler.setup()
    mock_thread = get_mock_task(DATA_FILE)

# Reading Thread
broadcast_thread = get_websocket_broadcast()
influx_logger_task = InfluxHandler.get_influx_logger_task()

# Initialize the Socket.IO app with the main app.
if ENABLE_WIRELESS:
    wireless_thread.start()
if ENABLE_MORK:
    mock_thread.start()

broadcast_thread.start()
influx_logger_task.start()

# please be adviced, that the 0.0.0.0 is strictly mandatory
sio.run(
    app,
    debug=bool(DEBUG),
    host="0.0.0.0",
    port=5000,
    allow_unsafe_werkzeug=True,
)
# on keyboard interrupt, the above handles killing
