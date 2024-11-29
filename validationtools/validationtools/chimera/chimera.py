import usb 
import usb.core
import usb.util
import time 
import sys

import validationtools.chimera.proto_libs.shared_pb2 as shared_lib
import validationtools.chimera.proto_libs.VC_pb2 as VC_lib
import validationtools.chimera.proto_libs.BMS_pb2 as BMS_lib
import validationtools.chimera.proto_libs.FSM_pb2 as FSM_lib
import validationtools.chimera.proto_libs.RSM_pb2 as RSM_lib
import validationtools.chimera.proto_libs.CRIT_pb2 as CRIT_lib
# import validaitontools.chimera.proto_libs.SSM as SSM


class Board:
    def __init__(self, id_vendor, id_product) -> None:
        self.dev = usb.core.find (id_vendor,id_product)
        self.interface = self.dev[0][(1,0)]
        self.W_ep = self.interface[0]
        self.R_ep = self.interface[1]
        if self.dev.is_kernel_driver_active(self.interface.bInterfaceNumber):
            try:
                self.dev.detach_kernel_driver(self.interface.bInterfaceNumber)
                usb.util.claim_interface(self.dev, self.interface.bInterfaceNumber)
            except usb.core.USBError as e:
                sys.exit("Could not detatch kernel driver from interface({0}): {1}".format(i, str(e)))

    
    def __exit__(self):
        usb.util.release_interface(self.dev, self.interface.bInterfaceNumber)

    def send_packet_size(self, packet_size: int) -> None:
        serialized_packet = packet_size.to_bytes(1,"little")
        self.dev.write(self.W_ep.bEndpointAddress, serialized_packet)
    
    def get_incoming_packet_size(self) -> int:
        serialized_read = self.dev.read(self.R_ep.bEndpointAddress, self.R_ep.wMaxPacketSize)
        return int.from_bytes(serialized_read, "little")
    
    #pass in the gpio name of what we want to read 
    def gpio_read(self,net_name: str) -> int:
        msg = shared_lib.DebugMessage()
        #this is where we pass in the GPIO and net_name is now a machine readable code (enumerated value) for the gpio 
        net_name = self.board_lib.GpioNetName.Value(net_name)
        setattr(msg.gpio_read.net_name, self.gpio_net_name, net_name)
        #ready as a binary for example
        serialized_gpio = msg.SerializeToString()
        packet_size = len(serialized_gpio)
        self.send_packet_size(packet_size)
        self.dev.write(self.W_ep.bEndpointAddress, serialized_gpio)

        # wait to receive written value back
        packet_size = self.get_incoming_packet_size()
        msg.ParseFromString(self.dev.read(self.R_ep.bEndpointAddress, self.R_ep.wMaxPacketSize))
        return msg.gpio_read.value - 1  # subtract one for enum scale offset
    
    
    def gpio_write(self, net_name: str, value: bool)-> None:

        msg = shared_lib.DebugMessage()
        net_name = self.board_lib.GPIONetName.Value(net_name)
        setattr(msg.gpio_write.net_name, self.gpio_net_name, net_name)
        msg.gpio_write.value = value + 1  # add one for enum scale offset

        serialized_msg = msg.SerializeToString()
        packet_size = len(serialized_msg)
        self.send_packet_size(packet_size)
        self.dev.write(self.W_ep.bEndpointAddress, serialized_msg)

        # wait to receive written value back
        packet_size = self.get_incoming_packet_size()
        assert (
                self.dev.read(self.R_ep.bEndpointAddress, self.R_ep.wMaxPacketSize) == serialized_msg
        ), "Debug Message Not Received"

    def adc_read(self, net_name: str) -> float:
        msg = shared_lib.DebugMessage()
        net_name = self.board_lib.AdcNetName.Value(net_name)
        setattr(msg.adc.net_name, self.adc_net_name, net_name)

        serialized_adc = msg.SerializeToString()
        packet_size = len(serialized_adc)
        self.send_packet_size(packet_size)
        self.dev.write(self.W_ep.bEndpointAddress, serialized_adc)

        # wait to receive written value back
        packet_size = self.get_incoming_packet_size()
        msg.ParseFromString(self.dev.read(self.R_ep.bEndpointAddress, self.R_ep.wMaxPacketSize))
        return msg.adc.value

# Now I make subclasses of type board!!! 

class VC(Board):
    def __init__(self, id_vendor, id_product):
        super().__init__(id_vendor, id_product)
        self.board_lib = VC_lib
        self.gpio_net_name = "vc_net_name"
        self.adc_net_name = "vc_net_name"

class BMS(Board):
    def __init__(self, id_vendor, id_product):
        super().__init__(id_vendor, id_product)
        self.board_lib = BMS_lib
        self.gpio_net_name = "bms_net_name"
        self.adc_net_name = "bms_net_name"

class FSM(Board):
    def __init__(self, id_vendor, id_product):
        super().__init__(id_vendor, id_product)
        self.board_lib = FSM_lib
        self.gpio_net_name = "fsm_net_name"
        self.adc_net_name = "fsm_net_name"

class RSM(Board):
    def __init__(self, id_vendor, id_product):
        super().__init__(id_vendor, id_product)        
        self.board_lib = RSM_lib
        self.gpio_net_name = "rsm_net_name"
        self.adc_net_name = "rsm_net_name"

class CRIT(Board):
    def __init__(self, id_vendor, id_product):
        super().__init__(id_vendor, id_product)
        self.board_lib = CRIT_lib
        self.gpio_net_name = "crit_net_name"
        self.adc_net_name = "crit_net_name"


# class SSM(Board):
#     def __init__(self, com_port: str) -> None:
#         super().__init__(com_port)
#         self.board_lib = SSM_lib
#         self.gpio_net_name = "ssm_net_name"
#         self.adc_net_name = "ssm_net_name"

    


        






    
