# TODO FINISH
# import time
# from tzlocal import get_localzone

from threading import Thread
import serial
from generated import telem_pb2
from logger import logger
from tasks.broadcaster import can_msg_queue, CanMsg
import datetime

HEADER_SIZE = 5
MAGIC = b'\xAA\x55'

MAX_PAYLOAD_SIZE = 50 # this is arbitrary lmao

def _make_bytes(message):
	"""
	Make the byte array out of the messages array.
	"""
	return bytearray([
		message.message_0, message.message_1, message.message_2, 
		message.message_3, message.message_4, message.message_5, 
		message.message_6, message.message_7
	]) 

def find_magic_in_buffer(buffer, magic=MAGIC): # do i need to set magic here
    """
	Return the index of the first occurrence of magic bytes in the buffer, or -1 if not found.
	"""
    magic_len = len(magic)
    for i in range(len(buffer) - magic_len + 1):
        if buffer[i:i+magic_len] == magic:
            return i
    return -1

def read_packet(ser: serial.Serial):
	buffer = bytearray()
	while True:
		data = ser.read(1)
		if not data:
			continue
		buffer += data
	
		#wait for full header
		if len(buffer) < HEADER_SIZE:
			continue

		#check for magic number
		if buffer[0:2] != MAGIC:
			magic_index = find_magic_in_buffer(buffer, MAGIC)
			if magic_index == -1:
				buffer.clear()
			else:
				buffer = buffer[magic_index:]
			continue

		if len(buffer) < HEADER_SIZE:
			continue

		#parse remainder of header
		payload_length = buffer[2]
		expected_crc = int.from_bytes(buffer[3:5], byteorder='big')

		if payload_length > MAX_PAYLOAD_SIZE:
			logger.error(f"Payload length {payload_length} is too large")
			buffer = buffer[1:]
			continue

		total_length = HEADER_SIZE + payload_length
		if len(buffer) < total_length:
			continue # wait for full packet

		packet = bytes(buffer[:total_length])

		#reset buffer
		buffer = buffer[total_length:]

		return packet, payload_length, expected_crc

def _read_messages(port: str):
	"""
	Read messages coming in through the serial port, decode them, unpack them and then emit them to the socket
	"""

	ser = serial.Serial(port='/dev/tty.usbserial-FT76H2U7', baudrate=57600, timeout = 1)
	#print(ser)
	ser.reset_input_buffer()
	ser.reset_output_buffer()
	while True:
		packet, payload_length, expected_crc = read_packet(ser)
		logger.info(f"Received packet of size {len(packet)} bytes")
		print(f"Received packet of size {len(packet)} bytes")
		payload = packet[HEADER_SIZE:]
		if len(payload) != payload_length:
			logger.error(f"Payload length mismatch: expected {payload_length}, got {len(payload)}")
			continue
			
		# CRC check
		computed_crc = 0xFFFF #HARDCODED
		#computed_crc = crc_FUNC_TODO(payload)
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


# GET RID OF HARDCODE
def get_wireless_task(serial_port: str | None) -> Thread:
	serial_port_hard = '/dev/tty.usbserial-FT76H2U7'
	if serial_port_hard is None:
		raise RuntimeError(
            "If running telemetry in wireless mode, you must specify the radio serial port!"
        )
	return Thread(
        target=_read_messages,
		args=(serial_port_hard, ),
        daemon=True,
    )