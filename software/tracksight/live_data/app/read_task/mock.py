import threading
import pandas as pd
import time
from broadcaster import CanMsg, can_msg_queue
from logger import logger

def read_messages_from_file(data_file: str):
	"""
	Read messages from a file to simulate receiving from port. Used for testing front end
	"""
	while True:
		# Read the CSV file into a DataFrame
		df = pd.read_csv(data_file)

		# Iterate over each row (simulate message reception over time)
		for _i, row in df.iterrows():
			# each message has multiple signals
			can_id, can_value = int(row['can_signal']), bytearray.fromhex(row["can_value"])
			can_msg_queue.put(CanMsg(can_id, can_value))
			# Add the time stamp and get name
			# single_signal["timestamp"] = message_received.time_stamp
			# signal_name = single_signal["name"] 

			# Update the list of availble signals and add it to client signals
			# if signal_name not in cls.available_signals:
			# 	cls.available_signals[signal_name] = True
			# 	cls.client_signals[signal_name] = []

		
			# Ensure the value is the correct type (convert to float)
			# value = int(single_signal["value"])

			# Create a DataFrame for the new signal
			# new_signal_df = pd.DataFrame([{
			# 	"time": pd.Timestamp.now(tz=get_localzone()),#TODO: Make time more accurate in mili since start
			# 	"value": value,
			# 	"unit": single_signal["unit"],
			# 	"signal": single_signal["name"]
			# }])
			# Filter out empty or all-NA columns before concatenation
			# cls.signal_df = cls.signal_df.dropna(axis=1, how='all')
			# new_signal_df = new_signal_df.dropna(axis=1, how='all')
			# Concatenate the new signal DataFrame with the existing one
			# cls.signal_df = pd.concat([cls.signal_df, new_signal_df], ignore_index=True)

	
			# Emit the message
			# if len(cls.signal_df) >= cls.max_df_size:
			# 	print(cls.signal_df)
			# 	data = cls.signal_df.to_dict(orient='records')
			# 	data[0]['time'] = data[0]['time'].isoformat()

			# 	InfluxHandler.write(
			# 		cls.signal_df, measurement='live'
			# 	)

			# 	cls.signal_df = pd.DataFrame(columns=['time', 'value', 'unit', 'signal'])

			# sleep before emitting next message
			time.sleep(1)

def get_mock_task(data_file: str | None) -> threading.Thread:
    if data_file is None:
        raise RuntimeError("In 'mock' mode, you must specify the data file to read from")
    mock_write_thread = threading.Thread(
        target=read_messages_from_file,
		args=(data_file, ),
        daemon=True,
    )
    return mock_write_thread
