"""
API for the serial debugger library.
"""

# import subprocess
import serial

# from importlib import reload
import validationtools.validationtools.chimera.proto_libs.shared_pb2 as shared_lib
import validationtools.validationtools.chimera.proto_libs.VC_pb2 as VC_lib
import validationtools.validationtools.chimera.proto_libs.BMS_pb2 as BMS_lib
import validationtools.validationtools.chimera.proto_libs.FSM_pb2 as FSM_lib
import validationtools.validationtools.chimera.proto_libs.RSM_pb2 as RSM_lib
import validationtools.validationtools.chimera.proto_libs.CRIT_pb2 as CRIT_lib

DEBUG_SIZE_MSG_BUF_SIZE = 1


# proto_libs = [VC_lib, BMS_lib, FSM_lib, RSM_lib, CRIT_lib]


class Board:
    def __init__(self, com_port: str) -> None:
        # TODO: Fix for Windows.
        # subprocess.call("./generate_proto_libs.sh")
        # for lib in proto_libs:
        #     reload(lib)
        # NOTE: If you send more or less data in a UART transaction, seems like the
        # peripheral can get confused...
        self.ser = serial.Serial(com_port, baudrate=115200)

    def __exit__(self) -> None:
        self.ser.close()

    def send_packet_size(self, packet_size: int) -> None:
        serialized_packet_size = packet_size.to_bytes(1, "little")
        self.ser.write(serialized_packet_size)

    def get_incoming_packet_size(self) -> int:
        return int.from_bytes(
            self.ser.read(DEBUG_SIZE_MSG_BUF_SIZE), byteorder="little"
        )

    def gpio_read(self, net_name: str) -> int:
        """
        Read from a GPIO.
        """
        msg = shared_lib.DebugMessage()
        net_name = self.board_lib.GpioNetName.Value(net_name)
        setattr(msg.gpio_read.net_name, self.gpio_net_name, net_name)

        serialized_gpio = msg.SerializeToString()
        packet_size = len(serialized_gpio)
        self.send_packet_size(packet_size)
        self.ser.write(serialized_gpio)

        # wait to receive written value back
        packet_size = self.get_incoming_packet_size()
        msg.ParseFromString(self.ser.read(packet_size))
        return msg.gpio_read.value - 1  # subtract one for enum scale offset

    def gpio_write(self, net_name: str, value: bool) -> None:
        """
        Write to a GPIO.
        """
        msg = shared_lib.DebugMessage()
        net_name = self.board_lib.GpioNetName.Value(net_name)
        setattr(msg.gpio_write.net_name, self.gpio_net_name, net_name)
        msg.gpio_write.value = value + 1  # add one for enum scale offset

        serialized_msg = msg.SerializeToString()
        packet_size = len(serialized_msg)
        self.send_packet_size(packet_size)
        self.ser.write(serialized_msg)

        # wait to receive written value back
        packet_size = self.get_incoming_packet_size()
        assert (
                self.ser.read(packet_size) == serialized_msg
        ), "Debug Message Not Received"

    def adc_read(self, net_name: str) -> float:
        msg = shared_lib.DebugMessage()
        net_name = self.board_lib.AdcNetName.Value(net_name)
        setattr(msg.adc.net_name, self.adc_net_name, net_name)

        serialized_adc = msg.SerializeToString()
        packet_size = len(serialized_adc)
        self.send_packet_size(packet_size)
        self.ser.write(serialized_adc)

        # wait to receive written value back
        packet_size = self.get_incoming_packet_size()
        msg.ParseFromString(self.ser.read(packet_size))
        return msg.adc.value


class VC(Board):
    def __init__(self, com_port: str) -> None:
        super().__init__(com_port)
        self.board_lib = VC_lib
        self.gpio_net_name = "vc_net_name"
        self.adc_net_name = "vc_net_name"


class BMS(Board):
    def __init__(self, com_port: str) -> None:
        super().__init__(com_port)
        self.board_lib = BMS_lib
        self.gpio_net_name = "bms_net_name"
        self.adc_net_name = "bms_net_name"


class FSM(Board):
    def __init__(self, com_port: str) -> None:
        super().__init__(com_port)
        self.board_lib = FSM_lib
        self.gpio_net_name = "fsm_net_name"
        self.adc_net_name = "fsm_net_name"


class RSM(Board):
    def __init__(self, com_port: str) -> None:
        super().__init__(com_port)
        self.board_lib = RSM_lib
        self.gpio_net_name = "rsm_net_name"
        self.adc_net_name = "rsm_net_name"


class CRIT(Board):
    def __init__(self, com_port: str) -> None:
        super().__init__(com_port)
        self.board_lib = CRIT_lib
        self.gpio_net_name = "crit_net_name"
        self.adc_net_name = "crit_net_name"
