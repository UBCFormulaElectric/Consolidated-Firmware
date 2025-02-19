from dataclasses import dataclass
from threading import Thread
from queue import Empty, Queue
from logger import logger
import influx_handler
from candb import can_db

@dataclass
class InfluxCanMsg:
	can_id: int
	data: bytearray

influx_queue = Queue()

def _log_influx():
	while True:
		try:
			i_canmsg: InfluxCanMsg = influx_queue.get(timeout=5)
		except Empty:
			logger.warning("No messages (for influx)")
			continue
		for signal in can_db.unpack(i_canmsg.can_id, i_canmsg.data):
			influx_handler.write_canmsg(signal.value, signal.name)
			
def get_influx_logger_task() -> Thread:
	return Thread(
		target=_log_influx,
		daemon=True
	)