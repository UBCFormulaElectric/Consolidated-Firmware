import json
from dataclasses import dataclass
from datetime import datetime
from typing import Any

@dataclass(frozen=True)
class CanMsg:
	can_id: int
	can_value: bytes
	can_timestamp: datetime

@dataclass(frozen=True)
class CanSignal:
	name: str
	value: Any
	timestamp: datetime 

	def toJSON(self) -> str:
		return json.dumps(self, default=lambda o: o.__dict__, sort_keys=True, indent=4)

