"""
update.py
-----------------
Main driver script used to update code over the CAN bus.

"""

import argparse
import os
from typing import List

import can
import intelhex
from rich.console import Console, Group
from rich.live import Live
from rich.progress import Progress, TextColumn, BarColumn, DownloadColumn
from rich.progress import TaskID
from typing import Callable

import boards
import bootloader
import can_router
from concurrent.futures import ThreadPoolExecutor, as_completed

console = Console()
status = console.status("Status")
progress = Progress(
    TextColumn("[progress.description]{task.description}"),
    BarColumn(),
    DownloadColumn(),
)

MAX_UPDATE_ATTEMPTS = 3 # first attempt + 2 retries

def all_goto_bootloader(live: Live, bootloaders: List[bootloader.Bootloader]):
    live.console.log("Putting all boards into bootloader mode")
    # first put everybody into bootloader mode
    bootload_task = progress.add_task("Jump to Bootloader")
    for b_idx, bootload_board in enumerate(bootloaders):
        progress.update(
            task_id=bootload_task,
            total=len(bootloaders),
            completed=b_idx,
            description=f"Putting {bootload_board.board.name} into bootloader mode",
        )
        if not bootload_board.goto_bootloader(retry=True):
            raise TimeoutError(
                f"Failed to send bootloader command to {bootload_board.board.name}"
            )
    progress.remove_task(bootload_task)
    live.console.log(f"[bold green]All boards pushed into bootloader mode successfully")


def all_goto_app(live: Live, bootloaders: List[bootloader.Bootloader]):
    live.console.log("Pushing all boards out of bootloader mode")
    app_task = progress.add_task("Jump to App")
    for b_idx, bootload_board in enumerate(bootloaders):
        progress.update(
            task_id=app_task,
            total=len(bootloaders),
            completed=b_idx,
            description=f"Putting {bootload_board.board.name} into application mode",
        )
        if not bootload_board.goto_app(retry=True):
            raise TimeoutError(
                f"Failed to send application command to {bootload_board.board.name}"
            )
    progress.remove_task(app_task)
    live.console.log(
        f"[bold green]All boards pushed out of bootloader mode successfully"
    )

def update_with_retries(session: bootloader.Bootloader, live: Live) -> None:
    """
    Run one board's update, retrying on failure. Raises the last error if every reattempt fails. 
    """
    for attempt in range(1, MAX_UPDATE_ATTEMPTS + 1): 
        session.clear_rx_queue()
        try:
            session.update()
            return
        except (RuntimeError, TimeoutError) as e:
            live.console.log(f"[yellow]{session.board.name} attempt {attempt}/{MAX_UPDATE_ATTEMPTS} failed: {e}")
            if attempt == MAX_UPDATE_ATTEMPTS:
                raise

def make_ui_callback(task_id: TaskID, board_name:str) -> Callable[[str, int, int], None]:
    def callback(description, total, completed):
        progress.update(task_id=task_id, description=f"{board_name}: {description}", total=total, completed=completed)
    return callback



def update(
    bus: can.Bus, configs: List[boards.Board], build_dir: str, is_fd: bool, only: bool, router: can_router.CanRouter
) -> None:
    """Update and handle UI."""
    num_boards = len(configs)
    board_tasks = {board: progress.add_task(board.name, total=None) for board in configs}
    bootloaders: List[bootloader.Bootloader] = [
        bootloader.Bootloader(
            bus=bus,
            board=board,
            ui_callback=make_ui_callback(board_tasks[board], board.name),
            ih=intelhex.IntelHex(os.path.join(build_dir, board.path)),
            is_fd=is_fd,
            rx_queue=router.queue_for(board)
        )
        for board in configs
    ]

    boards_to_bootload = configs if only else boards.hexray_boards

    hexray_bootloaders = [
        bootloader.Bootloader(
            bus=bus,
            board=board,
            ui_callback=lambda *args: None, # *args accepts any num of args and lambda ignores all
            ih=intelhex.IntelHex(os.path.join(build_dir, board.path)),
            is_fd=is_fd,
            rx_queue=router.queue_for(board)
        )
        for board in boards_to_bootload
    ]

    with Live(Group(status, progress), transient=True) as live:
        all_goto_bootloader(live, hexray_bootloaders)

        live.console.log(
            f"Updating firmware for boards: [blue bold]{', '.join(board.name for board in configs)}"
        )
        failed_boards = []
        # Flash every board in parallel, one worker thread per board. This is safe because each
        # session only receives from its own queue (see can_router.py) and the bus is a ThreadSafeBus.
        # Leaving the with block waits for every board to finish.
        with ThreadPoolExecutor(max_workers=len(bootloaders)) as pool:
            status.update(f"Updating {num_boards} boards in parallel")
            # submit() starts each board's update() on a worker and returns a Future for its result.
            # Map each Future back to its session so we know which board finished.
            futures = {pool.submit(update_with_retries, bootload_board, live): bootload_board for bootload_board in bootloaders }
            # as_completed() yields each Future as its board finishes, in whatever order that happens.
            for future in as_completed(futures):
                bootload_board = futures[future]
                task_id = board_tasks[bootload_board.board]
                # Re-raises any exception from that board's update() here on the main thread.
                # Boards already running keep going until they finish. Collect boards that failed every attempt which stay in bootloader.
                # The rest go to their app code.
                try:
                    future.result()
                    live.console.log(f"[green]{bootload_board.board.name} updated successfully")
                    progress.update(task_id, description=f"[green]{bootload_board.board.name}: done")
                except (RuntimeError, TimeoutError) as e:
                    failed_boards.append(bootload_board.board)
                    live.console.log(f"[red]{bootload_board.board.name} failed after {MAX_UPDATE_ATTEMPTS} attempts: {e}")
                    progress.update(task_id, description=f"[red]{bootload_board.board.name}: failed")
        for task_id in board_tasks.values():
            progress.remove_task(task_id)
        if not failed_boards:
            live.console.log(
                f"[bold green]Firmware update successfully ({num_boards} board{'s' if num_boards > 1 else ''} updated)"
            )
        app_sessions = [session for session in hexray_bootloaders if session.board not in failed_boards]
        all_goto_app(live, app_sessions)
        if failed_boards:
            names = ", ".join(board.name for board in failed_boards)
            raise RuntimeError(f"Update failed for {names}. They are still in bootloader; rerun CANup to reflash them.")


def erase(bus: can.Bus, configs: List[boards.Board], is_fd: bool, router: can_router.CanRouter) -> None:
    """Erase and handle UI."""
    # push all boards into bootloader
    num_boards = len(configs)
    steps_task = progress.add_task("Steps")
    bootloaders = [
        bootloader.Bootloader(
            bus=bus,
            board=board,
            ui_callback=lambda description, total, completed: progress.update(
                task_id=steps_task,
                total=total,
                description=description,
                completed=completed,
            ),
            is_fd=is_fd,
            rx_queue=router.queue_for(board)
        )
        for board in configs
    ]

    with Live(Group(status, progress), transient=True) as live:
        all_goto_bootloader(live, bootloaders)

        live.console.log(
            f"Erasing with config: [blue bold]{', '.join(board.name for board in configs)}"
        )
        for b_idx, bootloader_board in enumerate(bootloaders):
            # TODO do this in parallel
            status.update(f"Sending board {bootloader_board.board.name} to bootloader")
            status.update(
                f"Erasing board [yellow]{b_idx + 1}/{num_boards}[/]: [blue bold]{bootloader_board.board.name}"
            )
            bootloader_board.erase()
            live.console.log(
                f"[green]{bootloader_board.board.name} erased successfully"
            )
        progress.remove_task(steps_task)
        live.console.log(
            f"[bold green]Erase successful ({num_boards} board{'s' if num_boards > 1 else ''} erased)"
        )


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--bus", type=str, default="vector", help="python-can bus type")
    parser.add_argument("--channel", type=str, default="0", help="python-can channel")
    parser.add_argument(
        "--bit_rate", type=int, default=1000000, help="CAN bus bit rate"
    )
    parser.add_argument(
        "--config",
        "-c",
        type=str,
        required=True,
        help="Config to load. Note that you can specify multiple with comma separation.",
    )
    parser.add_argument(
        "--build",
        "-b",
        type=str,
        help="Path to Consolidated-Firmware firmware build directory (build_fw_deploy)",
    )
    parser.add_argument("--erase", action="store_true", help="Erase app code")
    parser.add_argument("--fd", action="store_true", help="Use FD mode")
    parser.add_argument(
        "--only",
        action="store_true",
        help="Only flash a single ECU. ONLY USED FOR DEVELOPMENT AND TESTING, NOT ON CAR OR BUCK",
    )
    parser.add_argument(
        "--data_bitrate", type=int, default=1000000, help="CAN FD data bitrate"
    )
    args = parser.parse_args()

    # Load config and binary.
    c: list[boards.Board] = list(
        {
            board
            for config_name in args.config.split(",")
            for board in boards.CONFIGS[config_name.strip()]
        }
    )

    fdcan_args = {
        "sjw_abr": 10,
        "tseg1_abr": 29,
        "tseg2_abr": 10,
        "sam_abr": 1,
        "sjw_dbr": 10,
        "tseg1_dbr": 29,
        "tseg2_dbr": 10,
        "output_mode": 1,
    }

    with can.ThreadSafeBus(
        interface=args.bus,
        channel=args.channel,
        fd=args.fd,
        bitrate=args.bit_rate,
        data_bitrate=args.data_bitrate,
        app_name=None,
        **(fdcan_args if args.fd else {}),
    ) as bus:
        router = can_router.CanRouter(c + boards.hexray_boards)
        with can.Notifier(bus, [router]):
            if args.erase:
                erase(bus=bus, configs=c, is_fd=args.fd, router=router)
            else:
                update(
                    bus=bus, configs=c, build_dir=args.build, is_fd=args.fd, only=args.only, router=router
                )
