"""
Note: make sure to keep this file updated with the firmware
"""
from dataclasses import dataclass
from datetime import datetime, timedelta
import struct
from threading import Thread
from typing import Optional, Union
import serial
from crc import Calculator, Crc32
from ntplib import ntp_to_system_time

# ours
from tasks.stop_signal import should_run
from logger import logger
from middleware.serial_port import get_serial
from tasks.broadcaster import CanMsg, can_msg_queue
from settings import SERIAL_PORT
from enum import Enum

from ntp import ntp_request

_HEADER_SIZE = 7
_MAGIC = b"\xaa\x55"
_MAX_PAYLOAD_SIZE = 100  # this is arbitrary lmao
# this is very interesting because CRC32 is an object which is of the correct type
_CRC_CALCULATOR = Calculator(Crc32.CRC32) # type: ignore


def _calculate_message_timestamp(message_timestamp, base_time):
    if message_timestamp > 0:
        # Convert milliseconds to seconds and create a timedelta
        delta = timedelta(milliseconds=message_timestamp)
        timestamp = base_time + delta
        # logger.debug(
        #     f"Message timestamp: {message_timestamp}ms, calculated time: {timestamp}"
        # )
    else:
        timestamp = datetime.now()
        logger.warning(
            f"Invalid timestamp received: {message_timestamp}, using current time"
        )

    return timestamp


buffer = bytearray()
# https://pyserial.readthedocs.io/en/latest/pyserial_api.html#:~:text=The%20network%20layer%20also%20has%20buffers.%20This%20means%20that%20flush()%2C%20reset_input_buffer()%20and%20reset_output_buffer()%20may%20work%20with%20additional%20delay.%20Likewise%20in_waiting%20returns%20the%20size%20of%20the%20data%20arrived%20at%20the%20objects%20internal%20buffer%20and%20excludes%20any%20bytes%20in%20the%20network%20buffers%20or%20any%20server%20side%20buffer.
# TLDR is that there are internal buffers for read/write, so we won't drop packets
def _read_packet(ser: serial.Serial):
    global buffer
    """
    Read a packet from the serial port.
    Packets come in the following format:
    |    Magic    |            Header (5 bytes)             |
    | 0xAA | 0x55 | payload_size (1 byte) | CRC32 (4 bytes) | payload (length of payload_size) |
    Returns (ONLY) the payload as bytes.
    """
    first = True
    while True:
        if first:
            first = False
        else:
            # merk the first byte automatically
            # this is because when we come around it generally is because it was wrong
            buffer = buffer[1:]
        
        # find the magic bytes
        while buffer[0:2] != _MAGIC:
            buffer = buffer [1:]  # remove the first byte if it is not magic
            if len(buffer) < 2: buffer += ser.read(_HEADER_SIZE - len(buffer))
            # after this read, len(buffer) == HEADER_SIZE
            # this optimization allows us to not read the serial port less
            # while remaining under the maximum amount of bytes we are allowed to keep
        assert buffer[0:2] == _MAGIC, "Buffer should contain the magic bytes"

        # wait for full header
        if len(buffer) < _HEADER_SIZE: buffer += ser.read(_HEADER_SIZE - len(buffer))
        assert len(buffer) >= _HEADER_SIZE, "Buffer should contain the full header of 7 bytes"

        # parse remainder of header
        payload_length = buffer[2]
        if payload_length > _MAX_PAYLOAD_SIZE:
            # note this means that magic is found, but the payload length is invalid
            # this is a good protection to make sure we don't overread the buffer and have to drop bytes
            logger.error(f"Payload length {payload_length} is too large")
            continue # restart


		# after this point, a supposed payload will be read into the buffer
        total_length = _HEADER_SIZE + payload_length
        if len(buffer) < total_length:
            buffer += ser.read(total_length - len(buffer))  # read the rest of the packet
        assert len(buffer) >= total_length, f"Buffer should contain at least {total_length} bytes"
 
        # CRC check
        payload = bytes(buffer[_HEADER_SIZE:total_length]) # extract the payload
        calculated_checksum = _CRC_CALCULATOR.checksum(payload).to_bytes(4, byteorder="big").hex()
        expected_crc = (
            int.from_bytes(buffer[3:7], byteorder="little")
            .to_bytes(4, byteorder="big")
            .hex()
        )  # Updated to read 32-bit CRC and cast to hex
        if expected_crc != calculated_checksum:
            logger.error(f"CRC mismatch: computed {calculated_checksum}, expected {expected_crc}")
            continue # restart

        # NOTE that this is possible because payload_length could be unreliable
        # and we could have read more bytes than expected
        # logger.warning(f"Buffer has more data than expected, {len(buffer) - total_length} bytes will be dropped")
        buffer = buffer[total_length:]

        #logger.debug("POGGERS!!!!!")
        return payload

@dataclass
class TelemetryMessage:
    pass

@dataclass
class CanMessage(TelemetryMessage):
    can_id: int
    can_time_offset: float
    can_payload: bytes

@dataclass
class NTPDateMessage(TelemetryMessage):
    pass

@dataclass
class NTPTimeMessage(TelemetryMessage):
    id: int

@dataclass
class BaseTimeRegMessage(TelemetryMessage):
    date_payload: datetime

class TelemetryMessageType(Enum):
    CAN = 0x01
    NTP = 0x02
    NTPDate = 0x03
    BaseTimeReg = 0x04

# After packet check has been done, parse the telem message received from the car
def _parse_telem_message(payload: bytes) -> Optional[TelemetryMessage]:
    """
    Converts the payload into a TelemetryMessage which is nice to handle
    """
    match payload[0]:
        case TelemetryMessageType.CAN.value:
            if len(payload) < 9:
                return None # Not enough data for CAN message
            return CanMessage(
                can_id=struct.unpack('<I', payload[1:5])[0],
                can_time_offset=struct.unpack('<f', payload[5:9])[0], 
                can_payload=payload[9:],
            )
        case TelemetryMessageType.NTP.value:
            if len(payload) < 2:
                return None # Not enough data for NTP message
            return NTPTimeMessage(id=payload[1])
        case TelemetryMessageType.NTPDate.value:
            return NTPDateMessage()
        case TelemetryMessageType.BaseTimeReg.value:
            if len(payload) < 11:
                return None # Not enough data for BaseTimeReg message
            return BaseTimeRegMessage(datetime(
                year=int(payload[1]) + 2000,  # need to offset this as on firmware side it is 0-99
                month=int(payload[2]),
                day=int(payload[3]),
                hour=int(payload[4]),
                minute=int(payload[5]),
                second=int(payload[6]),
                microsecond=struct.unpack('<I', payload[7:11])[0]
            ))
        case _:
            return None

def _read_messages():
    """
    Read messages coming in through the serial port, decode them, unpack them and then emit them to the socket
    """
    if SERIAL_PORT is None:
        raise RuntimeError("SERIAL_PORT is not set, cannot read messages")
    ser = get_serial(SERIAL_PORT)
    logger.debug("Read messages thread started.")

    base_time: datetime = None

    while should_run():
        payload = _read_packet(ser)

        message_received: TelemetryMessage | None = _parse_telem_message(payload)
        if message_received is None:
            logger.error("Failed to parse telemetry message, skipping")
            continue

        match message_received:
            case CanMessage(can_id, can_time_offset, can_payload):
                if not base_time:
                    # we do not know the base time so skip
                    logger.error(f"Got message {can_id} but no base time, hence will not be pushed into queue")
                    continue
                timestamp = _calculate_message_timestamp(can_time_offset, base_time)
                # Handle CAN message
                can_msg_queue.put(CanMsg(
                    can_id=can_id,
                    can_timestamp=timestamp,
                    can_value=can_payload
                ))
            case NTPTimeMessage(id):
                print(f"ntp received {id}")
                # Handle NTP message
                t1, t2 = ntp_request(0)

                def pack_ntp(t: float) -> bytes:
                    dt = datetime.fromtimestamp(ntp_to_system_time(t), datetime.timezone.utc)
                    return struct.pack('<BBB', dt.hour, dt.minute, dt.second) + struct.pack('<I', dt.microsecond)

                payload = pack_ntp(t1) + pack_ntp(t2)
                ser.write(payload)
            case NTPDateMessage():
                # Handle NTPDate message
                today = datetime.now(datetime.timezone.utc)
                year, month, date, day = today.year, today.month, today.day, today.weekday()
                payload = struct.pack('<BBBB', year - 2000, month, date, day)
                ser.write(payload)
            case BaseTimeRegMessage(date_payload):
                # Handle BaseTimeReg message
                # Note that it should already be in UTC when received
                base_time = date_payload 
                logger.info(f"Base time received: {base_time}")
            case _:
                logger.error(f"Unknown message type: {type(message_received.payload)}")
    logger.debug("Read messages thread stopped.")


def get_wireless_task(sio) -> Thread:
    return sio.start_background_task(_read_messages)