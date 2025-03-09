import json
from threading import Thread
from datetime import datetime
from queue import Queue, Empty

# types
from typing import NoReturn
from requests import HTTPError
from dataclasses import dataclass

# ours
from logger import logger
from subtable import SUB_TABLE
from candb import live_can_db, fetch_jsoncan_configs

from api.socket import sio
from tasks.influx_logger import influx_queue, InfluxCanMsg # for passing the message along

@dataclass
class CanMsg:
	can_id: int
	can_value: bytearray
	can_timestamp: datetime

@dataclass
class Signal:
	name: str
	value: any
	unit: str
	timestamp: str
	def toJSON(self) -> str:
		return json.dumps(
			self,
			default=lambda o: o.__dict__, 
			sort_keys=True,
			indent=4
		)

can_msg_queue = Queue()

def _send_data() -> NoReturn:
	while True:
		try:
			canmsg: CanMsg = can_msg_queue.get(timeout=30)
		except Empty:
			logger.warning("30 second canmsg drought (for sockets)")
			continue

		# handle commit info updates
		if canmsg.can_id % 100 == 6: # i.e. the canid is of the form x06, which means it is a commit info message
			try:
				config_path = fetch_jsoncan_configs(canmsg.can_value.hex())
				live_can_db.update_path(config_path) # TODO not working yet
			except HTTPError:
				logger.critical(f"Could not fetch new commit information for quintuna at commit {canmsg.can_value.hex()}")
			finally:
				continue # do not continue to parse this message

		for signal in live_can_db.unpack(canmsg.can_id, canmsg.can_value):
			for sid, signal_names in SUB_TABLE.items():
				if signal["name"] in signal_names:
					try:
						sio.emit('data', {
							'name': signal['name'],
							'value': signal['value'],
							'timestamp': canmsg.can_timestamp.isoformat()
						}, room=sid)
						logger.info(f'Data sent to sid {sid}')
					except Exception as e:
						logger.error(f'Emit failed for sid {sid}: {e}')
			# send to influx logger
			influx_queue.put(InfluxCanMsg(signal["name"], signal["value"], canmsg.can_timestamp))

def get_websocket_broadcast() -> Thread:
	return Thread(target=_send_data, daemon=True)