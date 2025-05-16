import serial

serial_port = serial.Serial("COM3", baudrate=57600, timeout=1)


def get_serial():
    return serial_port
