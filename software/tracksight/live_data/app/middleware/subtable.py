import sqlite3
from typing import Set
from logger import logger

# Connect to an in-memory SQLite database
conn = sqlite3.connect(":memory:") 

conn.execute('''CREATE TABLE subtable (
	sid STRING PRIMARY KEY,
	signal_name STRING NOT NULL
)''')
conn.execute('''CREATE UNIQUE INDEX idx_sid_signal_name ON subtable (sid, signal_name)''')
conn.commit()

_subs: Set[str] = set()

def add_sid(sid: str) -> None:
	_subs.add(sid)

def remove_sid(sid: str) -> None:
	if sid not in _subs:
		raise ValueError(f"SID {sid} not found in subscriptions.")
	c = conn.cursor()
	c.execute("DELETE FROM subtable WHERE sid = ?", (sid,))
	conn.commit()
	_subs.remove(sid)

def subscribe_signal(sid: str, signal_name: str) -> None:
	if sid not in _subs:
		raise ValueError(f"SID {sid} is not subscribed. Please subscribe it first.")

	c = conn.cursor()
	try:
		c.execute("INSERT INTO subtable (sid, signal_name) VALUES (?, ?)", (sid, signal_name))
		conn.commit()
	except sqlite3.IntegrityError:
		logger.warning(f"Signal {signal_name} for SID {sid} is already subscribed.")
		pass

def unsubscribe_signal(sid: str, signal_name: str) -> None:
	if sid not in _subs:
		raise ValueError(f"SID {sid} is not subscribed. Please subscribe it first.")

	c = conn.cursor()
	c.execute("DELETE FROM subtable WHERE sid = ? AND signal_name = ?", (sid, signal_name))
	conn.commit()

def get_subscribed_sids(signal_name: str) -> list[str]:
	c = conn.cursor()
	c.execute("SELECT sid FROM subtable WHERE signal_name = ?", (signal_name,))
	return [row[0] for row in c.fetchall()]