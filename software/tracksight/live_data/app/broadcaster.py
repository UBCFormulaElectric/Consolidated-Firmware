from signal_queue import signal_queue, Signal
from queue import Empty
from threading import Thread
from logger import logger
from api import sio
from subtable import SUB_TABLE

def _send_data():
    while True:
        try:
            data: Signal = signal_queue.get(timeout=5)
        except Empty:
            logger.warning("No messages")
            continue

        # Example processing logic
        for sid, signals in SUB_TABLE.items():
            if data.name in signals:
                try:
                    sio.emit('data', data.toJSON(), to=sid)
                    logger.info(f'Data sent to sid {sid}')
                except Exception as e:
                    logger.error(f'Emit failed for sid {sid}: {e}')

def get_websocket_broadcast():
    return Thread(target=_send_data, daemon=True)