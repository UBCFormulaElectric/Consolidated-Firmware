import proto_autogen
import usb

# Can be any non-zero byte.
START_RPC_BYTE = 0x01

class UsbDevice:
    # Abstraction around a USB CDC (communcations device class) device.
    # Vendor and product ID can be found in STM32 CubeMX.
    def __init__(self, idVendor: int, idProduct: int):
        self._device = usb.core.find(idVendor=idVendor, idProduct=idProduct)
        self._interface = self.device[0][(1,0)]
        self._endpoint_write = self.interface[0]
        self._endpoint_read = self.interface[1]

        # If the kernel is _attachted to another driver, detach it.
        if self._device.is_kernel_driver_active(self._interface.bInterfaceNumber):
            self._device.detach_kernel_driver(self._interface.bInterfaceNumber)

            # TODO: Investigate if claiming the interface explictly is needed.
            usb.util.claim_interface(self._device, self._interface.bInterfaceNumber)

    def __exit__(self):
        # Release the interface explictly.
        # TODO: Investigate if releasing the interface explictly is needed.
        usb.util.release_interface(self._device, self._interface.bInterfaceNumber)

    # Write bytes over usb.
    def write(self, buffer: bytes):
        self._device.write(self._endpoint_write.bEndpointAddress, buffer)

    # Read length bytes over usb.
    # Will block until length bytes are received.
    def read(self, length: int) -> bytes:
        res = []
        while len(res) < length:
            res.extend(self._device.read(
                self._endpoint_write.bEndpointAddress, 
                length - len(res)
            ))
        return bytes(res)

class Board:
    # Abstraction around rpc-communicating boards.
    def __init__(self, usb_device: UsbDevice):
        self._usb_device = usb_device

    # CHIMERA Packet Format:
    # [ Non-zero Byte    | length low byte  | length high byte | content bytes    | ... ]

    # Write an RPC message over the provided usb device.
    def _write(self, msg: str):
        # Get data.
        data = msg.SerializeToString()
        data_size = len(data)
        
        # Get low-endian bytes for packet.
        packet_size_bytes = [data_size & 0xff, data_size >> 8]

        # Format and send packet.
        packet = bytes([START_RPC_BYTE, *packet_size_bytes, *data.split('')])
        self._usb_device.write(packet)
    
    # Read and return an RPC message over the provided usb device.
    def _read(self) -> int:
        # Consume bytes until a non-zero start byte is found.
        start_bytes = self._usb_device.read(1)
        while start_bytes[0] == 0x00:
            start_bytes = self._usb_device.read(1)
        
        # Extract data size.
        data_size_bytes = self._usb_device.read(2)
        data_size = data_size_bytes[0] + data_size_bytes[1] << 8

        # Extract rpc message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        msg.ParseFromString(self._usb_device.read(data_size))
        return msg
    
    def gpio_read(self, net_name: str) -> int:
        # Create and send message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        net_name = self.board_lib.GpioNetName.Value(net_name)
        setattr(msg.gpio_read.net_name, self.gpio_net_name, net_name)
        self._write(msg)
        
        # Wait for response.
        response = self.read()

        # Subtract one for enum scale offset.
        return response.gpio_read.value - 1

    def gpio_write(self, net_name: str, value: bool) -> None:
        # Create and send message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        net_name = self.board_lib.GpioNetName.Value(net_name)
        setattr(msg.gpio_write.net_name, self.gpio_net_name, net_name)

        # Add one for enum scale offset.
        msg.gpio_write.value = value + 1
        self._write(msg)

        # Wait for response, and validate that the request was the same.
        response = self.read(msg)
        assert(response.SerializeToString() == msg.SerializeToString())

    def adc_read(self, net_name: str) -> float:
        # Create and send message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        net_name = self.board_lib.AdcNetName.Value(net_name)
        setattr(msg.adc.net_name, self.adc_net_name, net_name)
        self._write(msg)

        # Wait for response and return.
        response = self.read()
        return response.adc.value
    
class VC(Board):
    def __init__(self, com_port: str) -> None:
        super().__init__(com_port)
        self.board_lib = proto_autogen.VC_pb2
        self.gpio_net_name = "vc_net_name"
        self.adc_net_name = "vc_net_name"

class BMS(Board):
    def __init__(self, com_port: str) -> None:
        super().__init__(com_port)
        self.board_lib = proto_autogen.BMS_pb2
        self.gpio_net_name = "bms_net_name"
        self.adc_net_name = "bms_net_name"

class FSM(Board):
    def __init__(self, com_port: str) -> None:
        super().__init__(com_port)
        self.board_lib = proto_autogen.FSM_pb2
        self.gpio_net_name = "fsm_net_name"
        self.adc_net_name = "fsm_net_name"

class RSM(Board):
    def __init__(self, com_port: str) -> None:
        super().__init__(com_port)
        self.board_lib = proto_autogen.RSM_pb2
        self.gpio_net_name = "rsm_net_name"
        self.adc_net_name = "rsm_net_name"

class CRIT(Board):
    def __init__(self, com_port: str) -> None:
        super().__init__(com_port)
        self.board_lib = proto_autogen.CRIT_pb2
        self.gpio_net_name = "crit_net_name"
        self.adc_net_name = "crit_net_name"
