import datetime
from dataclasses import dataclass
from flask import Blueprint

from settings import SERIAL_PORT
from logger import logger
from middleware.serial_port import get_serial

rtc = Blueprint("rtc", __name__)

# NOTE deprecated by ntp.py
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


@rtc.route("/rtc", methods=["GET"])
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
