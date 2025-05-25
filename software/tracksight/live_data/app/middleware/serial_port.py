import serial
from settings import ENABLE_WIRELESS, SERIAL_PORT


class SerialPortSingleton:
    _instance = None
    _serial_port = None

    def __new__(cls):
        print("Creating SerialPortSingleton instance")
        if cls._instance is None:
            cls._instance = super(SerialPortSingleton, cls).__new__(cls)
            if ENABLE_WIRELESS and SERIAL_PORT:
                cls._serial_port = serial.Serial(SERIAL_PORT, baudrate=57600, timeout=1)
        return cls._instance

    def __del__(self):
        if self._serial_port:
            self._serial_port.close()
            self._serial_port = None

    def get_serial(self):
        return self._serial_port
