"""
Functions to validate the CAN JSON schema.
"""

from dataclasses import dataclass
from typing import Dict, List, TypedDict, Optional as Optional_t
from schema import And, Optional, Or, Schema

"""
Tx file schemas
"""
tx_signal_schema = Schema(
    # 4 options to define a signal"s representation...
    Or(
        Schema({
            # Just bits, and signal will be a uint of X bits, i.e. offset=0, min=0, max=(2^bits-1)
            "bits": int,
            Optional("unit"): str,
            Optional("start_value"): int,
            Optional("start_bit"): int,
            Optional("signed"): bool,
            Optional("scale"): Or(int, float),
        }),
        Schema({
            # Bits/min/max, and signal will range from min to max in X bits, scale/offset will be calculated accordingly
            "bits": int,
            "min": Or(int, float),
            "max": Or(int, float),
            Optional("unit"): str,
            Optional("start_value"): Or(int, float),
            Optional("start_bit"): int,
        }),
        Schema({
            # Resolution/min/max, and signal will range from min to max such that scale=resolution, bits/offset will be calculated accordingly
            "resolution": Or(int, float),
            "min": Or(int, float),
            "max": Or(int, float),
            Optional("unit"): str,
            Optional("start_value"): Or(int, float),
            Optional("start_bit"): int,
        }),
        Schema({
            # Enum, signal will be generated with minimum # of bits to hold all possible enum values
            "enum": str,
            Optional("start_value"): Or(int, float),
            Optional("start_bit"): int,
        }),
        Schema({
            # Scale/offset/bits/signedness: Basically if you want to configure like a DBC file
            "scale": Or(int, float),
            "offset": Or(int, float),
            "bits": int,
            "min": Or(int, float),
            "max": Or(int, float),
            Optional("start_value"): Or(int, float),
            Optional("start_bit"): int,
            Optional("signed"): bool,
            Optional("unit"): str,
        }),
    )
)

tx_msg_schema = Schema(
    {
        "bus": list[str],
        "msg_id": And(
            int, lambda x: 0 <= x < 2 ** 11
        ),  # Standard CAN uses 11-bit identifiers
        "signals": {
            str: tx_signal_schema,
        },
        "cycle_time": Or(int, Schema(None), lambda x: x >= 0),
        Optional("disabled"): bool,
        Optional("num_bytes"): Or(int, lambda x: 0 <= x <= 8),
        Optional("description"): str,
        Optional("allowed_modes"): [str],
        Optional("data_capture"): {
            Optional("log_cycle_time"): Or(int, Schema(None)),
            Optional("telem_cycle_time"): Or(int, Schema(None)),
        },
    }
)

tx_schema = Schema(Or(Schema({str: tx_msg_schema}), Schema({})))

"""
Rx file schema
"""


class RxSchemaSingle(TypedDict):
    bus: str
    messages: list[str]


rx_schema = Schema(
    Or(
        Schema([]),  # Allow an empty list
        Schema([
            {
                "bus": str,
                "messages": [str],  # Use schema.List to define a list of strings
            }
        ]),
    )
)

"""
Enum file schema
"""
enum_schema = Schema(Or(Schema({str: {str: int}}), Schema({})))  # If the node doesn"t define any enums

"""
Bus file schema
"""


class ForwarderConfigJson(TypedDict):
    forwarder: str
    bus1: str
    bus2: str


class BusConfigJson(TypedDict):
    name: str
    bus_speed: int
    modes: list[str]
    default_mode: str
    nodes: list[str]
    # is this optional?
    default_receiver: Optional_t[str]
    FD: Optional_t[bool]


class BusJson(TypedDict):
    forwarders: list[ForwarderConfigJson]
    buses: list[BusConfigJson]


single_bus_schema = Schema(
    {
        "default_receiver": str,
        "bus_speed": int,
        "modes": [str],
        "default_mode": str,
        "nodes": [str],
        Optional("FD"): bool,
    }
)

forwarders_schema = Schema(
    Or(
        Schema({}),
        Schema({
            "forwarder": str,
            "bus1": str,
            "bus2": str,
        }),
    )
)
bus_list = Schema(Or(list[single_bus_schema], Schema([])))
bus_schema = Schema({"forwarders": list[forwarders_schema], "buses": bus_list})

"""
Alerts file schema
"""


class AlertsEntry(TypedDict):
    id: int
    description: str


class AlertsJson(TypedDict):
    warnings_id: int
    warnings_counts_id: int
    faults_id: int
    faults_counts_id: int
    info_id: int
    info_counts_id: int
    bus: List[str]
    warnings: Dict[str, AlertsEntry]
    faults: Dict[str, AlertsEntry]
    info: Dict[str, AlertsEntry]


alerts_schema = Schema(
    Or(
        Schema({
            "warnings_id": And(int, lambda x: x >= 0),
            "warnings_counts_id": And(int, lambda x: x >= 0),
            "faults_id": And(int, lambda x: x >= 0),
            "faults_counts_id": And(int, lambda x: x >= 0),
            "info_id": And(int, lambda x: x >= 0),
            "info_counts_id": And(int, lambda x: x >= 0),
            "bus": list[str],
            "warnings": Or(
                Schema({}),
                Schema({
                    str: Or(
                        Schema({}),
                        Schema({
                            "id": int,
                            "description": str,
                            Optional("disabled"): bool,
                        }),
                    )
                }),
            ),
            "faults": Or(
                Schema({}),
                Schema({
                    str: Or(
                        Schema({}),
                        Schema({
                            "id": int,
                            "description": str,
                            Optional("disabled"): bool,
                        }),
                    )
                }),
            ),
            "info": Or(
                Schema({}),
                Schema({
                    str: Or(
                        Schema({}),
                        Schema({
                            "id": int,
                            "description": str,
                            Optional("disabled"): bool,
                        }),
                    )
                }),
            ),
        }),
        Schema({}),
    )
)


def validate_tx_json(json: Dict) -> Dict[str, dict]:
    return tx_schema.validate(json)


def validate_rx_json(json: Dict) -> list[RxSchemaSingle]:
    return rx_schema.validate(json)


def validate_enum_json(json: Dict) -> Dict[str, Dict[str, int]]:
    return enum_schema.validate(json)


def validate_bus_json(json: Dict) -> BusJson:
    return bus_schema.validate(json)


def validate_alerts_json(json: Dict) -> AlertsJson:
    return alerts_schema.validate(json)
