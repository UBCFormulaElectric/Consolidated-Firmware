# TODO FINISH
# import time
# from tzlocal import get_localzone

from threading import Thread
import serial
from generated import telem_pb2
from logger import logger
from tasks.broadcaster import can_msg_queue, CanMsg
import datetime
import struct

HEADER_FORMAT = ">2BBH"
HEADER_SIZE = 5
MAGIC = (0xAA, 0x55)

def _make_bytes(message):
	"""
	Make the byte array out of the messages array.
	"""
	return bytearray([
		message.message_0, message.message_1, message.message_2, 
		message.message_3, message.message_4, message.message_5, 
		message.message_6, message.message_7
	]) 

def _read_messages(port: str):
	"""
	Read messages coming in through the serial port, decode them, unpack them and then emit them to the socket
	"""

	ser = serial.Serial(port=port, baudrate=57600)
	ser.reset_input_buffer()
	ser.reset_output_buffer()
	expected_magic = bytes(MAGIC)
	while True:
		
		# Magic Number
		magic_bytes = ser.read(2)
		if len(magic_bytes) < 2:
			logger.error("Did not receive enough bytes for magic number")
			continue

		if magic_bytes != expected_magic:
			#nonvalid magic number
			logger.error("Magic Number did not match")
			continue

		# Payload Length
		payload_length_byte = ser.read(1)
		if not payload_length_byte:
			logger.error("Error Reading Payload Length")
			continue
		payload_length = int.from_bytes(payload_length_byte, byteorder='big')

		# CRC check
		crc_bytes = ser.read(2)
		if len(crc_bytes) < 2:
			logger.error("Did not receive enough bytes for CRC")
			continue
		expected_crc = int.from_bytes(crc_bytes, byteorder='big')
		

		header_bytes = len(magic_bytes) + len(payload_length_byte) + len(crc_bytes)

		# if len(header_bytes) != HEADER_SIZE:
		# 	logger.error("Received header of incorrect size")
		# 	continue

		# try:
		# 	#store for later
		# 	magic_high, magic_low, payload_length, expected_crc = struct.unpack(HEADER_FORMAT, header_bytes)
		# except struct.error as e:
		# 	logger.error("Header unpacking error: %s", e)
		# 	logger.info(f"Received data of size {payload_length + len(header_bytes)} bytes")
		# 	continue
		
		logger.info(f"Received data of size {payload_length + len(header_bytes)} bytes")
		
		#read the payload length given
		payload = ser.read(payload_length)
		if len(payload) < payload_length:
			logger.error("Incomplete payload received")
			continue

        # Compute CRC on the payload ONLY and validate it
        #computed_crc = crc_FUNC_TODO(payload)

		computed_crc = 0xFFFF #HARDCODED
		if computed_crc != expected_crc:
			logger.error(f"CRC mismatch: computed {computed_crc:04X}, expected {expected_crc:04X}")
			continue

		# Decode protobuf message
		try:
			message_received = telem_pb2.TelemMessage()
			message_received.ParseFromString(payload)
		except Exception as e:
			logger.error(f"Error decoding protobuf message: {e}")
			continue
		can_msg_queue.put(CanMsg(message_received.can_id, _make_bytes(message_received), datetime.datetime.now()))
		
		# Check if second has passed (Not sure if needed now?)
		# if current_time - start_time >= 1.0:
		# 	logger.info(f"Total data received in the last second: {total_data_received} bytes")
		# 	total_data_received = 0  # Reset the counter
		# 	start_time = current_time  # Reset the start time

def get_wireless_task(serial_port: str | None) -> Thread:
    if serial_port is None:
        raise RuntimeError(
            "If running telemetry in wireless mode, you must specify the radio serial port!"
        )
    return Thread(
        target=_read_messages,
		args=(serial_port, ),
        daemon=True,
    )