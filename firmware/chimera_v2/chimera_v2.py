"""Debug UBC Formula Electric boards with Python.

Provides tooling to debug devices over USB, CAN, etc.

"""

import types
from typing import Dict, Optional
import threading

import libusb_package
import cantools
import can

import proto_autogen.f4dev_pb2
import proto_autogen.shared_pb2


_MANUFACTURER = "ubc_formula_electric"


def log_usb_devices():
    """Debug utility for printing all available usb devices."""
    devices = libusb_package.find(find_all=True)
    for device in devices:
        print(
            f"Product: {device.product},",
            f"Manufacturer: {device.manufacturer},",
            f"Product ID: {device.idProduct:#x},",
            f"Vendor ID: {device.idVendor:#x}",
        )


class CanDevice:
    def __init__(self, dbc_path: str, bus: can.BusABC):
        """Create an abstraction around a CAN bus device.

        Args:
            dbc_path: Path to a dbc file.
            bus: handle to a can bus.

        """
        self._db = cantools.database.load_file(dbc_path, database_format="dbc")
        self._can_bus = bus

        # Build CAN message tables.
        self.rx_table: Dict[str, Optional[Dict[str, any]]] = {}
        for msg in self._db.messages:
            self.rx_table[msg.name] = None

        # Spin up a loop on a seperate thread to constantly receive messages.
        def can_rx_loop():
            while True:
                raw_msg = self._can_bus.recv()
                name = self._db.get_message_by_frame_id(raw_msg.arbitration_id).name
                msg = self._db.decode_message(raw_msg.arbitration_id, raw_msg.data)
                self.rx_table[name] = msg

        self.can_rx_thread = threading.Thread(target=can_rx_loop)
        self.can_rx_thread.start()

    def get(self, msg_name: str, signal_name: str) -> Optional[any]:
        """Get a given signal from the last received can message.

        Args:
            msg_name: Name of the message.
            signal_name: Name of the signal.

        Returns:
            The value of the signal, or None of the message name has not been received.

        """

        if self.rx_table[msg_name] == None:
            return None
        return self.rx_table[msg_name][signal_name]

    def transmit(self, msg_name: str, data: Dict[str, any]):
        """Transmit a message given it's data.

        Args:
            msg_name: Name of the message.
            data: Dictonary containing the signals to send.

        """
        msg_type = self._db.get_message_by_name(msg_name)
        raw_data = msg_type.encode(data)
        raw_msg = can.Message(arbitration_id=msg_type.frame_id, data=raw_data)
        self._can_bus.send(raw_msg)


class _UsbDevice:
    def __init__(self, product: str):
        """Create a USB device abstraction, to talk to Communications-Device-Class (CDC) devices.

        Args:
            product: Name of the USB product, set in STM32CubeMX. See docs for more info.

        """

        self._device = libusb_package.find(manufacturer=_MANUFACTURER, product=product)

        # If the device was not found.
        if self._device is None:
            # Error out, and list all devices.
            print("Error: Specified USB device not found.")
            print("Devices found:")
            log_usb_devices()
            exit(1)

        self._interface = self._device[0][(1, 0)]
        self._endpoint_write = self._interface[0]
        self._endpoint_read = self._interface[1]
        self._read_chunk_size = self._endpoint_read.wMaxPacketSize

        # Buffer for read bytes.
        # We have to read in chunks of _read_chunk_size, so if we read too much,
        # this buffer stores the data for the next call.
        self._read_buf = b""

    def write(self, buffer: bytes):
        """Write bytes over usb.

        Args:
            buffer: Bytes to send over USB.

        """
        self._device.write(self._endpoint_write.bEndpointAddress, buffer)

    def read(self, length: int) -> bytes:
        """Read bytes over usb. Will block until all bytes are received.

        Args:
            length: Number of bytes to receive.

        Returns:
            Bytes received.

        """

        # Read bytes until there are a sufficent amount.
        while len(self._read_buf) < length:
            self._read_buf += bytes(
                self._device.read(
                    self._endpoint_read.bEndpointAddress,
                    self._read_chunk_size,
                    # Set a long timeout.
                    100000000,
                )
            )

        # Remove the first length bytes from the buffer.
        res = self._read_buf[:length]
        self._read_buf = self._read_buf[length:]
        return res


class _Board:
    # CHIMERA Packet Format:
    # [ length low byte  | length high byte | content bytes    | ... ]

    def __init__(
        self,
        usb_device: _UsbDevice,
        gpio_net_name: str,
        adc_net_name: str,
        board_module: types.ModuleType,
    ):
        """Create an abstration around a Chimera V2 board.

        Args:
            usb_device: Interface to the usb device.
            gpio_net_name: Identifier for the GpioNetName coresponding to your board, defined in shared.proto.
            adc_net_name: Identifier for the AdcNetName coresponding to your board, defined in shared.proto.
            board_module: Generated protobuf module, found in proto_autogen.

        """

        self._usb_device = usb_device
        self.gpio_net_name = gpio_net_name
        self.adc_net_name = adc_net_name
        self.board_module = board_module

    def _write(self, msg: proto_autogen.shared_pb2.DebugMessage):
        """Write a protobuf DebugMessage over the provided usb device.

        Args:
            msg: DebugMessage to send over USB.

        """

        # Get data.
        data = msg.SerializeToString()
        data_size = len(data)

        # Get low-endian bytes for packet.
        packet_size_bytes = [data_size & 0xFF, data_size >> 8]

        # Format and send packet.
        packet = bytes([*packet_size_bytes, *data])
        self._usb_device.write(packet)

    def _read(self) -> proto_autogen.shared_pb2.DebugMessage:
        """Read and return an protobuf DebugMessage over the provided usb device.

        Returns:
            The debug message received.

        """

        # Extract data size.
        data_size_bytes = self._usb_device.read(2)
        data_size = int.from_bytes(data_size_bytes, byteorder="little")

        # Extract rpc message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        msg.ParseFromString(self._usb_device.read(data_size))
        return msg

    def gpio_read(self, net_name: str) -> bool:
        """Read the value of a GPIO pin given the net name of the pin.

        Args:
            net_name: Name of the pin.

        Returns:
            True if the pin is set high, otherwise false.

        """

        # Create and send message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        net_name = self.board_module.GpioNetName.Value(net_name)
        setattr(msg.gpio_read.net_name, self.gpio_net_name, net_name)
        self._write(msg)

        # Wait for response.
        response = self._read()

        return response.gpio_read.value

    def gpio_write(self, net_name: str, value: bool) -> None:
        """Write a value to a GPIO pin.

        Args:
            net_name: Name of the pin.
            value: True if high, otherwise false.

        """

        # Create and send message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        net_name = self.board_module.GpioNetName.Value(net_name)
        setattr(msg.gpio_write.net_name, self.gpio_net_name, net_name)

        msg.gpio_write.value = value
        self._write(msg)

        # Wait for response, and validate that the request was the same.
        response = self._read()
        assert response.SerializeToString() == msg.SerializeToString()

    def adc_read(self, net_name: str) -> float:
        """Read the voltage at an ADC pin specified by the net name.

        Args:
            net_name: Name of the ADC pin.

        Returns:
            Voltage value read over pin.

        """

        # Create and send message.
        msg = proto_autogen.shared_pb2.DebugMessage()
        net_name = self.board_module.AdcNetName.Value(net_name)
        setattr(msg.adc.net_name, self.adc_net_name, net_name)
        self._write(msg)

        # Wait for response and return.
        response = self._read()
        return response.adc.value


class F4Dev(_Board):
    def __init__(self) -> None:
        """Create an interface to an F4Dev board."""

        super().__init__(
            usb_device=_UsbDevice(product="f4dev"),
            gpio_net_name="f4dev_net_name",
            adc_net_name="f4dev_net_name",
            board_module=proto_autogen.f4dev_pb2,
        )
