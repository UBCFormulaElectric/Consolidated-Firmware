import sys

import serial
from settings import ENABLE_WIRELESS, SERIAL_PORT

# class SerialPortSingleton:
#     _instance = None
#     _serial_port = None

#     def __new__(cls):
#         print("Creating SerialPortSingleton instance")
#         if cls._instance is None:
#             cls._instance = super(SerialPortSingleton, cls).__new__(cls)
#             if ENABLE_WIRELESS and SERIAL_PORT:
#                 try:
#                     cls._serial_port = serial.Serial(
#                         SERIAL_PORT, baudrate=57600, timeout=1
#                     )
#                     cls._serial_port.reset_input_buffer()  # Clear input buffer
#                     cls._serial_port.reset_output_buffer()  # Clear output buffer
#                 except serial.SerialException:
#                     sys.exit()
#         return cls._instance

#     def __del__(self):
#         if self._serial_port:
#             self._serial_port.close()
#             self._serial_port = None

#     def get_serial(self):
#         return self._serial_port


_serial_port = None
if ENABLE_WIRELESS and SERIAL_PORT:
    try:
        _serial_port = serial.Serial(SERIAL_PORT, baudrate=57600, timeout=1)
        _serial_port.reset_input_buffer()  # Clear input buffer
        _serial_port.reset_output_buffer()  # Clear output buffer
    except serial.SerialException:
        print("No serial found or the serial is taken")
        sys.exit()


class SerialPortSingleton:
    _instance = None
    _serial_port = None


def get_serial():
    """
    Returns the serial port instance.
    If the serial port is not enabled or not set, returns None.
    """
    return _serial_port if ENABLE_WIRELESS and SERIAL_PORT else None
