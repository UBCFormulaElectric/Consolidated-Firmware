from dataclasses import dataclass
import json
from queue import Queue

# Simulating message_received object

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

signal_queue = Queue()