# logic for validate json
from ..can_database_v2 import *
from ..utils import load_json_file

"""
Functions to validate the CAN JSON schema.
"""

from typing import Dict, List, TypedDict, Union

from schema import And, Optional, Or, Schema

# static schema validation of json
"""
Tx file schemas
"""
tx_signal_schema = Schema(
    # 4 options to define a signal"s representation...
    Or(
        {
            # Just bits, and signal will be a uint of X bits, i.e. scale=1, offset=0, min=0, max=(2^bits-1)
            "name": str,
            "bits": int,
            Optional("unit"): str,
            Optional("start_value"): int,
            Optional("start_bit"): int,
            Optional("signed"): bool,
        },
        {
            # Bits/min/max, and signal will range from min to max in X bits, scale/offset will be calculated accordingly
            "name": str,
            "bits": int,
            "min": Or(int, float),
            "max": Or(int, float),
            Optional("unit"): str,
            Optional("start_value"): Or(int, float),
            Optional("start_bit"): int,
        },
        {
            # Resolution/min/max, and signal will range from min to max such that scale=resolution, bits/offset will be calculated accordingly
            "name": str,
            "resolution": Or(int, float),
            "min": Or(int, float),
            "max": Or(int, float),
            Optional("unit"): str,
            Optional("start_value"): Or(int, float),
            Optional("start_bit"): int,
        },
        {
            # Enum, signal will be generated with minimum # of bits to hold all possible enum values
            "name": str,
            "enum": str,
            Optional("start_value"): Or(int, float),
            Optional("start_bit"): int,
        },
        {
            # Scale/offset/bits/signedness: Basically if you want to configure like a DBC file
            "name": str,
            "scale": Or(int, float),
            "offset": Or(int, float),
            "bits": int,
            "min": Or(int, float),
            "max": Or(int, float),
            Optional("start_value"): Or(int, float),
            Optional("start_bit"): int,
            Optional("signed"): bool,
            Optional("unit"): str,
        },
    )
)

tx_msg_schema = Schema(
    {
        "name": str,
        "msg_id": And(
            int, lambda x: x >= 0 and x < 2**11
        ),  # Standard CAN uses 11-bit identifiers
        "bus": list[str],
        "signals": list[tx_signal_schema],
        "cycle_time": Or(int, None, lambda x: x >= 0),
        Optional("disabled"): bool,
        Optional("num_bytes"): Or(int, lambda x: x >= 0 and x <= 8),
        Optional("description"): str,
        Optional("allowed_modes"): [str],
        Optional("data_capture"): {
            Optional("log_cycle_time"): Or(int, None),
            Optional("telem_cycle_time"): Or(int, None),
        },
    }
)

tx_schema = Schema(Or([tx_msg_schema], []))

"""
Rx file schema
"""


class RxSchema(TypedDict):
    messages: list[str]


rx_schema = Schema({"messages": [str]})

"""
Enum file schema
"""


class EnumEntry(TypedDict):
    name: str
    value: int


enum_schema = Schema(Or({str: {str: int}}, {}))  # If the node doesn"t define any enums

"""
Bus file schema
"""


class BusJson(TypedDict):
    default_receiver: str
    bus_speed: int
    modes: list[str]
    default_mode: str
    name: str


bus_schema = Or(
    [
        {
            "name": str,
            "default_receiver": str,
            "bus_speed": And(
                int, lambda x: x > 0
            ),  # Ensures bus_speed is a positive integer
            "modes": [str],  # Use `list` instead of `typing.List`
            "default_mode": str,
        }
    ],
    [],  # Allow an empty list as a valid input
)

"""
Alerts file schema
"""


class AlertsJson(TypedDict):
    warnings_id: int
    warnings_counts_id: int
    faults_id: int
    faults_counts_id: int
    warnings: List[AlertsEntry]
    faults: List[AlertsEntry]


alerts_schema = Schema(
    Or(
        {
            "warnings_id": And(int, lambda x: x >= 0),
            "warnings_counts_id": And(int, lambda x: x >= 0),
            "faults_id": And(int, lambda x: x >= 0),
            "faults_counts_id": And(int, lambda x: x >= 0),
            "warnings": Or(
                [],
                Or(
                    {},
                    {
                        "name": str,
                        "id": int,
                        "description": str,
                        Optional("disabled"): bool,
                    },
                ),
            ),
            "faults": Or(
                [],
                [
                    Or(
                        {},
                        {
                            "name": str,
                            "id": int,
                            "description": str,
                            Optional("disabled"): bool,
                        },
                    )
                ],
            ),
        },
        {},
    )
)


def schema_validate_tx_json(json: Dict) -> Dict[str, dict]:
    return tx_schema.validate(json)


def schema_validate_rx_json(json: Dict) -> RxSchema:
    return rx_schema.validate(json)


def schema_validate_enum_json(json: Dict) -> Dict[str, Dict[str, int]]:
    return enum_schema.validate(json)


def schema_validate_bus_json(json: Dict) -> BusJson:
    return bus_schema.validate(json)


def schema_validate_alerts_json(json: Dict) -> AlertsJson:
    return alerts_schema.validate(json)


def check_repeat_names(signals_json: List[Dict]) -> bool:
    seen = set()
    for signal in signals_json:
        name = signal["name"]
        if name in seen:
            return False
        seen.add(name)
    return True


# dynamic validation of json


# validate bus config
def dynamic_validate_bus(bus_json: List[Dict]) -> bool:
    # check if bus speed is valid
    if not check_repeat_names(bus_json):
        return False

    for bus in bus_json:
        modes = bus["modes"]
        default_mode = bus["default_mode"]

        if default_mode not in modes:
            return False

    return True


def dynamic_validate_enum(enum_json: Dict) -> bool:
    # no repeated names
    names = set()
    for enum_name, enum_entries in enum_json.items():
        # check duplicate names
        if enum_name in names:
            return False
        set.add(enum_name)

        # check duplicate values and entries names
        enum_values = set()
        enum_entries_names = set()
        for entry_name, entry_value in enum_entries.items():
            if entry_name in enum_entries_names or entry_value in enum_values:
                return False
            enum_values.add(entry_value)
            enum_entries_names.add(entry_name)

    return True


# validate signals from one message
def dynamic_validate_tx_signals(signals_json: Dict, enum: dict[str, CanEnum]) -> bool:
    # no repeated names
    if not check_repeat_names(signals_json):
        return False

    # if in enum, then enum must exist
    for signal in signals_json:
        if "enum" in signal:
            if signal["enum"] not in enum:
                return False

    # check bits are smaller than 64
    total_bits = 0
    for signal in signals_json:
        if "bits" in signal:
            total_bits += signal["bits"]
        elif "emum" in signal:
            total_bits += enum[signal["enum"]].num_bits()

    if total_bits > 64:
        return False

    # if start bit is specified, then all signals must have start bit
    start_bit_specified = False
    for signal in signals_json:
        if "start_bit" in signal:
            start_bit_specified = True
            break

    if start_bit_specified:
        for signal in signals_json:
            if "start_bit" not in signal:
                return False

    return True


# validate a single message
def dynamic_validate_tx_message(
    tx_message_json, buses: dict[str, CanBusConfig], enum: dict[str, CanEnum]
) -> bool:
    # no repeated names
    if not check_repeat_names(tx_message_json["signals"]):
        return False

    # check if cycle time is valid
    if "cycle_time" in tx_message_json:
        if tx_message_json["cycle_time"] < 0:
            return False

    # check if bus is valid
    for bus in tx_message_json["bus"]:
        if bus not in buses.keys():
            return False

    # check if signals are valid
    if not dynamic_validate_tx_signals(tx_message_json["signals"], enum):
        return False

    return True


def validate_bus_json(json: Dict) -> List[BusJson]:
    data = schema_validate_bus_json(json)
    if not dynamic_validate_bus(data):
        raise ValueError("Invalid bus config")
    return data


def validate_enum_json(json: Dict) -> Dict[str, Dict[str, int]]:
    data = schema_validate_enum_json(json)
    if not dynamic_validate_enum(data):
        raise ValueError("Invalid enum config")
    return data


def validate_tx_json(
    json: Dict, buses: dict[str, CanBusConfig], enum: dict[str, CanEnum]
) -> Dict[str, dict]:
    data = schema_validate_tx_json(json)
    for message in data:
        if not dynamic_validate_tx_message(message, buses, enum):
            raise ValueError("Invalid message")
    return data

def validate_alerts_json(json: Dict) -> AlertsJson:
    data = schema_validate_alerts_json(json)
    return data
