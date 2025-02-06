"""Debug UBC Formula Electric boards with Python over USB."""

import types

# Note, we must use libusb_package for Windows support when finding devices.
import usb
import libusb_package

import proto_autogen.f4dev_pb2
import proto_autogen.shared_pb2

# Can be any non-zero byte.
_START_RPC_BYTE = 0x01

# CHIMERA Packet Format:
# [ Non-zero Byte    | length low byte  | length high byte | content bytes    | ... ]

def log_usb_devices():
    """Debug utility for printing all available usb devices."""
    devices = libusb_package.find(find_all=True)
    for device in devices:
        print(
            f"Product: {device.product},",
            f"Manufacturer: {device.manufacturer},",
            f"Product ID: {device.idProduct:#x},",
            f"Vendor ID: {device.idVendor:#x}"
        )

class _UsbDevice:
    """Abstraction around a USB CDC (communcations device class) device."""

    def __init__(self, idVendor: int, idProduct: int):
        """Create a USB device.

        Vendor and product ID can be found in STM32 CubeMX.
        """

        self._device = libusb_package.find(idVendor=idVendor, idProduct=idProduct)

        # If the device was not found.
        if self._device == None:

            # Error out, and list all devices.
            print("Error: Specified USB device not found.")
            print("Devices found:")
            log_usb_devices()
            exit(1)

        self._interface = self._device[0][(1,0)]
        self._endpoint_write = self._interface[0]
        self._endpoint_read = self._interface[1]
        self._read_chunk_size = self._endpoint_read.wMaxPacketSize

        # Buffer for read bytes.
        # We have to read in chunks of _read_chunk_size, so if we read too much, 
        # this buffer stores the data for the next call.
        self._read_buf = b''

        # If the kernel is attached to another driver, detach it.
        if self._device.is_kernel_driver_active(self._interface.bInterfaceNumber):
            self._device.detach_kernel_driver(self._interface.bInterfaceNumber)
            
            # Claim the interface explictly.
            usb.util.claim_interface(self._device, self._interface.bInterfaceNumber)

    def __exit__(self):
        """Release the interface explictly."""
        usb.util.release_interface(self._device, self._interface.bInterfaceNumber)

    def write(self, buffer: bytes):
        """Write bytes over usb."""
        self._device.write(self._endpoint_write.bEndpointAddress, buffer)

    def read(self, length: int) -> bytes:
        """Read length bytes over usb.

        Will block until length bytes are received.
        """

        # Read bytes until there are a sufficent amount.
        while len(self._read_buf) < length:
            self._read_buf += bytes(self._device.read(
                self._endpoint_read.bEndpointAddress, 
                self._read_chunk_size,

                # Set a long timeout.
                100000000
            ))

        # Remove the first length bytes from the buffer.
        res = self._read_buf[:length]
        self._read_buf = self._read_buf[length:]
        return res

class _Board:
    """Abstraction around rpc-over-usb-communicating boards."""

    def __init__(self, usb_device: _UsbDevice, gpio_net_name: str, adc_net_name: str, board_module: types.ModuleType):
        """Create a new board."""
        
        self._usb_device = usb_device
        self.gpio_net_name = gpio_net_name
        self.adc_net_name = adc_net_name
        self.board_module = board_module

    def _write(self, msg: proto_autogen.shared_pb2.DebugMessage):
        """Write an RPC message over the provided usb device."""

        # Get data.
        data = msg.SerializeToString()
        data_size = len(data)
        
        # Get low-endian bytes for packet.
        packet_size_bytes = [data_size & 0xff, data_size >> 8]

        # Format and send packet.
        packet = bytes([_START_RPC_BYTE, *packet_size_bytes, *data])
        self._usb_device.write(packet)
    
    def _read(self) -> proto_autogen.shared_pb2.DebugMessage:
        """Read and return an RPC message over the provided usb device."""

        # Consume bytes until a non-zero start byte is found.
        start_bytes = self._usb_device.read(1)
        while start_bytes[0] == 0x00:
            start_bytes = self._usb_device.read(1)

        # Extract data size.
        data_size_bytes = self._usb_device.read(2)
        data_size = int.from_bytes(data_size_bytes, byteorder='little')

        # Extract rpc message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        msg.ParseFromString(self._usb_device.read(data_size))
        return msg
    
    def gpio_read(self, net_name: str) -> bool:
        """Read the value of a GPIO pin given the net name of the pin, returns true if high."""

        # Create and send message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        net_name = self.board_module.GpioNetName.Value(net_name)
        setattr(msg.gpio_read.net_name, self.gpio_net_name, net_name)
        self._write(msg)
        
        # Wait for response.
        response = self._read()

        # Subtract one for enum scale offset.
        return response.gpio_read.value == proto_autogen.shared_pb2.GpioValue.HIGH

    def gpio_write(self, net_name: str, value: bool) -> None:
        """Write a value to the gpio pin indicated by the provided net name, true for high."""
                
        # Create and send message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        net_name = self.board_module.GpioNetName.Value(net_name)
        setattr(msg.gpio_write.net_name, self.gpio_net_name, net_name)

        # Add one for enum scale offset.
        msg.gpio_write.value = value + 1
        self._write(msg)

        # Wait for response, and validate that the request was the same.
        response = self._read()
        assert(response.SerializeToString() == msg.SerializeToString())

    def adc_read(self, net_name: str) -> float:
        """Read the voltage at an adc pin specified by the net name."""

        # Create and send message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        net_name = self.board_module.AdcNetName.Value(net_name)
        setattr(msg.adc.net_name, self.adc_net_name, net_name)
        self._write(msg)

        # Wait for response and return.
        response = self._read()
        return response.adc.value
    
class F4Dev(_Board):
    """Chimera access point for the F4Dev."""
    def __init__(self) -> None:
        super().__init__(
            usb_device=_UsbDevice(idVendor=0x0483, idProduct=0x5740),
            gpio_net_name="f4dev_net_name",
            adc_net_name="f4dev_net_name",
            board_module=proto_autogen.f4dev_pb2
        )
