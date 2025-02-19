# TODO FINISH
# import time
# from tzlocal import get_localzone

from threading import Thread
import serial
from generated import telem_pb2
from logger import logger
from broadcaster import can_msg_queue, CanMsg

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

	last_bit = 0
	# last_time = time.time()  # Initialize last_time
	total_data_received = 0  # Initialize the data counter
	# start_time = time.time()  # Start timer for data measurement

	ser = serial.Serial(port=port, baudrate=57600, timeout=1)
	ser.reset_input_buffer()
	ser.reset_output_buffer()

	while True:
		# TODO: Lara: Upload actual signals instead!
		# TODO: what happens with dropped packets??
		# current_time = time.time()
		packet_size = int.from_bytes(ser.read(1), byteorder="little")
		logger.info(f"Received data of size {packet_size}")
		if packet_size in {255, 165, 253}: # idk why this is here
			continue
		# the size will be different due to 0 not often being include
		if last_bit != 0 or packet_size == 0:
			last_bit = packet_size
			continue

		# Read in UART message
		bytes_read = ser.read(packet_size)
		total_data_received += len(bytes_read) + 1  # Including packetsize byte, log how much data

		# Parse protobuf
		message_received = telem_pb2.TelemMessage()
		message_received.ParseFromString(bytes_read)
		can_msg_queue.put(CanMsg(message_received.can_id, _make_bytes(message_received)))
		
		# Check if second has passed
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