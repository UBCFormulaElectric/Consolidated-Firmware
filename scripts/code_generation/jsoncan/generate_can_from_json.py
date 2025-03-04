"""
Entry point for generating CAN drivers and DBC from JSON data, as a command line utility.
TODO: Generate callback functions for received messages? Could be cool
TODO: Why do we need start values???
"""

import argparse
import os

from calc_bus_load import report_bus_load
from src.codegen.c_generation.app_can_alerts_module import AppCanAlertsModule
from src.codegen.c_generation.app_can_data_capture_module import \
    AppCanDataCaptureModule
from src.codegen.c_generation.app_can_rx_module import AppCanRxModule
from src.codegen.c_generation.app_can_tx_module import AppCanTxModule
from src.codegen.c_generation.app_can_utils_module import AppCanUtilsModule
from src.codegen.c_generation.io_can_reroute_module import IoCanRerouteModule
from src.codegen.c_generation.io_can_rx_module import IoCanRxModule
from src.codegen.c_generation.io_can_tx_module import IoCanTxModule
from src.codegen.dbc_generation.dbc_generation import DbcGenerator
from src.json_parsing.json_can_parsing import JsonCanParser
from src.utils import write_text

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--board", help="Choose a board name")
    parser.add_argument("--can_data_dir", help="Path to JSON CAN data")
    parser.add_argument("--output_dir", help="Path to the output source files")
    parser.add_argument("--dbc_output", help="Path to the DBC file")
    parser.add_argument(
        "--only_dbc", action="store_true", help="Only generate DBC file"
    )
    args = parser.parse_args()

    # Parse JSON
    can_db = JsonCanParser(can_data_dir=args.can_data_dir).make_database()
    pandas = can_db.make_pandas_dataframe()
    print(pandas)
    # Generate DBC file
    write_text(DbcGenerator(database=can_db).source(), args.dbc_output)
    if args.only_dbc:
        exit()

    modules = {
        AppCanUtilsModule(can_db, args.board): os.path.join("app", "app_canUtils"),
        AppCanTxModule(can_db, args.board): os.path.join("app", "app_canTx"),
        AppCanRxModule(can_db, args.board): os.path.join("app", "app_canRx"),
        AppCanAlertsModule(can_db, args.board): os.path.join("app", "app_canAlerts"),
        IoCanTxModule(can_db, args.board): os.path.join("io", "io_canTx"),
        IoCanRxModule(can_db, args.board): os.path.join("io", "io_canRx"),
        AppCanDataCaptureModule(can_db): os.path.join("app", "app_canDataCapture"), 
        # IoCanRerouteModule(can_db, args.board): os.path.join("io", "io_canReroute"),
    }
    for module, module_path in modules.items():
        module_full_path = os.path.join(args.output_dir, module_path)
        write_text(module.header_template(), module_full_path + ".h")
        write_text(module.source_template(), module_full_path + ".c")
