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

import boards
import bootloader

console = Console()
status = console.status("Status")
progress = Progress(
    TextColumn("[progress.description]{task.description}"),
    BarColumn(),
    DownloadColumn(),
)


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
        if not bootload_board.goto_bootloader():
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
        if not bootload_board.goto_app():
            raise TimeoutError(
                "Failed to send application command to {bootload_board.board.name}"
            )
    progress.remove_task(app_task)
    live.console.log(
        f"[bold green]All boards pushed out of bootloader mode successfully"
    )


def update(configs: List[boards.Board], build_dir: str, is_fd: bool) -> None:
    """Update and handle UI."""
    num_boards = len(configs)
    steps_task = progress.add_task("Steps")
    bootloaders: List[bootloader.Bootloader] = [
        bootloader.Bootloader(
            bus=bus,
            board=board,
            ui_callback=lambda description, total, completed: progress.update(
                task_id=steps_task,
                total=total,
                description=description,
                completed=completed,
                is_fd=is_fd,
            ),
            ih=intelhex.IntelHex(os.path.join(build_dir, board.path)),
        )
        for board in configs
    ]

    # push all boards into bootloader
    with Live(Group(status, progress), transient=True) as live:
        # push all boards into bootloader
        all_goto_bootloader(live, bootloaders)
        live.console.log(
            f"Updating firmware for boards: [blue bold]{', '.join(board.name for board in configs)}"
        )
        for b_idx, bootload_board in enumerate(bootloaders):
            # TODO do this in parallel
            progress.update(
                task_id=steps_task,
                total=0,
                completed=0,
                description=f"Starting update for {bootload_board.board.name}",
            )
            status.update(
                f"Updating board [yellow]{b_idx + 1}/{num_boards}[/]: [blue bold]{bootload_board.board.name}"
            )
            bootload_board.update()
            live.console.log(f"[green]{bootload_board.board.name} updated successfully")
        progress.remove_task(steps_task)
        live.console.log(
            f"[bold green]Firmware update successfully ({num_boards} board{'s' if num_boards > 1 else ''} updated)"
        )
        # push all boards out of bootloader
        all_goto_app(live, bootloaders)


def erase(configs: List[boards.Board]) -> None:
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
    parser.add_argument("--bus", type=str, default="pcan", help="python-can bus type")
    parser.add_argument(
        "--channel", type=str, default="PCAN_USBBUS1", help="python-can channel"
    )
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
        "--data_bitrate", type=int, default=4000000, help="CAN FD data bitrate"
    )
    args = parser.parse_args()

    # Load config and binary.
    c = list(
        {
            board
            for config_name in args.config.split(",")
            for board in boards.CONFIGS[config_name.strip()]
        }
    )

    fdcan_args = {
        "sjw_abr": 20,
        "tseg1_abr": 59,
        "tseg2_abr": 20,
        "sam_abr": 1,
        "sjw_dbr": 20,
        "tseg1_dbr": 59,
        "tseg2_dbr": 20,
        "output_mode": 0,
    }
    with can.interface.Bus(
        interface=args.bus,
        channel=args.channel,
        fd=args.fd,
        bitrate=args.bit_rate,
        data_bitrate=args.data_bitrate,
        app_name = None,
        **(fdcan_args if args.fd else {})
    ) as bus:
        if args.erase:
            erase(configs=c, is_fd=args.fd)
        else:
            update(configs=c, build_dir=args.build, is_fd=args.fd)
