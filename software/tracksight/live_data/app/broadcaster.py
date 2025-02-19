from dataclasses import dataclass

from requests import HTTPError
from can_msg_queue import can_msg_queue, CanMsg
from queue import Empty
from threading import Thread
from logger import logger
from api import sio
from subtable import SUB_TABLE
import json
from candb import can_db, fetch_jsoncan_configs

@dataclass
class Signal:
	name: str
	value: any
	unit: str
	timestamp: str
	def toJSON(self):
		return json.dumps(
            self,
            default=lambda o: o.__dict__, 
            sort_keys=True,
            indent=4
		)

def _send_data():
    while True:
        try:
            canmsg: CanMsg = can_msg_queue.get(timeout=5)
        except Empty:
            logger.warning("No messages")
            continue

        # handle commit info updates
        if canmsg.can_id % 100 == 6: # i.e. the canid is of the form x06, which means it is a commit info message
            try:
                config_path = fetch_jsoncan_configs("quintuna", canmsg.data.hex())
                can_db.update_path(config_path)
            except HTTPError:
                logger.critical(f"Could not fetch new commit information for quintuna at commit {canmsg.data.hex()}")
            finally:
                continue # do not continue to parse this message
        
        for signal in can_db.unpack(canmsg.can_id, canmsg.data):
            for sid, signal_names in SUB_TABLE.items():
                if signal.name in signal_names:
                    try:
                        sio.emit('data', canmsg.toJSON(), to=sid)
                        logger.info(f'Data sent to sid {sid}')
                    except Exception as e:
                        logger.error(f'Emit failed for sid {sid}: {e}')

def get_websocket_broadcast():
    return Thread(target=_send_data, daemon=True)