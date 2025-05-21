import datetime
from calendar import week, weekday
from dataclasses import dataclass

import influxdb_client
import serial

# api blueprints
from api.historical_handler import historical_api
from flask import Blueprint, Response, request
from logger import logger
from middleware.candb import live_can_db
from middleware.serial_port import get_serial

# ours
from settings import CAR_NAME, INFLUX_BUCKET, INFLUX_ORG, INFLUX_TOKEN, INFLUX_URL

# from api.files_handler import sd_api

api = Blueprint("api", __name__)
api.register_blueprint(historical_api)
# api.register_blueprint(sd_api)


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
    return [
        {
            "name": signal.name,
            "min_val": signal.min_val,
            "max_val": signal.max_val,
            "unit": signal.unit,
            "enum": signal.enum,
            "tx_node": msg.tx_node,
            "cycle_time_ms": msg.cycle_time,
        }
        for msg in live_can_db.msgs.values()
        for signal in msg.signals
    ]


@api.route("/signal/<signal_name>", methods=["GET"])
def get_cached_signals(signal_name: str):
    """
    Gets the signal name data (from the last 5 minutes) from the influx DB
    """
    # query for the signal on the CURRENT LIVE CAR_live for all data points in the last prev_time
    with influxdb_client.InfluxDBClient(
        url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG, debug=False
    ) as client:
        query = f"""from(bucket:"{INFLUX_BUCKET}")
            |> range(start: -1m)
            |> filter(fn: (r) => r._measurement == "{CAR_NAME}_live" and r._field == "{signal_name}")
            |> tail(n: {6000})"""
        table = client.query_api().query(query)
    signals = table.to_json(columns=["_time", "_value"], indent=1)
    return Response(signals, status=200, mimetype="application/json")


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


def set_rtc_time(time: RtcTime):
    """
    Sets the RTC time
    """

    ser = get_serial()
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

    ser.write(buffer)


@api.route("/rtc", methods=["GET"])
def api_set_rtc_time():
    """
    Sets the RTC time
    """
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
    set_rtc_time(rtcTime)

    return {"success": True}, 200
