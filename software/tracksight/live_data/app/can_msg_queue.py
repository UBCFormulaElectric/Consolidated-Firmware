from dataclasses import dataclass
from queue import Queue

# Simulating message_received object
@dataclass
class CanMsg:
	can_id: int
	data: bytearray


can_msg_queue = Queue()