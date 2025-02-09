from dataclasses import dataclass
from queue import Queue

# Simulating message_received object

@dataclass
class Signal:
	name: str
	value: any
	unit: str
	timestamp: str

signal_queue = Queue()