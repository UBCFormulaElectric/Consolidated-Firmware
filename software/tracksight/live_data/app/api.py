from flask import request
from queue import Empty
from logger import logger
from signal_queue import signal_queue, Signal
from threading import Thread, Event
from flask_app import sio, api


VALID_SIGNALS = []
sub_table = {}


@sio.on('connect')
def connect():
    logger.info('SocketIO connection is established!')
    sub_table[request.sid] = ['BMS_BmsOk', 'BMS_ImdLatchedFault']

@sio.on('disconnect')
def disconnect():
    print(request.sid)
    logger.info('SocketIO is disconnected')

@api.route("/")
def hello_world():
    return "<p>Hello, World!</p>"

def _send_data(stop_event):
    while not stop_event.is_set():  # Continue until stop_event is set
        try:
            data: Signal = signal_queue.get(timeout=5)
        except Empty:
            print("No messages...")
            continue

        # Process the data
        # if not isinstance(data, list) or not data or 'signal' not in data[0]:
        #     logger.error("Invalid data format: %s", data)
        #     continue

        # Example processing logic
        for sid, signals in sub_table.items():
            if data.name in signals:
                try:
                    sio.emit('data', data, to=sid)
                    logger.info(f'Data sent to sid {sid}')
                except Exception as e:
                    logger.error(f'Emit failed for sid {sid}: {e}')
    logger.info('Send data thread stopped')

def run_broadcast_thread(stop_event: Event):
    read_thread = Thread(
        target=_send_data,
        args=(stop_event,),
        daemon=True
    )
    return read_thread