"""Debug UBC Formula Electric boards with Python.

Provides tooling to debug devices over USB.

"""

# Typing.
from __future__ import annotations
import types

# Peripherals.
import libusb_package

# Protobuf autogen packages.
import proto_autogen.shared_pb2
import proto_autogen.f4dev_pb2
import proto_autogen.fsm_pb2
import proto_autogen.rsm_pb2
import proto_autogen.ssm_pb2
import proto_autogen.crit_pb2
import proto_autogen.dam_pb2

# Pyvisa Peripherals.
from load_bank import *
from power_supply import *
from scope import *

# Protobuf autogen packages.
import proto_autogen.shared_pb2

import proto_autogen.f4dev_pb2
import proto_autogen.ssm_pb2
import proto_autogen.crit_pb2
import proto_autogen.bms_pb2
import proto_autogen.rsm_pb2
import proto_autogen.fsm_pb2
import proto_autogen.vc_pb2

# USB Manufacturer ID, specified per-board in STM32 CubeMX.
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
        self._write_chunk_size = self._endpoint_write.wMaxPacketSize

        # Buffer for read bytes.
        # We have to read in chunks of _read_chunk_size, so if we read too much,
        # this buffer stores the data for the next call.
        self._read_buf = b""

    def write(self, buffer: bytes):
        """Write bytes over usb.

        Args:
            buffer: Bytes to send over USB.

        """

        # Chunk to maximum size accepted by endpoint before writing.
        for index in range(0, len(buffer), self._write_chunk_size):
            chunk = buffer[index : index + self._write_chunk_size]
            self._device.write(self._endpoint_write.bEndpointAddress, chunk)

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
        self, usb_device: _UsbDevice, net_name_tag: str, board_module: types.ModuleType
    ):
        """Create an abstration around a Chimera V2 board.

        Args:
            usb_device: Interface to the usb device.
            net_name_tag:
                Identifier for the net name corresponding to your board.
                We expect this to be the same for every peripheral. Defined in ``shared.proto``.
            board_module: Generated protobuf module, found in proto_autogen.

        """

        self._usb_device = usb_device
        self._net_name_tag = net_name_tag
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
            self._net_name_tag,
            self.board_module.GpioNetName.Value(net_name),
        )
        self._write(request)

        # Wait for response.
        response = self._read()
        assert response.WhichOneof("payload") == "gpio_read"
        return response.gpio_read.value

    def gpio_write(self, net_name: str, value: bool):
        """Write a value to a GPIO pin.

        Args:
            net_name: Name of the pin.
            value: True if high, otherwise false.

        """

        # Create and send message.
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(
            request.gpio_write.net_name,
            self._net_name_tag,
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
            self._net_name_tag,
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

    def spi_device(self, net_name: str) -> SpiDevice:
        """Create an abstraction around a SPI device.

        Args:
            net_name: Identifier of the SPI device.

        Returns:
            An SPI device abstraction.

        """
        return SpiDevice(self, net_name)
    
    def pwm_input(self, net_name: str) -> SpiDevice:
        """Read incoming PWM signal's frequency and duty cycle.

        Args:
            net_name: Identifier of the PWM sender.

        Returns:
            Frequency and duty cycle values.

        """
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(
            request.pwm_read.net_name,
            self._net_name_tag,
            self.board_module.PwmNetName.Value(net_name),
        )
        self._write(request)

        # Wait for response.
        response = self._read()
        assert response.WhichOneof("payload") == "pwm_full_input"
        return {"freq_hz":response.pwm_full_input.frequency_hz, "duty_cycle": response.pwn_full_input.duty_cycle}
    
    def pwm_input_freq_only(self, net_name: str) -> SpiDevice:
        """Read incoming PWM signal's frequency only.

        Args:
            net_name: Identifier of the PWM sender.

        Returns:
            Only the frequency value of the PWM signal.

        """
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(
            request.pwm_read.net_name,
            self._net_name_tag,
            self.board_module.PwmNetName.Value(net_name),
        )
        self._write(request)

        # Wait for response.
        response = self._read()
        assert response.WhichOneof("payload") == "pwm_freq_only_input"
        return {"freq_hz":response.pwm_full_input.frequency_hz}
    
    def pwm_output(self, net_name: str, duty_cycle: float, frequency_hz: float) -> SpiDevice:
        """Send a PWM signal to a designated board with specific timer channel frequency and desired duty cycle.

        Args:
            net_name: Identifier of the PWM receiver.
        """
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(
            request.pwm_read.net_name,
            self._net_name_tag,
            self.board_module.PwmNetName.Value(net_name),
        )
        self._write(request)

        # Wait for response.
        response = self._read()
        assert response.WhichOneof("payload") == "pwm_full_output"
        return {"freq_hz":response.pwm_full_output.frequency_hz}

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
        setattr(request.i2c_ready.net_name, self._owner._net_name_tag, self._net_name)
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
        setattr(request.i2c_receive.net_name, self._owner._net_name_tag, self._net_name)
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
        setattr(
            request.i2c_transmit.net_name, self._owner._net_name_tag, self._net_name
        )
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
            request.i2c_memory_read.net_name, self._owner._net_name_tag, self._net_name
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
            request.i2c_memory_write.net_name, self._owner._net_name_tag, self._net_name
        )
        request.i2c_memory_write.memory_address = memory_address
        request.i2c_memory_write.data = data

        self._owner._write(request)

        # Wait for response.
        response = self._owner._read()
        assert response.WhichOneof("payload") == "i2c_memory_write"
        assert response.i2c_memory_write.success


class SpiDevice:
    def __init__(self, owner: _Board, net_name: str):
        """Create an abstraction around an SPI device.

        This constructor should NEVER be called on its own,
        instead create SPI devices via a board's ``spi_device`` method.

        Args:
            owner: Owner board.
            net_name: Identifier of the SPI device.

        """

        self._owner = owner
        self._net_name = self._owner.board_module.SpiNetName.Value(net_name)

    def receive(self, length: int) -> bytes:
        """Receive bytes from the SPI device.

        Args:
            length: Number of bytes to receive.

        Returns:
            Bytes received.

        """

        # Create and send message.
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(request.spi_receive.net_name, self._owner._net_name_tag, self._net_name)
        request.spi_receive.length = length

        self._owner._write(request)

        # Wait for response.
        response = self._owner._read()
        assert response.WhichOneof("payload") == "spi_receive"
        return response.spi_receive.data

    def transmit(self, data: bytes):
        """Transmit bytes to the SPI device.

        Args:
            data: Bytes to transmit.

        """

        # Create and send message.
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(
            request.spi_transmit.net_name, self._owner._net_name_tag, self._net_name
        )
        request.spi_transmit.data = data

        self._owner._write(request)

        # Wait for response.
        response = self._owner._read()
        assert response.WhichOneof("payload") == "spi_transmit"
        assert response.spi_transmit.success

    def transact(self, request_data: bytes, response_length: int) -> bytes:
        """Run a full transaction (tx/rx) to the SPI device.

        Args:
            request_data: Bytes to transmit.
            response_length: Number of bytes to read.

        Returns:
            Bytes received.

        """

        # Create and send message.
        request = proto_autogen.shared_pb2.ChimeraV2Request()
        setattr(
            request.spi_transaction.net_name, self._owner._net_name_tag, self._net_name
        )
        request.spi_transaction.tx_data = request_data
        request.spi_transaction.rx_length = response_length

        self._owner._write(request)

        # Wait for response.
        response = self._owner._read()
        assert response.WhichOneof("payload") == "spi_transaction"
        return response.spi_transaction.rx_data


class F4Dev(_Board):
    def __init__(self):
        """Create an interface to an F4Dev board."""

        super().__init__(
            usb_device=_UsbDevice(product="f4dev"),
            net_name_tag="f4dev_net_name",
            board_module=proto_autogen.f4dev_pb2,
        )


class SSM(_Board):
    def __init__(self):
        """Create an interface to an SSM board."""

        super().__init__(
            usb_device=_UsbDevice(product="ssm"),
            net_name_tag="ssm_net_name",
            board_module=proto_autogen.ssm_pb2,
        )


class CRIT(_Board):
    def __init__(self):
        """Create an interface to a CRIT/cDIM board."""

        super().__init__(
            usb_device=_UsbDevice(product="crit"),
            net_name_tag="crit_net_name",
            board_module=proto_autogen.crit_pb2,
        )


class BMS(_Board):
    def __init__(self):
        """Create an interface to a BMS board."""

        super().__init__(
            usb_device=_UsbDevice(product="bms"),
            net_name_tag="bms_net_name",
            board_module=proto_autogen.bms_pb2,
        )


class RSM(_Board):
    def __init__(self):
        """Create an interface to a RSM board."""

        super().__init__(
            usb_device=_UsbDevice(product="rsm"),
            net_name_tag="rsm_net_name",
            board_module=proto_autogen.rsm_pb2,
        )


class FSM(_Board):
    def __init__(self):
        """Create an interface to a FSM board."""

        super().__init__(
            usb_device=_UsbDevice(product="fsm"),
            net_name_tag="fsm_net_name",
            board_module=proto_autogen.fsm_pb2,
        )


class DAM(_Board):
    def __init__(self):
        """Create an interface to a DAM board."""

        super().__init__(
            usb_device=_UsbDevice(product="dam"),
            net_name_tag="dam_net_name",
            board_module=proto_autogen.dam_pb2,
        )


class VC(_Board):
    def __init__(self):
        """Create an interface to a VC board."""

        super().__init__(
            usb_device=_UsbDevice(product="vc"),
            net_name_tag="vc_net_name",
            board_module=proto_autogen.vc_pb2,
        )
