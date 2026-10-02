"""
bootloader.py
-----------------
Class used to interface with a embedded CAN bootloader.

"""

from typing import Callable, Optional
import math
import can
import time
import intelhex
import threading

import boards

# Keep CAN protocol in sync with:
# Consolidated-Firmware/firmware/boot/shared/config.h

# CAN command message IDs.
MCU_10HZ_STATUS_CAN_ID_LOWBITS = 0x0
START_UPDATE_ID_LOWBITS = 0x1
UPDATE_ACK_ID_LOWBITS = 0x2
GO_TO_APP_LOWBITS = 0x3
ERASE_SECTOR_CAN_ID_LOWBITS = 0x4
ERASE_SECTOR_COMPLETE_CAN_ID_LOWBITS = 0x5
PROGRAM_CAN_ID_LOWBITS = 0x6
VERIFY_CAN_ID_LOWBITS = 0x7
APP_VALIDITY_CAN_ID_LOWBITS = 0x8
GO_TO_BOOT_CAN_ID_LOWBITS = 0x9
ERASE_SECTOR_FAILED_ID_LOWBITS = 0xA
PROGRAM_ID_FAILED_LOWBITS = 0xB
PROGRAM_DONE_LOWBITS = 0xC

# Verify response options.
# Keep in sync with:
# Consolidated-Firmware/firmware/boot/bootloader.c
BOOT_STATUS_APP_VALID = 0
BOOT_STATUS_APP_INVALID = 1
BOOT_STATUS_NO_APP = 2

# The minimum amount of data the microcontroller can program at a time.
MIN_PROG_SIZE_BYTES = 32


class Bootloader:
    bus: can.Bus
    ih: intelhex.IntelHex
    board: boards.Board
    timeout: int
    ui_callback: Callable
    is_fd: bool

    def __init__(
        self,
        bus: can.Bus,
        board: boards.Board,
        ui_callback: Callable,
        ih: intelhex.IntelHex = None,
        timeout: int = 1,
        is_fd: bool = False,
    ) -> None:
        self.bus: can.Bus = bus
        self.ih: intelhex.IntelHex = ih
        self.board: boards.Board = board
        self.timeout: int = timeout
        self.ui_callback: Callable = ui_callback
        self.is_fd = is_fd

    def goto_bootloader(self, retry: bool = False, retry_delay: float = 0.5) -> bool:
        """
        Pushes all boards to bootloader mode.
        :throws: TimeoutError if the boards do not respond
        :return: True when the bootloader acknowledges the command.
        """
        while True:
            self.bus.send(
                can.Message(
                    arbitration_id=(
                        self.board.boot_id_range_start | GO_TO_BOOT_CAN_ID_LOWBITS
                    ),
                    data=[],
                    is_extended_id=True,
                    is_fd=self.is_fd,
                ),
                timeout=10,
            )
            if (
                self._await_can_msg(
                    lambda msg: msg.arbitration_id
                    in (
                        self.board.boot_id_range_start | GO_TO_BOOT_CAN_ID_LOWBITS,
                        self.board.boot_id_range_start | MCU_10HZ_STATUS_CAN_ID_LOWBITS,
                    ),
                    5,
                )
                is not None
            ):
                return True
            if not retry:
                return False
            time.sleep(retry_delay)

    def goto_app(self, retry: bool = False, retry_delay: float = 0.5) -> bool:
        while True:
            self.bus.send(
                can.Message(
                    arbitration_id=self.board.boot_id_range_start | GO_TO_APP_LOWBITS,
                    data=[],
                    is_extended_id=True,
                    is_fd=self.is_fd,
                ),
                timeout=10,
            )
            if (
                self._await_can_msg(
                    lambda msg: msg.arbitration_id == self.board.app_id_range_start + 0,
                    5,
                )
                is not None
            ):
                return True
            if not retry:
                return False
            time.sleep(retry_delay)

    def start_update(self, retry: bool = False, retry_delay: float = 0.5) -> bool:
        """
        Command a board's bootloader to enter update mode by sending the
        "start update" commmand. Await a response.

        Returns:
            True if the bootloader acknowledged the command within the timeout,
            false otherwise.

        """

        def _validator(msg: can.Message) -> bool:
            """Validate that we've received the "update ack" msg."""
            return (
                True
                if msg.arbitration_id
                == (self.board.boot_id_range_start | UPDATE_ACK_ID_LOWBITS)
                else None
            )

        start_addr = self.ih.minaddr()
        end_addr = self.ih.minaddr() + self.size_bytes()
        CAN_FRAME_SIZE_BYTES = 64 if self.is_fd else 8
        expected_block_num = (end_addr - start_addr) // CAN_FRAME_SIZE_BYTES
        # convert expected_block_num to a little endian byte array
        expected_block_num_bytes = expected_block_num.to_bytes(4, byteorder="little")
        while True:
            self.bus.send(
                can.Message(
                    arbitration_id=self.board.boot_id_range_start | START_UPDATE_ID_LOWBITS,
                    data=expected_block_num_bytes,
                    is_extended_id=True,
                    is_fd=self.is_fd,
                )
            )
            if self._await_can_msg(validator=_validator, timeout=self.timeout) is not None:
                return True
            if not retry:
                return False
            time.sleep(retry_delay)

    def erase_sectors(self, sectors) -> bool:
        """
        Erase specific sectors of FLASH, sets all bytes to 0xFF. FLASH memory must first be
        erased before it can be programmed.

        Returns:
            True if the bootloader acknowledged the commands within the timeout,
            false otherwise.

        """

        def _validator(msg: can.Message):
            """Validate that we've received the "erase complete" msg."""
            return (
                True
                if msg.arbitration_id
                == (self.board.boot_id_range_start | ERASE_SECTOR_COMPLETE_CAN_ID_LOWBITS)
                else None
            )

        erase_size = sum([sector.size for sector in sectors])
        erase_progress = 0

        for sector in sectors:
            if self.ui_callback:
                self.ui_callback("Erasing FLASH sectors", erase_size, erase_progress)

            if sector.write_protect:
                raise RuntimeError(f"Attempted to write to a readonly memory sector!{sectors}")

            while True:
                self.bus.send(
                    can.Message(
                        arbitration_id=self.board.boot_id_range_start
                        | ERASE_SECTOR_CAN_ID_LOWBITS,
                        data=[sector.id],
                        is_extended_id=True,
                        is_fd=self.is_fd,
                    )
                )
                if self._await_can_msg(validator=_validator, timeout=self.timeout):
                    break

            erase_progress += sector.size

        if self.ui_callback:
            self.ui_callback("Erasing FLASH sectors", erase_size, erase_size)

        return True

    def program(self) -> None:
        """
        Program the binary into flash. There is no CAN handshake here to reduce
        latency during programming. Also, the bootloader will verify the app's code is valid
        by computing a checksum.

        """
        CAN_FRAME_SIZE_BYTES = 64 if self.is_fd else 8
        start_addr = self.ih.minaddr()
        end_addr = self.ih.minaddr() + self.size_bytes()

        jump_back_block: Optional[int] = None  # TODO populate with CAN failure messages
        done = False
        stop_listener = threading.Event()
        # spawn a new thread which populates the above
        def listener(stop_listener: threading.Event):
            nonlocal jump_back_block, done
            while not stop_listener.is_set():
                can_msg = self._await_can_msg(
                    validator=lambda msg: msg.arbitration_id == (self.board.boot_id_range_start | PROGRAM_ID_FAILED_LOWBITS) or msg.arbitration_id == (self.board.boot_id_range_start | PROGRAM_DONE_LOWBITS),
                    timeout=1,
                )
                if can_msg is None:
                    continue
                if can_msg.arbitration_id == (self.board.boot_id_range_start | PROGRAM_DONE_LOWBITS):
                    done = True
                    break
                elif can_msg.arbitration_id == (self.board.boot_id_range_start | PROGRAM_ID_FAILED_LOWBITS):
                    k = can_msg.data
                    assert can_msg.dlc == 4, f"Expected 4 bytes in PROGRAM_ID_FAILED message, got {can_msg.dlc}"
                    # TODO maybe better struct unpacking :sob:
                    jump_back_block = (k[3] << 24) | (k[2] << 16) | (k[1] << 8) | k[0]
                    print(f"jumpback received to {jump_back_block}")
                else:
                    raise RuntimeError(f"Received unexpected message with id {can_msg.arbitration_id} during programming")
        listen_thread = threading.Thread(target=listener, args=(stop_listener,))
        listen_thread.start()

        # TODO: Check if binary is aligned to 64 bytes and ensure ending bytes are sent
        block = 0
        last_block = (end_addr - start_addr) // CAN_FRAME_SIZE_BYTES
        tries_left = 10
        while not done:
            while block <= last_block:
                # check if we need to go back
                if jump_back_block is not None:
                    block = jump_back_block
                    jump_back_block = None
                if self.ui_callback and block % 128 == 0: # update ui only every little while
                    self.ui_callback(
                        "Programming data", self.size_bytes(), block * CAN_FRAME_SIZE_BYTES
                    )

                data = [self.ih[start_addr + block * CAN_FRAME_SIZE_BYTES + j] for j in range(0, CAN_FRAME_SIZE_BYTES)]
                wire_block_num = block << 4
                while True:
                    try:
                        self.bus.send(
                            can.Message(
                                arbitration_id=self.board.boot_id_range_start | wire_block_num | PROGRAM_CAN_ID_LOWBITS,
                                data=data,
                                is_extended_id=True,
                                is_fd=self.is_fd,
                            )
                        )
                        break
                    except can.interfaces.vector.exceptions.VectorOperationError:
                        pass
                block += 1
            time.sleep(0.1)
            if jump_back_block is not None:
                block = jump_back_block
                jump_back_block = None
            if tries_left <= 0:
                break # inshallah
            tries_left -= 1
        stop_listener.set()
        listen_thread.join()

        if self.ui_callback:
            self.ui_callback("Programming data", self.size_bytes(), self.size_bytes())

    def status(self) -> Optional[int]:
        """
        Query the bootloader if programming was successful. To do this, 2 checksums are computed:
        1. At compile time, a checksum of the app hex is calculated and added to the generated image's hex.
        2. The bootloader can independly calculate a checksum of the app code in its flash memory.

        If these match, we can conclude with confidence the data programmed into flash by the bootloader
        matches the binary built by the compiler.

        Returns:
            Message with bootloader's status, or None if no message was received within the timeout.
            The first byte of the message is one of BOOT_STATUS_*. (NO_APP, APP_INVALID, APP_VALID)

        """

        def _validator(msg: can.Message):
            """Validate that we've received the "app validity" msg, and the app is valid."""
            return (
                True
                if msg.arbitration_id
                == (self.board.boot_id_range_start | APP_VALIDITY_CAN_ID_LOWBITS)
                else None
            )

        self.bus.send(
            can.Message(
                arbitration_id=self.board.boot_id_range_start | VERIFY_CAN_ID_LOWBITS,
                data=[],
                is_extended_id=True,
                is_fd=self.is_fd,
            )
        )
        rx_msg = self._await_can_msg(_validator)
        if rx_msg is None:
            return None

        if rx_msg.dlc < 1:
            raise RuntimeError("Zero Message recieved")

        return rx_msg.data[0]

    def update(self) -> None:
        """
        Run the update procedure for this bootloader.

        """

        def _intersect(a_min, a_max, b_min, b_max):
            """1-D intersection to check if an app's hex and a flash sector share any addresses."""
            return a_max >= b_min and b_max >= a_min

        if not self.start_update(retry=True):
            raise RuntimeError(
                f"Bootloader for {self.board.name} did not respond to command to start a firmware update."
            )

        time.sleep(0.1)

        # To speedup programming, only erase the sectors used by the app.
        app_flash_sectors = [
            sector
            for sector in self.board.mcu.flash_sectors
            if _intersect(
                a_min=self.ih.minaddr(),
                a_max=self.ih.minaddr() + self.size_bytes(),
                b_min=sector.base_address,
                b_max=sector.max_address,
            )
        ]

        if not self.erase_sectors(app_flash_sectors):
            raise RuntimeError(
                f"Bootloader for {self.board.name} did not respond to command to erase flash memory."
            )
        time.sleep(0.5)

        self.program()
        time.sleep(0.5)

        self.ui_callback("Verifying programming", self.size_bytes(), 0)
        boot_status = self.status()
        if boot_status is not None:
            if boot_status != BOOT_STATUS_APP_VALID:
                raise RuntimeError(
                    f"Bootloader for {self.board.name} failed to verify application integrity, something went wrong during updating."
                )
        else:
            raise RuntimeError(
                f"Bootloader for {self.board.name} did not respond to command to verify application integrity."
            )

        self.ui_callback("Verifying programming", self.size_bytes(), self.size_bytes())
        time.sleep(0.5)

    def erase(self) -> None:
        """
        Erase this bootloader's application.

        """
        if not self.start_update():
            raise RuntimeError(
                f"Bootloader for {self.board.name} did not respond to erase command."
            )

        writeable_sectors = [
            sector
            for sector in self.board.mcu.flash_sectors
            if not sector.write_protect
        ]
        erase_size = sum([sector.size for sector in writeable_sectors])

        if not self.erase_sectors(writeable_sectors):
            raise RuntimeError(
                f"Bootloader for {self.board.name} did not respond to command to erase flash memory."
            )
        time.sleep(0.5)

        self.ui_callback("Verifying erase", erase_size, 0)
        boot_status = self.status()
        if boot_status is not None:
            if boot_status != BOOT_STATUS_NO_APP:
                raise RuntimeError(
                    f"Bootloader for {self.board.name} failed to verify erase, something went wrong."
                )
        else:
            raise RuntimeError(
                f"Bootloader for {self.board.name} did not respond to command to erase flash."
            )

        self.ui_callback("Verifying erase", erase_size, erase_size)
        time.sleep(0.5)

    def _await_can_msg(
        self, validator=Callable[[can.Message], Optional[bool]], timeout: int = 5
    ) -> Optional[can.Message]:
        """
        Helper function to await a CAN msg response within a timeout, with a validator function.

        """
        assert validator is not None

        start = time.time()
        while time.time() - start < timeout:
            rx_msg: can.Message = self.bus.recv(timeout=1)
            if rx_msg is None:
                continue
            if validator(rx_msg):
                return rx_msg
        return None

    def size_bytes(self) -> int:
        """
        Get the size of the binary. This **must** be a multiple of 32, since the
        STM32H733xx microcontroller can only write 32 bytes at a time. See
        `firmware/boot/bootloader_h7.c` for how this works on the
        microcontroller side.

        Returns:
            Size, in bytes.

        """
        return int(
            math.ceil((self.ih.maxaddr() - self.ih.minaddr()) / MIN_PROG_SIZE_BYTES)
            * MIN_PROG_SIZE_BYTES
        )
