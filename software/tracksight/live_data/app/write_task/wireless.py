# TODO FINISH
import threading
import time
from tzlocal import get_localzone
import serial
import pandas as pd

from generated import telem_pb2
from logger import logger
from candb import can_db

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
	last_time = time.time()  # Initialize last_time
	total_data_received = 0  # Initialize the data counter
	start_time = time.time()  # Start timer for data measurement

	ser = serial.Serial(port=port, baudrate=57600, timeout=1)
	ser.reset_input_buffer()
	ser.reset_output_buffer()

	while True:
		# TODO: Lara: Upload actual signals instead!

		current_time = time.time()

		packet_size = int.from_bytes(ser.read(1), byteorder="little")
		#logger.info(f"Received data: {packet_size}")
		if packet_size in INVALID_PACKET_SIZES:
			continue

		# the size will be different due to 0 not often being include
		if last_bit == 0 and packet_size != 0:
			# Read in UART message and parse the protobuf
			bytes_read = ser.read(packet_size)
			total_data_received += len(bytes_read) + 1  # Including packetsize byte, log how much data

			message_received = telem_pb2.TelemMessage()
			message_received.ParseFromString(bytes_read)

			# Make data array out of ints
			data_array = _make_bytes(message_received)

			# Unpack the data and add the id and meta data
			signal_list = can_db.unpack(message_received.can_id, data_array)

			for single_signal in signal_list:
				# Add the time stamp and get name
				single_signal["timestamp"] = message_received.time_stamp
				signal_name = single_signal["name"] 

				# Update the list of availble signals and add it to client signals
				if signal_name not in cls.available_signals:
					cls.available_signals[signal_name] = True
					cls.client_signals[signal_name] = []

				
				# Ensure the value is the correct type (convert to float)
				value = int(single_signal["value"])

				# Create a DataFrame for the new signal
				new_signal_df = pd.DataFrame([{
					"time": pd.Timestamp.now(tz=get_localzone()),#TODO: Make time more accurate in mili since start
					"value": value,
					"unit": single_signal["unit"],
					"signal": single_signal["name"]
				}])


				# Filter out empty or all-NA columns before concatenation
				cls.signal_df = cls.signal_df.dropna(axis=1, how='all')
				new_signal_df = new_signal_df.dropna(axis=1, how='all')
				# Concatenate the new signal DataFrame with the existing one
				cls.signal_df = pd.concat([cls.signal_df, new_signal_df], ignore_index=True)

		
					# Emit the message
				if len(cls.signal_df) >= cls.max_df_size:
					#print(cls.signal_df)
					InfluxHandler.write(
						cls.signal_df, measurement='live'
					)

					cls.signal_df = pd.DataFrame(columns=['time', 'value', 'unit', 'signal'])
					time.sleep(1)
		else:
			last_bit = packet_size

		# Check if second has passed
		if current_time - start_time >= 1.0:
			logger.info(f"Total data received in the last second: {total_data_received} bytes")
			total_data_received = 0  # Reset the counter
			start_time = current_time  # Reset the start time
	ser.close()

def run_wireless_mode_task(serial_port: str | None) -> threading.Thread:
    if serial_port is None:
        raise RuntimeError(
            "If running telemetry in wireless mode, you must specify the radio serial port!"
        )
    wireless_write_thread = threading.Thread(
        target=_read_messages,
		args=(serial_port),
        daemon=True,
    )
    wireless_write_thread.start()
    return wireless_write_thread