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
from live_data.app.generated import telem_pb2
from logger import logger
from middleware.serial_port import get_serial
from tasks.broadcaster import CanMsg, can_msg_queue
from google.protobuf.message import DecodeError

HEADER_SIZE = 7
MAGIC = b"\xaa\x55"

MAX_PAYLOAD_SIZE = 52  # this is arbitrary lmao


def find_magic_in_buffer(buffer, magic=MAGIC):  # do i need to set magic here
    """
    Return the index of the first occurrence of magic bytes in the buffer, or -1 if not found.
    """
    magic_len = len(magic)
    for i in range(len(buffer) - magic_len + 1):
        if buffer[i: i + magic_len] == magic:
            return i
    return -1


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


def read_packet(ser: serial.Serial):
    buffer = bytearray()
    while True:
        # data = b'1'
        data = ser.read(1)
        # print(data)
        if not data:
            continue
        buffer += data

        # wait for full header
        if len(buffer) < HEADER_SIZE:
            continue

        if buffer[0:2] != MAGIC:
            magic_index = find_magic_in_buffer(buffer.hex(), MAGIC)
            if magic_index == -1:
                buffer.clear()
            else:
                buffer = buffer[magic_index:]
            continue

        if len(buffer) < HEADER_SIZE:
            continue

        # parse remainder of header
        payload_length = buffer[2]
        expected_crc = (
            int.from_bytes(buffer[3:7], byteorder="big")
            .to_bytes(4, byteorder="big")
            .hex()
        )  # Updated to read 32-bit CRC and cast to hex

        if payload_length > MAX_PAYLOAD_SIZE:
            logger.error(f"Payload length {payload_length} is too large")
            buffer = buffer[1:]
            continue

        total_length = HEADER_SIZE + payload_length
        if len(buffer) < total_length:
            continue  # wait for full packet

        packet = bytes(buffer[:total_length])

        # reset buffer
        buffer = buffer[total_length:]

        return packet, payload_length, expected_crc

@dataclass
class TelemetryMessage:
    can_id: int
    payload: bytes
    timestamp: datetime.datetime

def parse_telem_message(payload: bytes) -> Optional[TelemetryMessage]:
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
        packet, payload_length, expected_crc = read_packet(ser)
        payload = packet[HEADER_SIZE:]
        if len(payload) != payload_length:
            logger.error(
                f"Payload length mismatch: expected {payload_length}, got {len(payload)}"
            )
            continue

        # CRC check
        # this is very interesting because CRC32 is an object which is of the correct type
        calculator = Calculator(Crc32.CRC32) # type: ignore
        calculated_checksum = (
            calculator.checksum(payload).to_bytes(4, byteorder="big").hex()
        )
        if expected_crc != calculated_checksum:
            logger.error(
                f"CRC mismatch: computed ",
                calculated_checksum,
                "expected ",
                expected_crc,
            )
            continue

        message_received = parse_telem_message(payload)
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
