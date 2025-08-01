import sqlite3
from typing import Set
from logger import logger

# Connect to an in-memory SQLite database
conn = sqlite3.connect(":memory:") 

conn.execute('''CREATE TABLE subtable (
	sid STRING NOT NULL,
	signal_name STRING NOT NULL,
	UNIQUE (sid, signal_name)
)''')
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

class SubscriptionError(Exception):
	...

def subscribe_signal(sid: str, signal_name: str) -> None:
	if sid not in _subs:
		raise SubscriptionError(f"SID {sid} is not subscribed. Please subscribe it first.")

	c = conn.cursor()
	try:
		c.execute("INSERT INTO subtable (sid, signal_name) VALUES (?, ?)", (sid, signal_name))
		conn.commit()
	except sqlite3.IntegrityError:
		logger.warning(f"Signal {signal_name} for SID {sid} is already subscribed.")
		raise SubscriptionError(f"Signal {signal_name} is already subscribed for SID {sid}.")

def unsubscribe_signal(sid: str, signal_name: str) -> None:
	if sid not in _subs:
		raise SubscriptionError(f"SID {sid} is not subscribed. Please subscribe it first.")

	c = conn.cursor()
	# check if signal_name exists for the given sid
	c.execute("SELECT COUNT(*) FROM subtable WHERE sid = ? AND signal_name = ?", (sid, signal_name))
	if c.fetchone()[0] == 0:
		raise SubscriptionError(f"Signal {signal_name} is not subscribed for SID {sid}.")
	# remove the signal_name for the given sid
	c.execute("DELETE FROM subtable WHERE sid = ? AND signal_name = ?", (sid, signal_name))
	conn.commit()

def get_subscribed_sids(signal_name: str) -> list[str]:
	c = conn.cursor()
	c.execute("SELECT sid FROM subtable WHERE signal_name = ?", (signal_name,))
	return [row[0] for row in c.fetchall()]