"""Debug UBC Formula Electric boards with Python.

Provides tooling to debug devices over USB, CAN, etc.

"""

# Typing.
from __future__ import annotations
from typing import Dict, Optional, Any
import types

# Threading.
import threading
import signal

# Peripherals.
import libusb_package
import cantools
import can

# Pyvisa Peripherals.
from load_bank import *
from power_supply import *

# Protobuf autogen packages.
import proto_autogen.f4dev_pb2
import proto_autogen.ssm_pb2
import proto_autogen.crit_pb2
import proto_autogen.shared_pb2
import proto_autogen.rsm_pb2
import proto_autogen.fsm_pb2

_MANUFACTURER = "ubc_formula_electric"

# Roughly 3 years.
_TIMEOUT_FOREVER_SECS = 100000000


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
        self.rx_table: Dict[str, Dict[str, Any]] = {}
        for msg in self._db.messages:  # type: ignore
            self.rx_table[msg.name] = {}
            for sig in msg.signals:
                self.rx_table[msg.name][sig.name] = None

        # Setup exit handler and signal.
        exit_event = threading.Event()

        def exit_handler(_signalnum: int, _stackframe: Any):
            exit_event.set()

        signal.signal(signal.SIGINT, exit_handler)

        # Spin up a loop on a seperate thread to constantly receive messages.
        def can_rx_loop():
            while True:
                raw_msg = self._can_bus.recv(0.1)
                if raw_msg is not None:
                    name = self._db.get_message_by_frame_id(raw_msg.arbitration_id).name
                    msg = self._db.decode_message(raw_msg.arbitration_id, raw_msg.data)
                    self.rx_table[name] = msg

                if exit_event.is_set():
                    break

        self.can_rx_thread = threading.Thread(target=can_rx_loop)
        self.can_rx_thread.start()

    def get(self, msg_name: str, signal_name: str) -> Optional[Any]:
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

    def transmit(self, msg_name: str, data: Dict[str, Any]):
        """Transmit a message given it's data.

        Args:
            msg_name: Name of the message.
            data: Dictonary containing the signals to send.

        """
        msg_type = self._db.get_message_by_name(msg_name)  # type: ignore
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

        # Read bytes until there are a sufficient amount.
        while len(self._read_buf) < length:
            self._read_buf += bytes(
                self._device.read(
                    self._endpoint_read.bEndpointAddress,
                    self._read_chunk_size,
                    _TIMEOUT_FOREVER_SECS,
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
            i2c_net_name: str,
            board_module: types.ModuleType,
    ):
        """Create an abstration around a Chimera V2 board.

        Args:
            usb_device: Interface to the usb device.
            gpio_net_name: Identifier for the GpioNetName corresponding to your board, defined in shared.proto.
            i2c_net_name: Identifier for the I2cNetName corresponding to your board, defined in shared.proto.
            adc_net_name: Identifier for the AdcNetName corresponding to your board, defined in shared.proto.
            board_module: Generated protobuf module, found in proto_autogen.

        """

        self._usb_device = usb_device
        self.gpio_net_name = gpio_net_name
        self.adc_net_name = adc_net_name
        self.i2c_net_name = i2c_net_name
        self.board_module = board_module

    def _write(self, msg: proto_autogen.shared_pb2.ChimeraV2Request):
        """Write a protobuf request to the provided usb device.

        Args:
            msg: request to send over USB.

        """

        # Get data.
        data = msg.SerializeToString()
        data_size = len(data)

        # Get low-endian bytes for packet.
        packet_size_bytes = [data_size & 0xFF, data_size >> 8]

        # Format and send packet.
        packet = bytes([*packet_size_bytes, *data])
        self._usb_device.write(packet)

    def _read(self) -> proto_autogen.shared_pb2.ChimeraV2Response:
        """Read and return a response from the usb device.

        Returns:
            The message received.

        """

        # Extract data size.
        data_size_bytes = self._usb_device.read(2)
        data_size = int.from_bytes(data_size_bytes, byteorder="little")

        # Extract rpc message.
        msg = proto_autogen.shared_pb2.ChimeraV2Response()
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
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(
            request.gpio_read.net_name,
            self.gpio_net_name,
            self.board_module.GpioNetName.Value(net_name),
        )
        self._write(request)

        # Wait for response.
        response = self._read()
        assert response.WhichOneof("payload") == "gpio_read"
        return response.gpio_read.value

    def gpio_write(self, net_name: str, value: bool) -> None:
        """Write a value to a GPIO pin.

        Args:
            net_name: Name of the pin.
            value: True if high, otherwise false.

        """

        # Create and send message.
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(
            request.gpio_write.net_name,
            self.gpio_net_name,
            self.board_module.GpioNetName.Value(net_name),
        )
        request.gpio_write.value = value
        self._write(request)

        # Wait for validation response.
        response = self._read()
        assert response.WhichOneof("payload") == "gpio_write"
        assert response.gpio_write.success

    def adc_read(self, net_name: str) -> float:
        """Read the voltage at an ADC pin specified by the net name.

        Args:
            net_name: Name of the ADC pin.

        Returns:
            Voltage value read over pin.

        """

        # Create and send message.
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(
            request.adc_read.net_name,
            self.adc_net_name,
            self.board_module.AdcNetName.Value(net_name),
        )
        self._write(request)

        # Wait for response and return.
        response = self._read()
        assert response.WhichOneof("payload") == "adc_read"
        return response.adc_read.value

    def i2c_device(self, net_name: str) -> I2cDevice:
        """Create an abstraction around an I2c device.

        Args:
            net_name: Identifier of the I2C device.

        Returns:
            An I2C device abstraction.

        """
        return I2cDevice(self, net_name)


class I2cDevice:
    def __init__(self, owner: _Board, net_name: str):
        """Create an abstraction around an I2c device.

        This constructor should NEVER be called on its own,
        instead create I2c devices via a board's ``i2c_device`` method.

        Args:
            owner: Owner board.
            net_name: Identifier of the I2C device.

        """

        self._owner = owner
        self._net_name = self._owner.board_module.I2cNetName.Value(net_name)

    def is_ready(self) -> bool:
        """Check if an i2c device is ready.

        Returns:
            True if the device is ready, otherwise false.

        """

        # Create and send message.
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(request.i2c_ready.net_name, self._owner.i2c_net_name, self._net_name)
        self._owner._write(request)

        # Wait for response.
        response = self._owner._read()
        assert response.WhichOneof("payload") == "i2c_ready"
        return response.i2c_ready.ready

    def receive(self, length: int) -> bytes:
        """Receive bytes from the I2C device.

        Args:
            length: number of bytes to receive.

        Returns:
            Bytes received.

        """

        # Create and send message.
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(request.i2c_receive.net_name, self._owner.i2c_net_name, self._net_name)
        request.i2c_receive.length = length

        self._owner._write(request)

        # Wait for response.
        response = self._owner._read()
        assert response.WhichOneof("payload") == "i2c_receive"
        return response.i2c_receive.data

    def transmit(self, data: bytes):
        """Transmit bytes to the I2C device.

        Args:
            data: Bytes to transmit.

        """

        # Create and send message.
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(request.i2c_transmit.net_name, self._owner.i2c_net_name, self._net_name)
        request.i2c_transmit.data = data

        self._owner._write(request)

        # Wait for response.
        response = self._owner._read()
        assert response.WhichOneof("payload") == "i2c_transmit"
        assert response.i2c_transmit.success

    def memory_read(self, memory_address: int, length: int) -> bytes:
        """Read bytes from I2C device memory.

        Args:
            memory_address: Address of memory to be read.
            length: Number of bytes to read.

        Returns:
            Bytes read.

        """

        # Create and send message.
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(
            request.i2c_memory_read.net_name, self._owner.i2c_net_name, self._net_name
        )
        request.i2c_memory_read.memory_address = memory_address
        request.i2c_memory_read.length = length

        self._owner._write(request)

        # Wait for response.
        response = self._owner._read()
        assert response.WhichOneof("payload") == "i2c_memory_read"
        return response.i2c_memory_read.data

    def memory_write(self, memory_address: int, data: bytes):
        """Read bytes into I2C device memory.

        Args:
            memory_address: Address of memory to be write to.
            data: Bytes to write.

        """

        # Create and send message.
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(
            request.i2c_memory_write.net_name, self._owner.i2c_net_name, self._net_name
        )
        request.i2c_memory_write.memory_address = memory_address
        request.i2c_memory_write.data = data

        self._owner._write(request)

        # Wait for response.
        response = self._owner._read()
        assert response.WhichOneof("payload") == "i2c_memory_write"
        assert response.i2c_memory_write.success


class F4Dev(_Board):
    def __init__(self) -> None:
        """Create an interface to an F4Dev board."""

        super().__init__(
            usb_device=_UsbDevice(product="f4dev"),
            gpio_net_name="f4dev_net_name",
            adc_net_name="f4dev_net_name",
            i2c_net_name="f4dev_net_name",
            board_module=proto_autogen.f4dev_pb2,
        )


class SSM(_Board):
    def __init__(self) -> None:
        """Create an interface to an SSM board."""

        super().__init__(
            usb_device=_UsbDevice(product="ssm"),
            gpio_net_name="ssm_net_name",
            adc_net_name="ssm_net_name",
            i2c_net_name="ssm_net_name",
            board_module=proto_autogen.ssm_pb2,
        )


class CRIT(_Board):
    def __init__(self) -> None:
        """Create an interface to a CRIT/cDIM board."""

        super().__init__(
            usb_device=_UsbDevice(product="crit"),
            gpio_net_name="crit_net_name",
            adc_net_name="crit_net_name",
            i2c_net_name="crit_net_name",
            board_module=proto_autogen.crit_pb2,
        )

class RSM(_Board):
    def __init__(self) -> None:
        """Create an interface to a RSM board."""

        super().__init__(
            usb_device=_UsbDevice(product="rsm"),
            gpio_net_name="rsm_net_name",
            adc_net_name="rsm_net_name",
            i2c_net_name="rsm_net_name",
            board_module=proto_autogen.rsm_pb2,
        )

class FSM(_Board):
    def __init__(self) -> None:
        """Create an interface to a FSM board."""

        super().__init__(
            usb_device=_UsbDevice(product="fsm"),
            gpio_net_name="fsm_net_name",
            adc_net_name="fsm_net_name",
            i2c_net_name="fsm_net_name",
            board_module=proto_autogen.fsm_pb2,
        )
