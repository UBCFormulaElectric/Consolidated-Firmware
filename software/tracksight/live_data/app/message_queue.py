from queue import Queue

# Simulating message_received object
# TODO make this a data class
class MessageReceived:
	def __init__(self, time_stamp, can_id, data):
		self.time_stamp = time_stamp
		self.can_id = can_id
		self.data = data

message_queue = Queue()