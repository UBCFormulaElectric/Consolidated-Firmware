from threading import Thread
import serial
from generated import telem_pb2
from logger import logger
from tasks.broadcaster import can_msg_queue, CanMsg
import datetime
from crc import Calculator, Crc32

HEADER_SIZE = 7
MAGIC = b'\xAA\x55'

MAX_PAYLOAD_SIZE = 52 # this is arbitrary lmao

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

		if buffer[0:2] != MAGIC:
			magic_index = find_magic_in_buffer(buffer.hex(), MAGIC)
			if magic_index == -1:
				buffer.clear()
			else:
				buffer = buffer[magic_index:]
			continue

		if len(buffer) < HEADER_SIZE:
			continue

		#parse remainder of header
		payload_length = buffer[2]
		expected_crc = int.from_bytes(buffer[3:7], byteorder='big').to_bytes(4, byteorder='big').hex()  # Updated to read 32-bit CRC and cast to hex

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
	ser = serial.Serial(port, baudrate=57600, timeout = 1)
	ser.reset_input_buffer()
	ser.reset_output_buffer()

	while True:	
		packet, payload_length, expected_crc = read_packet(ser)
		payload = packet[HEADER_SIZE:]
		if len(payload) != payload_length:
			logger.error(f"Payload length mismatch: expected {payload_length}, got {len(payload)}")
			continue
			
		# CRC check
		calculator = Calculator(Crc32.CRC32)
		calculated_checksum = calculator.checksum(payload).to_bytes(4, byteorder='big').hex()
		if expected_crc != calculated_checksum:
			logger.error(f"CRC mismatch: computed ", calculated_checksum, "expected ", expected_crc)
			continue

		try:
			message_received = telem_pb2.TelemMessage()
			message_received.ParseFromString(payload)
		except Exception as e:
			logger.error(f"Error decoding protobuf message: {e}")
			continue
		can_msg_queue.put(CanMsg(message_received.can_id, _make_bytes(message_received), datetime.datetime.now()))

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