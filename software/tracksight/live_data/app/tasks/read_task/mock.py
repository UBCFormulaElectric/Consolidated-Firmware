import datetime
import os
import time
from threading import Thread

# types
from typing import NoReturn

import pandas as pd
from middleware.candb import live_can_db
from tasks.broadcaster import CanMsg, can_msg_queue

# ours
# we pass messages to the socket broadcaster

#


def read_messages_from_file() -> NoReturn:
    """
    Read messages from a file to simulate receiving from port. Used for testing front end
    """
    while True:
        # Read the CSV file into a DataFrame
        # Iterate over each row (simulate message reception over time)
        # for _i, row in df.iterrows():
        #     # each message has multiple signals
        #     can_timestamp, can_id, can_value = datetime.now(), int(
        #         row['can_signal']), bytearray.fromhex(row["can_value"])
        #     can_msg_queue.put(CanMsg(can_id, can_value, can_timestamp))
        #     # sleep before emitting next message
        #     time.sleep(1)

        for msg_name, msg_obj in live_can_db.msgs.items():
            can_msg_queue.put(
                CanMsg(412, bytearray(os.urandom(8)), datetime.datetime.now())
            )
            can_msg_queue.put(
                CanMsg(500, bytearray(os.urandom(8)), datetime.datetime.now())
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
            can_msg_queue.put(
                CanMsg(233, bytearray(os.urandom(8)), datetime.datetime.now())
            )



            time.sleep(0.1)


def get_mock_task() -> Thread:
    # if data_file is None:
    #     raise RuntimeError(
    #         "In 'mock' mode, you must specify the data file to read from")
    mock_write_thread = Thread(
        target=read_messages_from_file,
        args=(),
        daemon=True,
    )
    return mock_write_thread
