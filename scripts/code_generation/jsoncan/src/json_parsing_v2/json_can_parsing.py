import json
import os
from math import ceil
from typing import Any, Tuple

from ..can_database_v2 import *
from ..utils import load_json_file, max_uint_for_bits
from .schema_validation import validate_bus_json, validate_enum_json, validate_tx_json


class InvalidCanJson(Exception):
    """
    General exception if anything is wrong with the JSON.
    """

    ...


def dir_pattern(root_dir: str, type: str, node: str):
    return f"{root_dir}/{node}/{node}_{type}"


def calc_signal_scale_and_offset(
    max_val: int, min_val: int, num_bits: int
) -> Tuple[float, float]:
    """
    Calculate scale and offset for DBC file.
    """
    scale = (max_val - min_val) / max_uint_for_bits(num_bits)
    offset = min_val
    return scale, offset


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
            bus["name"]: CanBusConfig(
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
            enums_json_data = validate_enum_json(
                load_json_file(dir_pattern(can_data_dir, "enum", node))
            )
            enum_data = {
                enum_name: CanEnum(name=enum_name, items=enum_values)
                for enum_name, enum_values in enums_json_data.items()
            }

            self._enums.update(enum_data)

            # Parse tx
            tx_json_data = validate_tx_json(
                load_json_file(dir_pattern(can_data_dir, "tx", node)),
                self._bus_cfg,
                self._enums,
            )
            tx_data = self._build_can_message(tx_json_data, node)
            self._messages.update(tx_data)

            # parse alerts
            alerts_json_data = validate_alerts_json(
                load_json_file(dir_pattern(can_data_dir, "alerts", node))
            )

    def _build_can_message(self, tx_json_data, node) -> list[CanMessage]:
        assert self._enums is not None
        assert self._bus_cfg is not None
        assert self._nodes is not None

        messages = []
        for message in tx_json_data:

            if message["disabled"] == True:
                continue

            msg_id = message["id"]
            msg_name = message["name"]
            description = self._get_optional_value(tx_json_data, "description", "")
            msg_cycle_time = tx_json_data["cycle_time"]
            bus = tx_json_data["bus"]

            # FIXME: change it later
            msg_modes = self._get_optional_value(
                tx_json_data, "allowed_modes", ["default"]
            )

            log_cycle_time = msg_cycle_time
            telem_cycle_time = msg_cycle_time
            if "data_capture" in tx_json_data:
                log_cycle_time = tx_json_data["data_capture"]["log_cycle_time"]
                telem_cycle_time = tx_json_data["data_capture"]["telem_cycle_time"]

            signals = []
            next_available_bit = 0
            occupied_bits = [None] * 64

            for signal in message["signals"]:
                signal_obj = self._build_can_signal(
                    signal, next_available_bit, occupied_bits
                )

            signals.append(signal)
            next_available_bit += signal.bits

        return CanMessage(
            name=msg_name,
            id=msg_id,
            description=description,
            signals=signals,
            cycle_time=msg_cycle_time,
            tx_node=node,
            rx_nodes=[
                self._bus_cfg.default_receiver
            ],  # Every msg is received by the default receiver
            modes=msg_modes,
            log_cycle_time=log_cycle_time,
            telem_cycle_time=telem_cycle_time,
        )

    def _build_can_signal(
        self, signal_json, next_available_bit, occupied_bits
    ) -> CanSignal:
        assert self._enums is not None

        signal_name = signal_json["name"]
        max_val = 0
        min_val = 0
        scale = 0
        offset = 0
        bits = 0
        enum = None

        unit, _ = self._get_optional_value(signal_json, "unit", "")
        start_bit, specified_start_bit = self._get_optional_value(
            signal_json, "start_bit", next_available_bit
        )

        signed, _ = self._get_optional_value(signal_json, "signed", False)

        # Get signal value data. Method depends on which data provided in JSON file.
        # Option 1: Provide DBC data
        if all(
            datum in signal_json for datum in ("min", "max", "scale", "offset", "bits")
        ):
            bits = signal_json["bits"]
            max_val = signal_json["max"]
            min_val = signal_json["min"]
            scale = signal_json["scale"]
            offset = signal_json["offset"]

        # Option 2: Provide min, max, and bit length. Scale and offset are calculated.
        elif all(datum in signal_json for datum in ("min", "max", "bits")):
            max_val = signal_json["max"]
            min_val = signal_json["min"]
            bits = signal_json["bits"]

            scale, offset = calc_signal_scale_and_offset(
                max_val=max_val, min_val=min_val, num_bits=bits
            )

        # Option 3: Provide min, max, and resolution (scale). Offset and bit length are calculated.
        elif all(datum in signal_json for datum in ("min", "max", "resolution")):
            max_val = signal_json["max"]
            min_val = signal_json["min"]
            sig_resolution = signal_json["resolution"]
            scale = sig_resolution
            offset = min_val

            max_raw_val = ceil((max_val - min_val) / sig_resolution)
            bits = max_raw_val.bit_length()

        # Option 4: Provide an enum. Min, max, bits, have to be calculated.
        # Scale and offset are assumed to be 1 and 0, respectively (so start your enums at 0!)
        elif "enum" in signal_json:
            enum_name = signal_json["enum"]
            if enum_name not in self._enums:
                raise InvalidCanJson(
                    f"Signal '{signal_name}' requests an enum named '{enum_name}', but an enum by that name was not defined."
                )

            enum = self._enums[enum_name]
            max_val = enum.max_val()
            min_val = enum.min_val()
            bits = enum.num_bits()
            scale = 1
            offset = 0

        # Option 5: Just provide bits, and will be considered to be an unsigned int of however many bits.
        elif "bits" in signal_json:
            bits = signal_json["bits"]
            if signed:
                max_val = max_uint_for_bits(bits - 1) - 1
                min_val = -max_uint_for_bits(bits - 1)
            else:
                max_val = max_uint_for_bits(bits)
                min_val = 0
            scale = 1
            offset = 0

        # Otherwise, payload data was not inputted correctly
        else:
            raise InvalidCanJson(
                f"Signal '{signal_name}' has invalid payload representation, and could not be parsed."
            )

        # Parse start value
        start_val, _ = self._get_optional_value(signal_json, "start_value", min_val)

        # Signals can"t be longer than 32 bits, to maintain atomic read/write
        if bits < 1 or bits > 32:
            raise InvalidCanJson(
                f"Signal '{signal_name}' has invalid bit length {bits}. Bit length must be between 1 and 32."
            )

        return (
            CanSignal(
                name=signal_name,
                bits=bits,
                scale=scale,
                offset=offset,
                min_val=min_val,
                max_val=max_val,
                start_bit=start_bit,
                unit=unit,
                enum=enum,
                start_val=start_val,
                signed=signed,
            ),
            specified_start_bit,
        )

    def _build_alerts(self, alerts_json_data, node) -> dict[CanAlert, AlertsEntry]:
        alerts = {}
        for alert in alerts_json_data:
            alert_name = alert["name"]
            alert_id = alert["id"]
            alert_desc = alert["description"]
            alert_severity = alert["severity"]
            alert_type = alert["type"]
            alert_ack = alert["ack"]
            alert_data = alert["data"]

            alerts[alert_name] = AlertsEntry(
                name=alert_name,
                id=alert_id,
                description=alert_desc,
                severity=alert_severity,
                type=alert_type,
                ack=alert_ack,
                data=alert_data,
            )

        return alerts

    @staticmethod
    def _get_optional_value(data: Dict, key: str, default: any) -> (str or any, bool):
        """
        Parse a value from a key in data. If key not found, return default.
        """
        if key in data:
            return data[key], True
        else:
            return default, False
