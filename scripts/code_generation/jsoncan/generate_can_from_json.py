"""
Entry point for generating CAN drivers and DBC from JSON data, as a command line utility.

"""

import argparse
import os

from .src.codegen.c_generation.cmodule import CModule
from .src.codegen.c_generation.app_can_alerts_module import AppCanAlertsModule
from .src.codegen.c_generation.app_can_data_capture_module import AppCanDataCaptureModule
from .src.codegen.c_generation.app_can_rx_module import AppCanRxModule
from .src.codegen.c_generation.app_can_tx_module import AppCanTxModule
from .src.codegen.c_generation.app_can_utils_module import AppCanUtilsModule
from .src.codegen.c_generation.io_can_reroute_module import IoCanRerouteModule
from .src.codegen.c_generation.io_can_rx_module import IoCanRxModule
from .src.codegen.c_generation.io_can_tx_module import IoCanTxModule
from .src.codegen.dbc_generation.dbc_generation import DbcGenerator
from .src.json_parsing.json_can_parsing import JsonCanParser


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
    with open(os.path.join(output_dir, output_name), "w") as fout:
        fout.write(text)


def generate_can_from_json(can_data_dir: str, dbc_output: str, only_dbc: bool, board: str, output_dir: str):
    # Parse JSON
    can_db = JsonCanParser(can_data_dir=can_data_dir).make_database()
    # pandas = can_db.make_pandas_dataframe()
    # print(pandas)
    # Generate DBC file
    write_text(DbcGenerator(database=can_db).source(), dbc_output)
    if only_dbc:
        exit()

    modules: list[tuple[CModule, str]] = [
        (AppCanUtilsModule(can_db, board), os.path.join("app", "app_canUtils")),
        (AppCanTxModule(can_db, board), os.path.join("app", "app_canTx")),
        (AppCanRxModule(can_db, board), os.path.join("app", "app_canRx")),
        (IoCanTxModule(can_db, board), os.path.join("io", "io_canTx")),
        (IoCanRxModule(can_db, board), os.path.join("io", "io_canRx")),
        # TODO only generate this if the current node can capture data
        (AppCanDataCaptureModule(can_db), os.path.join("app", "app_canDataCapture")),
        # TODO only generate this if node has alerts. this is less priority because all nodes generate alerts
        (AppCanAlertsModule(can_db, board), os.path.join("app", "app_canAlerts")),
        # TODO only do this if the current node is a rerouter
        (IoCanRerouteModule(can_db, board), os.path.join("io", "io_canReroute")),
    ]
    for module, module_path in modules:
        module_full_path = os.path.join(output_dir, module_path)
        write_text(module.header_template(), module_full_path + ".h")
        write_text(module.source_template(), module_full_path + ".c")


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
    generate_can_from_json(args.can_data_dir, args.dbc_output, args.only_dbc, args.board, args.output_dir)
