import datetime
from dataclasses import dataclass

# api blueprints
from api.historical_handler import historical_api
from flask import Blueprint, jsonify
from settings import SERIAL_PORT
from logger import logger
from middleware.candb import live_can_db
from middleware.serial_port import get_serial

from api.files_handler import sd_api

api = Blueprint("api", __name__)
api.register_blueprint(historical_api)
api.register_blueprint(sd_api)


@api.route("/health", methods=["GET"])
def hello_world():
    return "<p>Hello, World!</p>"


# Viewing Live Data
# this technically shouldn't be inline but who cares


@api.route("/signal", methods=["GET"])
def get_signal_metadata():
    """
    Gets all the signals of the current parser
    """
    return jsonify([
        {
            "name": signal.name,
            "min_val": signal.min_val,
            "max_val": signal.max_val,
            "unit": signal.unit,
            "enum": signal.enum,
            "tx_node": msg.tx_node_name,
            "cycle_time_ms": msg.cycle_time,
            "id": msg.id,
            "msg_name": msg.name,
        }
        for msg in live_can_db.msgs.values()
        for signal in msg.signals
    ]), 200

@api.route("/signal/<signal_name>", methods=["GET"])
def get_cached_signals(signal_name: str):
    """
    Gets the signal name data (from the last 5 minutes) from the influx DB
    """
    msg = live_can_db.signals_to_msgs.get(signal_name)
    if msg is None:
        return jsonify({"error": "Signal not found"}), 404
    matching_signals = [
        signal for signal in msg.signals if signal.name == signal_name
    ]
    if len(matching_signals) == 0:
        return jsonify({"error": "Signal not found"}), 404
    if len(matching_signals) > 1:
        return jsonify({"error": "Multiple signals with the same name found"}), 500
    signal = matching_signals[0]
    return jsonify({
        "name": signal.name,
        "description": signal.description,
        "tx_node": msg.tx_node_name,
        "cycle_time_ms": msg.cycle_time,
        # TODO figure out what we need from the individual signal metadata
    }), 200


@dataclass
class RtcTime:
    """
    Class to handle the RTC time
    """

    year: int  # 0-99
    month: int  # 1-12
    weekday: int  # 0-6 (0=Sunday)
    day: int  # 1-31
    hour: int  # 0-23
    minute: int  # 0-59
    second: int  # 0-59


def set_rtc_time(serial_port: str, time: RtcTime):
    """
    Sets the RTC time
    """

    ser = get_serial(serial_port)
    if ser is None:
        logger.error("Serial port not found, cannot set RTC time")
        return {"success": False, "error": "Serial port not found"}, 500
    # create a bytearray to hold from Rtctime
    buffer = bytearray(9)
    buffer[0] = 0xFF
    buffer[1] = 0x01
    # set the year
    buffer[2] = time.second
    # set the month
    buffer[3] = time.minute
    # set the weekday
    buffer[4] = time.hour
    # set the day
    buffer[5] = time.day
    # set the hour
    buffer[6] = time.weekday
    # set the minute
    buffer[7] = time.month
    # set the second
    buffer[8] = time.year
    # write the buffer to the serial port

    # ser.write(buffer)

    return {"success": True}, 200


@api.route("/rtc", methods=["GET"])
def api_set_rtc_time():
    """
    Sets the RTC time
    """
    if SERIAL_PORT is None:
        logger.error("Serial port not configured, cannot set RTC time")
        return {"success": False, "error": "Serial port not configured"}, 500 

    # get the system time
    time = datetime.datetime.now()

    rtcTime = RtcTime(
        year=time.year % 100,  # RTC only accepts 2 digit year
        month=time.month,
        weekday=time.weekday(),
        day=time.day,
        hour=time.hour,
        minute=time.minute,
        second=time.second,
    )  # type: ignore
    print(time)

    # set the time
    return set_rtc_time(SERIAL_PORT, rtcTime)
