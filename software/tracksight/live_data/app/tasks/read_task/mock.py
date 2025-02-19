from typing import NoReturn
import pandas as pd
import time
from tasks.broadcaster import CanMsg, can_msg_queue
from datetime import datetime
from logger import logger
from threading import Thread

def read_messages_from_file(data_file: str) -> NoReturn:
	"""
	Read messages from a file to simulate receiving from port. Used for testing front end
	"""
	while True:
		# Read the CSV file into a DataFrame
		df: pd.DataFrame = pd.read_csv(data_file)
		# Iterate over each row (simulate message reception over time)
		for _i, row in df.iterrows():
			# each message has multiple signals
			can_timestamp, can_id, can_value = datetime.utcnow(), int(row['can_signal']), bytearray.fromhex(row["can_value"])
			can_msg_queue.put(CanMsg(can_id, can_value, can_timestamp))
			# sleep before emitting next message
			time.sleep(1)

def get_mock_task(data_file: str | None) -> Thread:
    if data_file is None:
        raise RuntimeError("In 'mock' mode, you must specify the data file to read from")
    mock_write_thread = Thread(
        target=read_messages_from_file,
		args=(data_file, ),
        daemon=True,
    )
    return mock_write_thread
