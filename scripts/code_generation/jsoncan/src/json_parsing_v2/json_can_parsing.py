import json
import os
from math import ceil
from typing import Any, Tuple

from ..can_database_v2 import *
from ..utils import load_json_file, max_uint_for_bits
from .schema_validation import (validate_bus_json, validate_enum_json,
                                validate_tx_json)


def dir_pattern(root_dir:str, type:str, node:str):
    return f"{root_dir}/{node}/{node}_{type}"

class JsonCanParser:
    def __init__(self, can_data_dir: str):
        self._bus_cfg: dict[str, CanBusConfig] = {}
        self._nodes: list[str] = []  # List of node names
        self._messages: dict[str, CanMessage] = {}  # Dict of msg names to msg objects
        self._enums: dict[str, CanEnum] = {}  # Dict of enum names to enum objects
        self._shared_enums: list[CanEnum] = []  # Set of shared enums
        self._alerts: dict[str, dict[CanAlert, AlertsEntry]] = (
            {}
        )  # Dict of node names to node's alerts
        self._alert_descriptions = {}  # TODO this is not used

        self._parse_json_data(can_data_dir=can_data_dir)
        # parse the json and construct the CanMessage object

    def _parse_json_data(self, can_data_dir: str):
        # Get all the nodes
        self._nodes = [f.name for f in os.scandir(can_data_dir) if f.is_dir()]

        # Parse 
        buses_json_data = validate_bus_json(load_json_file(f"{can_data_dir}/bus"))
        self._bus_cfg = {
            bus["name"] : CanBusConfig(
                default_receiver=bus["default_receiver"],
                bus_speed=bus["bus_speed"],
                modes=bus["modes"],
                default_mode=bus["default_mode"],
                name=bus["name"],
            )
            for bus in buses_json_data
        }

        # for each node, parse enums, tx, rx, and alerts
        for node in self._nodes:
            # Parse enums
            enums_json_data = validate_enum_json(load_json_file(dir_pattern(can_data_dir, "enum", node)))
            enum_data = {
                enum_name: CanEnum(
                    name=enum_name, values=enum_values
                )
                for enum_name, enum_values in enums_json_data.items()
            }

            self._enums.update(enum_data)

            # Parse tx
            tx_json_data = validate_tx_json(load_json_file(dir_pattern(can_data_dir, "tx", node)))
            tx_data = self._build_can_message(tx_json_data, enum_data)



    def _build_can_message(self, tx_json_data) -> list[CanMessage]:
        assert self._enums is not None
        assert self._bus_cfg is not None
        assert self._nodes is not None

        ret_list = []
        # Parse the tx json data
        pass


    def _build_can_signal(self, signal_name, signal_data) -> CanSignal:
        assert self._enums is not None

        pass 


        
