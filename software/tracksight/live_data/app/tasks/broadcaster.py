from queue import Empty, Queue
from threading import Thread
from time import time
from flask_socketio import SocketIO
from logger import logger

# ours
from middleware.candb import live_can_db
from api.subtable_handler import SUB_TABLE
from tasks.influx_logger import influx_queue
from types.CanMsg import CanMsg, CanSignal
from tasks.stop_signal import should_run
from sio import sio

can_msg_queue: Queue[CanMsg] = Queue()


def _send_data():
    logger.debug("Starting signal broadcaster thread")
    last_message = time()
    logged = False
    while should_run():
        try:
            canmsg: CanMsg = can_msg_queue.get(timeout=1) # tune this to get the server kill timeout
            # note a large value would require the server to take longer to kill the thread
            # while a smaller value will cause more frequent checks for the stop signal (cpu usage)
        except Empty:
            if time() - last_message > 5 and not logged:
                logger.warning("No CAN messages received by the signal broadcaster thread in the last 5 seconds.")
                logged = True
            continue

        last_message = time()
        logged = False

        # handle commit info updates
        # if (
        #     canmsg.can_id % 100 == 6
        # ):  # i.e. the canid is of the form x06, which means it is a commit info message
        #     try:
        #         # TODO concurrency issue since live_can_db also used in http
        #         config_path = fetch_jsoncan_configs(canmsg.can_value.hex())
        #         # live_can_db.update_path(config_path) # TODO not working yet
        #         update_can_db(config_path)
        #     except HTTPError:
        #         logger.critical(
        #             f"Could not fetch new commit information for quintuna at commit {canmsg.can_value.hex()}"
        #         )
        #         continue  # do not continue to parse this message
        msg = live_can_db.get_message_by_id(canmsg.can_id)
        if msg is None:
            logger.warning(f"Received CAN message with unknown ID {canmsg.can_id}. Skipping.")
            continue
        for signal in msg.unpack(canmsg.can_value):
            for sid, signal_names in SUB_TABLE.items():
                if signal.name in signal_names:
                    try:
                        sio.emit(
                            "data",
                            {
                                "name": signal.name,
                                "value": signal.value,
                                "timestamp": canmsg.can_timestamp.isoformat(),
                            },
                            to=sid,
                        )
                        logger.info(f"Data sent to sid {sid}")
                    except Exception as e:
                        logger.error(f"Emit failed for sid {sid}: {e}")
            # send to influx logger
            influx_queue.put(
                CanSignal(signal.name, signal.value, canmsg.can_timestamp)
            )
    logger.debug("Signal broadcaster thread stopped.")

def get_websocket_broadcast(sio: SocketIO) -> Thread:
    return sio.start_background_task(_send_data)