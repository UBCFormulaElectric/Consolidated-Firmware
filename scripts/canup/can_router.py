"""
This module contains the CAN router that sorts received
CAN messages into one queue per board, so several bootloader sessions can share
one bus without reading each other's replies.

A can.Notifier is the only reader of the bus and calls the router once per
message. Each Bootloader session then reads only from its own board's queue.
"""

import queue
import can
import boards


# Keeps the board's high bits of a bootloader CAN ID. Matches the mask the
# bootloader firmware uses (firmware/boot/bootloader.cpp).
BOARD_HIGHBITS_MASK = 0xFF000000


class CanRouter:
    """
    Routes each received CAN message to the queue of the board that sent it.

    Messages are matched in two ways:
    1. Bootloader messages carry the board in their high bits (e.g. 0x08000005 -> VC),
       so they are masked and matched against boot_id_range_start.
    2. App messages use small IDs (e.g. 500 for VC), so they are matched exactly
       against app_id_range_start. goto_app() waits on this ID to confirm the board
       restarted into its app.

    Messages matching neither are dropped. The router only picks the board; each
    session's validator still checks for the exact reply it is waiting on.

    """

    def __init__(
        self,
        boards_to_route: list[boards.Board]
    ) -> None:
        """
        Build the lookup tables from boot and app IDs to board queues.

        :param boards_to_route: Boards to route messages for. Duplicates are ignored,
            so each board gets exactly one queue.
        """

        # Create the look up tables that contain board and app ids to look up from later.

        self.boot_id_to_queue: dict[int, queue.Queue[can.Message]] = {}
        self.app_id_to_queue: dict[int, queue.Queue[can.Message]] = {}

        # Populate the look up tables with each board's info that was parsed in boards_to_route.

        for board in set(boards_to_route):
            board_queue: queue.Queue[can.Message] = queue.Queue()
            self.boot_id_to_queue[board.boot_id_range_start] = board_queue
            self.app_id_to_queue[board.app_id_range_start] = board_queue

    def queue_for(self, board: boards.Board) -> queue.Queue[can.Message]:
        """
        Get the queue that holds messages from a board.

        :param board: Board to get the queue for.
        :return: The board's queue. Raises KeyError if the board was not passed to the router.
        """
        return self.boot_id_to_queue[board.boot_id_range_start]

    def __call__(self, msg: can.Message) -> None:
        """
        Put a received message into its board's queue, or drop it if no board matches.
        Called by can.Notifier on its reader thread for every message, so it must not block.

        :param msg: Received CAN message.
        """
        board_highbits: int = msg.arbitration_id & BOARD_HIGHBITS_MASK
        board_queue = self.boot_id_to_queue.get(board_highbits)
        if board_queue is None:
            board_queue = self.app_id_to_queue.get(msg.arbitration_id)
        if board_queue is not None:
            board_queue.put(msg)
