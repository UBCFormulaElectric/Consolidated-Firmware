"""
Entry point for generating CAN drivers and DBC from JSON data, as a command line utility.

"""

import argparse
import os

from . import (
    AppCanAlertsModule,
    AppCanDataCaptureModule,
    AppCanRxModule,
    AppCanTxModule,
    AppCanUtilsModule,
    CModule,
    DbcGenerator,
    IoCanRerouteModule,
    IoCanRxModule,
    IoCanTxModule,
    JsonCanParser,
)
from .src.codegen.c_generation.routing import resolve_tx_rx_reroute


def write_text(text: str, output_path: str) -> None:
    """
    Write the text stored in text to output_path.
    """
    output_dir = (
        os.getcwd()
        if os.path.dirname(output_path) == ""
        else os.path.dirname(output_path)
    )
    output_name = os.path.basename(output_path)

    # Generate output folder if it doesn't exist yet
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Write file to disk
    with open(os.path.join(output_dir, output_name), "w") as cpp_file:
        cpp_file.write(text)


def generate_can_from_json(
    can_data_dir: str, dbc_output: str, only_dbc: bool, board: str, output_dir: str
):
    # Parse JSON
    can_db = JsonCanParser(can_data_dir=can_data_dir).make_database()
    tx_configs, rx_configs, reroute_config = resolve_tx_rx_reroute(can_db)

    # Generate DBC file
    write_text(
        DbcGenerator(database=can_db, rx_configs=rx_configs).source(), dbc_output
    )
    if only_dbc:
        return

    if board not in can_db.nodes:
        raise ValueError(f"Board {board} not found in CAN database.")
    # NOTE that not all files are required, but it's very hard to communicate to cmake at generate time
    # which files are required.
    modules: list[tuple[CModule, str]] = [
        (
            AppCanUtilsModule(can_db, tx_configs[board], rx_configs[board]),
            os.path.join("app", "app_canUtils"),
        ),
        (AppCanTxModule(can_db, tx_configs[board]), os.path.join("app", "app_canTx")),
        (AppCanAlertsModule(can_db, board), os.path.join("app", "app_canAlerts")),
        (AppCanDataCaptureModule(can_db), os.path.join("app", "app_canDataCapture")),
        (
            AppCanRxModule(can_db, board, rx_configs[board]),
            os.path.join("app", "app_canRx"),
        ),
        (
            IoCanTxModule(can_db, board, tx_configs[board]),
            os.path.join("io", "io_canTx"),
        ),
        (
            IoCanRxModule(can_db, board, rx_configs[board]),
            os.path.join("io", "io_canRx"),
        ),
        (
            IoCanRerouteModule(can_db, board, reroute_config.get(board)),
            os.path.join("io", "io_canReroute"),
        ),
    ]

    for module, module_path in modules:
        module_full_path = os.path.join(output_dir, module_path)
        write_text(module.header_template(), module_full_path + ".h")
        write_text(module.source_template(), module_full_path + ".cpp")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--board", help="Choose a board name"
    )  # can be none when onlyh dbc
    parser.add_argument("--can_data_dir", help="Path to JSON CAN data", required=True)
    parser.add_argument("--output_dir", help="Path to the output source files")
    parser.add_argument("--dbc_output", help="Path to the DBC file", required=True)
    parser.add_argument(
        "--only_dbc", action="store_true", help="Only generate DBC file"
    )
    args = parser.parse_args()
    if not args.only_dbc:
        if not args.board or not args.output_dir:
            parser.error(
                "--board and --output_dir are required unless --only_dbc is set."
            )
    generate_can_from_json(
        args.can_data_dir, args.dbc_output, args.only_dbc, args.board, args.output_dir
    )
