"""
Note: make sure to keep this file updated with the firmware
"""
from dataclasses import dataclass
import datetime
from threading import Thread
from typing import Optional
import serial
from crc import Calculator, Crc32

# ours
from generated import telem_pb2
from logger import logger
from middleware.serial_port import get_serial
from tasks.broadcaster import CanMsg, can_msg_queue
from google.protobuf.message import DecodeError

HEADER_SIZE = 7
MAGIC = b"\xaa\x55"

MAX_PAYLOAD_SIZE = 52  # this is arbitrary lmao

def calculate_message_timestamp(message_timestamp, base_time):
    if message_timestamp > 0:
        # Convert milliseconds to seconds and create a timedelta
        delta = datetime.timedelta(milliseconds=message_timestamp)
        timestamp = base_time + delta
        # logger.debug(
        #     f"Message timestamp: {message_timestamp}ms, calculated time: {timestamp}"
        # )
    else:
        timestamp = datetime.datetime.now()
        logger.warning(
            f"Invalid timestamp received: {message_timestamp}, using current time"
        )

    return timestamp

# https://pyserial.readthedocs.io/en/latest/pyserial_api.html#:~:text=The%20network%20layer%20also%20has%20buffers.%20This%20means%20that%20flush()%2C%20reset_input_buffer()%20and%20reset_output_buffer()%20may%20work%20with%20additional%20delay.%20Likewise%20in_waiting%20returns%20the%20size%20of%20the%20data%20arrived%20at%20the%20objects%20internal%20buffer%20and%20excludes%20any%20bytes%20in%20the%20network%20buffers%20or%20any%20server%20side%20buffer.
# TLDR is that there are internal buffers for read/write, so we won't drop packets
def _read_packet(ser: serial.Serial):
    """
    Read a packet from the serial port.
    Packets come in the following format:
    |    Magic    |            Header (5 bytes)             |
    | 0xAA | 0x55 | payload_size (1 byte) | CRC32 (4 bytes) | payload (length of payload_size) |
    Returns (ONLY) the payload as bytes.
    """
    buffer = bytearray()
    while True:
        # merk the first byte automatically
        # this is because when we come around it generally is because it was wrong
        buffer = buffer[1:]

        # find the magic bytes
        while buffer[0:2] != MAGIC:
            buffer = buffer [1:]  # remove the first byte if it is not magic
            if len(buffer) < 2: buffer += ser.read(HEADER_SIZE - len(buffer))
            # after this read, len(buffer) == HEADER_SIZE
            # this optimization allows us to not read the serial port less
            # while remaining under the maximum amount of bytes we are allowed to keep
        assert buffer[0:2] == MAGIC, "Buffer should contain the magic bytes"

        # wait for full header
        if len(buffer) < HEADER_SIZE: buffer += ser.read(HEADER_SIZE - len(buffer))
        assert len(buffer) >= HEADER_SIZE, "Buffer should contain the full header of 7 bytes"

        # parse remainder of header
        payload_length = buffer[2]
        if payload_length > MAX_PAYLOAD_SIZE:
            # note this means that magic is found, but the payload length is invalid
            # this is a good protection to make sure we don't overread the buffer and have to drop bytes
            logger.error(f"Payload length {payload_length} is too large")
            continue # restart


		# after this point, a supposed payload will be read into the buffer
        total_length = HEADER_SIZE + payload_length
        if len(buffer) < total_length:
            buffer += ser.read(total_length - len(buffer))  # read the rest of the packet
        assert len(buffer) >= total_length, "Buffer should contain the full packet"

        # this is very interesting because CRC32 is an object which is of the correct type
        calculator = Calculator(Crc32.CRC32) # type: ignore

        # CRC check
        payload = bytes(buffer[HEADER_SIZE:])
        calculated_checksum = calculator.checksum(payload).to_bytes(4, byteorder="big").hex()
        expected_crc = (
            int.from_bytes(buffer[3:7], byteorder="big")
            .to_bytes(4, byteorder="big")
            .hex()
        )  # Updated to read 32-bit CRC and cast to hex
        if expected_crc != calculated_checksum:
            logger.error(f"CRC mismatch: computed {calculated_checksum}, expected {expected_crc}")
            continue # restart

        if len(buffer) > total_length:
            # NOTE that this is possible because payload_length could be unreliable
            # and we could have read more bytes than expected
            logger.warning(f"Buffer has more data than expected, {len(buffer) - total_length} bytes will be dropped")
            # TODO add some mechanism of carrying over the (currently dropped) bytes
            buffer = buffer[total_length:]

        return payload

@dataclass
class TelemetryMessage:
    can_id: int
    payload: bytes
    timestamp: datetime.datetime

def _parse_telem_message(payload: bytes) -> Optional[TelemetryMessage]:
    """
    We contain all the nonsense protobuf disgusting types here
    and make a nice interface for the rest of the code.
    """
    try:
        message_received = telem_pb2.TelemMessage() # type: ignore
        message_received.ParseFromString(payload) # type: ignore
    except DecodeError as e:
        logger.error(f"Failed to parse telemetry message: {e}")
        return None
    
    return TelemetryMessage(
        can_id=message_received.can_id, # type: ignore
        payload=bytearray(
            [
                message_received.message_0, # type: ignore
                message_received.message_1, # type: ignore
                message_received.message_2, # type: ignore
                message_received.message_3, # type: ignore
                message_received.message_4, # type: ignore
                message_received.message_5, # type: ignore
                message_received.message_6, # type: ignore
                message_received.message_7, # type: ignore
            ]
        ),
        timestamp=message_received.time_stamp # type: ignore
    )

def _read_messages(port: str):
    """
    Read messages coming in through the serial port, decode them, unpack them and then emit them to the socket
    """
    ser = get_serial(port)

    base_time = None

    while True:
        payload = _read_packet(ser)

        message_received = _parse_telem_message(payload)
        if message_received is None:
            continue

        # decode for start_time messages, don't push this to the queue
        if message_received.can_id == 0x999:
            # parse start_time from data if this pacakage is correct
            # print(message_received)
            base_time = datetime.datetime(
                year=message_received.payload[0]
                + 2000,  # need to offset this as on firmware side it is 0-99
                month=message_received.payload[1],
                day=message_received.payload[2],
                hour=message_received.payload[3],
                minute=message_received.payload[4],
                second=message_received.payload[5],
            ).astimezone(datetime.timezone.utc)
            logger.info(f"Base time recieved: {base_time}")
            continue

        if not base_time:
            # we do not know the base time so skip
            print(f"get message {message_received.can_id} but no base time")
            continue
        # print(CanMsg(message_received.can_id, _make_bytes(message_received), base_time))
        timestamp = calculate_message_timestamp(
            message_received.timestamp, base_time)
        can_msg_queue.put(
            CanMsg(message_received.can_id, message_received.payload, timestamp)
        )


def get_wireless_task(serial_port: str | None) -> Thread:
    if serial_port is None:
        raise RuntimeError(
            "If running telemetry in wireless mode, you must specify the radio serial port!"
        )
    return Thread(
        target=_read_messages,
        args=(serial_port,),
        daemon=True,
    )
