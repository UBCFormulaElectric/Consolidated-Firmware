import serial
from settings import ENABLE_WIRELESS, SERIAL_PORT

serial_port = None
if ENABLE_WIRELESS:
    serial_port = serial.Serial(SERIAL_PORT, baudrate=57600, timeout=1)


def get_serial():
    return serial_port
