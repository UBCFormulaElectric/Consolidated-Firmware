"""
API for the serial debugger library.
"""

import subprocess
import serial
from importlib import reload
import board_libs.VC_pb2 as VC_lib


DEBUG_SIZE_MSG_BUF_SIZE = 1

class Board():
    def __init__(self, com_port: str) -> None:
        subprocess.call("./generate_board_libs.sh")
        reload(VC_lib)
        # NOTE: If you send more or less data in a UART transaction, seems like the
        # peripheral can get confused...
        self.ser = serial.Serial(com_port, baudrate=115200)

    def __exit__(self) -> None:
        self.ser.close()

    def send_packet_size(self, packet_size: int) -> None:
        serialized_packet_size = packet_size.to_bytes(1, "little")
        self.ser.write(serialized_packet_size)

    def get_incoming_packet_size(self) -> int:
        return int.from_bytes(self.ser.read(DEBUG_SIZE_MSG_BUF_SIZE), byteorder='little')


class VC(Board):
    def __init__(self, com_port: str) -> None:
        super().__init__(com_port)
        self.debug_msg = VC_lib.DebugMessage()
    
    def gpio_read(self, net_label: str) -> int:
        self.debug_msg.gpio_read.net_name = VC_lib.GpioNetName.Value(net_label)

        serialized_gpio = self.debug_msg.SerializeToString()
        packet_size = len(serialized_gpio)
        self.send_packet_size(packet_size)
        self.ser.write(serialized_gpio)

        packet_size = self.get_incoming_packet_size()

        self.debug_msg.ParseFromString(self.ser.read(packet_size))
        return self.debug_msg.gpio_read.value - 1  # subtract one for enum scale offset

    def gpio_write(self, net_label: str, value: bool) -> None:
        self.debug_msg.gpio_write.net_name = VC_lib.GpioNetName.Value(net_label)
        self.debug_msg.gpio_write.value = value + 1  # add one for enum scale offset

        serialized_gpio = self.debug_msg.SerializeToString()
        packet_size = len(serialized_gpio)
        self.send_packet_size(packet_size)
        self.ser.write(serialized_gpio)

        # wait to receive written value back
        packet_size = self.get_incoming_packet_size()
        assert self.ser.read(packet_size) == serialized_gpio, "Debug Message Not Received"

    def adc_read(self, net_label: str) -> float:
        self.debug_msg.adc.net_name = VC_lib.AdcNetName.Value(net_label)

        serialized_adc = self.debug_msg.SerializeToString()
        packet_size = len(serialized_adc)
        self.send_packet_size(packet_size)
        self.ser.write(serialized_adc)

        packet_size = self.get_incoming_packet_size()

        self.debug_msg.ParseFromString(self.ser.read(packet_size))
        return self.debug_msg.adc.value
