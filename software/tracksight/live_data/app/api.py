from flask import request
from queue import Empty
from logger import logger
from message_queue import message_queue
from threading import Thread, Event

VALID_SIGNALS = []
sub_table = {}

from initilize_app import sio

@sio.on('connect')
def connect():
    logger.info('SocketIO connection is established!')
    sub_table[request.sid] = ['BMS_BmsOk', 'BMS_ImdLatchedFault']

@sio.on('disconnect')
def disconnect():
    logger.info('SocketIO is disconnect')

def _send_data(stop_event):
    while not stop_event.is_set():  # Continue until stop_event is set
        try:
            data = message_queue.get(timeout=1)
        except Empty:
            print("No messages...")
            continue

        # Process the data
        # if not isinstance(data, list) or not data or 'signal' not in data[0]:
        #     logger.error("Invalid data format: %s", data)
        #     continue
        # Example processing logic
        for sid, signals in sub_table.items():
            if data[0]['signal'] in signals:
                try:
                    # sio.emit('data', 'hello')
                    print('Sending Signal')
                    sio.emit('data', data, to=sid)
                    logger.info(f'Data sent to sid {sid}')
                except Exception as e:
                    logger.error(f'Emit failed for sid {sid}: {e}')
    logger.info('Send data thread stopped')

def run_broadcast_thread(stop_event: Event):
    read_thread = Thread(
        target=_send_data,
        args=(stop_event),
        daemon=True
    )
    read_thread.start()
    return read_thread