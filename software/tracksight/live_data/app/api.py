from flask import Blueprint, jsonify, request, render_template
from flask_socketio import emit
import logging
from queue import Empty

logger = logging.getLogger('live_data_logger')

api = Blueprint('api', __name__)

VALID_SIGNALS = []
sub_table = {}

class WebSocket:
    @classmethod
    def setup(cls, sio):
        cls.sio = sio
        cls.register_sio_events()

    @classmethod
    def register_sio_events(cls):
        @cls.sio.on('connect')
        def connect():
            logger.info('SocketIO connection is established!')
            sub_table[request.sid] = ['BMS_BmsOk', 'BMS_ImdLatchedFault']
        
        @cls.sio.on('disconnect')
        def disconnect():
            logger.info('SocketIO is disconnect')
    
    @classmethod
    def send_data(cls, _shared_queue, stop_event):
        while not stop_event.is_set():  # Continue until stop_event is set
            try:
                # Get data from the queue with a timeout
                data = _shared_queue.get(timeout=1)
            except Empty:
                # No data in the queue, check for stop_event and continue
                continue

            # Process the data
            # if not isinstance(data, list) or not data or 'signal' not in data[0]:
            #     logger.error("Invalid data format: %s", data)
            #     continue

            # Example processing logic
            for sid, signals in sub_table.items():
                if data[0]['signal'] in signals:
                    print('HEYYYYYY')
                    try:
                        cls.sio.emit('data', 'hello')
                        cls.sio.emit('data', data, to=sid)
                        logger.info(f'Data sent to sid {sid}')
                    except Exception as e:
                        logger.error(f'Emit failed for sid {sid}: {e}')

        logger.info('Send data thread stopped')

@api.route('/home', methods=['POST', 'GET'])
def home():
    return render_template('index.html')
            
        
   