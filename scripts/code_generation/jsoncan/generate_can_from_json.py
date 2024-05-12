"""
Entry point for generating CAN drivers and DBC from JSON data, as a command line utility.
TODO: Generate callback functions for received messages? Could be cool
TODO: Why do we need start values???
"""

import argparse

from src.codegen.c_generation.app_can_alerts_module import AppCanAlertsModule
from src.codegen.c_generation.app_can_rx_module import AppCanRxModule
from src.codegen.c_generation.app_can_tx_module import AppCanTxModule
from src.codegen.c_generation.app_can_utils_module import AppCanUtilsModule
from src.codegen.c_generation.io_can_rx_module import IoCanRxModule
from src.codegen.c_generation.io_can_tx_module import IoCanTxModule
from src.codegen.dbc_generation.dbc_generation import DbcGenerator
from src.json_parsing.json_can_parsing import JsonCanParser
from src.utils import write_text

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--board", help="Choose a board name")
    parser.add_argument("--can_data_dir", help="Path to JSON CAN data")
    parser.add_argument(
        "--app_can_tx_source_output",
        help="Path to the output CAN TX source file for the App layer",
    )
    parser.add_argument(
        "--app_can_tx_header_output",
        help="Path to the output CAN TX header file for the App layer",
    )
    parser.add_argument(
        "--io_can_tx_source_output",
        help="Path to the output CAN TX source file for the IO layer",
    )
    parser.add_argument(
        "--io_can_tx_header_output",
        help="Path to the output CAN TX header file for the IO layer",
    )
    parser.add_argument(
        "--app_can_rx_source_output",
        help="Path to the output CAN RX source file for the App layer",
    )
    parser.add_argument(
        "--app_can_rx_header_output",
        help="Path to the output CAN RX header file for the App layer",
    )
    parser.add_argument(
        "--io_can_rx_source_output",
        help="Path to the output CAN RX source file for the IO layer",
    )
    parser.add_argument(
        "--io_can_rx_header_output",
        help="Path to the output CAN RX header file for the IO layer",
    )
    parser.add_argument(
        "--app_can_utils_source_output",
        help="Path to the output source file for packing/unpacking CAN messages",
    )
    parser.add_argument(
        "--app_can_utils_header_output",
        help="Path to the output header file for packing/unpacking CAN messages",
    )
    parser.add_argument(
        "--app_can_alerts_source_output",
        help="Path to the output source file for aperiodic alerts",
    )
    parser.add_argument(
        "--app_can_alerts_header_output",
        help="Path to the output header file for aperiodic alerts",
    )
    parser.add_argument("--dbc_output", help="Path to the DBC file")
    parser.add_argument(
        "--only_dbc", action="store_true", help="Only generate DBC file"
    )
    args = parser.parse_args()

    # Parse JSON
    can_db = JsonCanParser(can_data_dir=args.can_data_dir).make_database()

    # Generate DBC file
    write_text(DbcGenerator(database=can_db).source(), args.dbc_output)
    if args.only_dbc:
        exit()

    # Generate app_canUtils.h/c
    app_can_utils_mod = AppCanUtilsModule(can_db, args.board)
    write_text(app_can_utils_mod.header(), args.app_can_utils_header_output)
    write_text(app_can_utils_mod.source(), args.app_can_utils_source_output)

    # Generate app_canTx.h/c
    app_can_tx_mod = AppCanTxModule(can_db, args.board)
    write_text(app_can_tx_mod.header(), args.app_can_tx_header_output)
    write_text(app_can_tx_mod.source(), args.app_can_tx_source_output)

    # Generate app_canRx.h/c
    app_can_rx_mod = AppCanRxModule(can_db, args.board)
    write_text(app_can_rx_mod.header(), args.app_can_rx_header_output)
    write_text(app_can_rx_mod.source(), args.app_can_rx_source_output)

    # Generate io_canTx.h/c
    io_can_tx_mod = IoCanTxModule(can_db, args.board)
    write_text(io_can_tx_mod.header(), args.io_can_tx_header_output)
    write_text(io_can_tx_mod.source(), args.io_can_tx_source_output)

    # Generate io_canRx.h/c
    io_can_rx_mod = IoCanRxModule(can_db, args.board)
    write_text(io_can_rx_mod.header(), args.io_can_rx_header_output)
    write_text(io_can_rx_mod.source(), args.io_can_rx_source_output)

    # Generate app_canAlerts.h/c
    app_can_alerts_mod = AppCanAlertsModule(can_db, args.board)
    write_text(app_can_alerts_mod.header(), args.app_can_alerts_header_output)
    write_text(app_can_alerts_mod.source(), args.app_can_alerts_source_output)
