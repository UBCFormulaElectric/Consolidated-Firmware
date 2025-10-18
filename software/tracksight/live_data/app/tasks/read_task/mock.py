import datetime
import time
from threading import Thread
from random import random

from flask_socketio import SocketIO 

# ours
from logger import logger

from tasks.broadcaster import CanMsg, can_msg_queue
from tasks.stop_signal import should_run
from middleware.candb import live_can_db

def read_messages_from_file():
    """
    Read messages from a file to simulate receiving from port. Used for testing front end
    """
    logger.debug("Starting mock read task")
    while should_run():
        # Read the CSV file into a DataFrame
        # Iterate over each row (simulate message reception over time)
        # for _i, row in df.iterrows():
        #     # each message has multiple signals
        #     can_timestamp, can_id, can_value = datetime.now(), int(
        #         row['can_signal']), bytearray.fromhex(row["can_value"])
        #     can_msg_queue.put(CanMsg(can_id, can_value, can_timestamp))
        #     # sleep before emitting next message
        #     time.sleep(1)

        bms_ts_msg = live_can_db.get_message_by_id(412)
        assert bms_ts_msg is not None, "BMS Tractive System message not found"
        payload = {
            signal.name: round(random() * (signal.max_val - signal.min_val) + signal.min_val) for signal in bms_ts_msg.signals
        }
        bms_ts_payload = bms_ts_msg.pack(payload)
        can_msg_queue.put(
            CanMsg(412, bms_ts_payload, datetime.datetime.now(datetime.timezone.utc)) # BMS Tractive System
        )

        vc_vitals_msg = live_can_db.get_message_by_id(500)
        assert vc_vitals_msg is not None, "VC Vitals message not found"
        vc_vitals_payload = vc_vitals_msg.pack({
            signal.name: round(random() * (signal.max_val - signal.min_val) + signal.min_val) for signal in vc_vitals_msg.signals
        })
        can_msg_queue.put(
            CanMsg(500, vc_vitals_payload, datetime.datetime.now(datetime.timezone.utc)) # VC Vitals
        )
        # can_msg_queue.put(
        #     CanMsg(501, bytearray(os.urandom(8)), datetime.datetime.now())
        # )
        # can_msg_queue.put(
        #     CanMsg(502, bytearray(os.urandom(8)), datetime.datetime.now())
        # )

        # can_msg_queue.put(
        #     CanMsg(503, bytearray(os.urandom(8)), datetime.datetime.now())
        # )

        # can_msg_queue.put(
        #     CanMsg(504, bytearray(os.urandom(8)), datetime.datetime.now())
        # )
        # can_msg_queue.put(
        #     CanMsg(505, bytearray(os.urandom(8)), datetime.datetime.now())
        # )
        # can_msg_queue.put(
        #     CanMsg(506, bytearray(os.urandom(8)), datetime.datetime.now())
        # )
        # can_msg_queue.put(
        #     CanMsg(233, bytearray(os.urandom(8)), datetime.datetime.now()) # VC ConnectorBoardEfuse
        # )
        time.sleep(0.001)
    logger.debug("Mock read task stopped")

def get_mock_task(sio: SocketIO) -> Thread:
    # if data_file is None:
    #     raise RuntimeError(
    #         "In 'mock' mode, you must specify the data file to read from")
    return sio.start_background_task(
        read_messages_from_file
    ) 
