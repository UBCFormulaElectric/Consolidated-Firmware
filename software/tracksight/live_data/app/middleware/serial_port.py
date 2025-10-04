from typing import Dict
import serial
from settings import DATA_SOURCE

_serial_port_map: Dict[str, serial.Serial] = {}

def get_serial(serial_port: str):
    """
    Returns the serial port instance.
    If the serial port is not enabled or not set, returns None.
    Note that once a serial port is opened, it will be reused for subsequent calls. This may result in stale ports
    """
    if DATA_SOURCE != "WIRELESS":
        raise ValueError("Wireless is not enabled. Cannot get serial port.")

    if serial_port not in _serial_port_map:
        _serial_port_map[serial_port] = serial.Serial(serial_port, baudrate=57600, timeout=1)
        _serial_port_map[serial_port].reset_input_buffer()  # Clear input buffer
        _serial_port_map[serial_port].reset_output_buffer()  # Clear output buffer

    return _serial_port_map[serial_port]
