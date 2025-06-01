from typing import TypedDict, Optional as Optional_t, Dict, List

from schema import Schema, Or, And, Optional, SchemaError

from .parse_error import InvalidCanJson
from .parse_utils import load_json_file
from ..can_database import CanAlertType, CanSignal, CanMessage, CanAlert

WARNINGS_ALERTS_CYCLE_TIME = 1000  # 1Hz
FAULTS_ALERTS_CYCLE_TIME = 100  # 10Hz
INFO_ALERTS_CYCLE_TIME = 100  # 10Hz TODO figure out a good number for this


class _AlertsEntryJson(TypedDict):
    id: int
    description: str
    disabled: Optional_t[bool]


class _AlertsJson(TypedDict):
    warnings_id: int
    warnings_counts_id: int
    faults_id: int
    faults_counts_id: int
    info_id: int
    info_counts_id: int
    # bus: List[str]
    warnings: Dict[str, _AlertsEntryJson]
    faults: Dict[str, _AlertsEntryJson]
    info: Dict[str, _AlertsEntryJson]


_AlertsJson_schema = Schema(
    Or(
        Schema(
            {
                "warnings_id": And(int, lambda x: x >= 0),
                "warnings_counts_id": And(int, lambda x: x >= 0),
                "faults_id": And(int, lambda x: x >= 0),
                "faults_counts_id": And(int, lambda x: x >= 0),
                "info_id": And(int, lambda x: x >= 0),
                "info_counts_id": And(int, lambda x: x >= 0),
                # TODO why does this need to exist? - to specify which bus/buses to send the alerts same as tx messages
                "warnings": Or(
                    Schema({}),
                    Schema(
                        {
                            str: Or(
                                Schema({}),
                                Schema(
                                    {
                                        "id": int,
                                        "description": str,
                                        Optional("disabled"): bool,
                                    }
                                ),
                            )
                        }
                    ),
                ),
                "faults": Or(
                    Schema({}),
                    Schema(
                        {
                            str: Or(
                                Schema({}),
                                Schema(
                                    {
                                        "id": int,
                                        "description": str,
                                        Optional("disabled"): bool,
                                    }
                                ),
                            )
                        }
                    ),
                ),
                "info": Or(
                    Schema({}),
                    Schema(
                        {
                            str: Or(
                                Schema({}),
                                Schema(
                                    {
                                        "id": int,
                                        "description": str,
                                        Optional("disabled"): bool,
                                    }
                                ),
                            )
                        }
                    ),
                ),
            }
        ),
        Schema({}),
    )
)


def _validate_alerts_json(json: Dict) -> _AlertsJson:
    return _AlertsJson_schema.validate(json)


def _parse_node_alert_signals(
    node: str, alerts: dict[str, _AlertsEntryJson], alert_type: CanAlertType
) -> tuple[dict[str, _AlertsEntryJson], list[CanSignal]]:
    """
    From a list of strings of alert names, return a list of CAN signals that will make up the frame for an alerts msg.
    :returns metadata which maps names of alert signals to their AlertsEntry, list of CanSignals for the alert message
    """
    signals: list[CanSignal] = []
    meta_data: dict[str, _AlertsEntryJson] = {}
    bit_pos = 0

    for alerts_name, alerts_entry in alerts.items():
        signals.append(
            CanSignal(
                name=f"{node}_{alert_type}_{alerts_name}",
                start_bit=bit_pos,
                bits=1,
                scale=1,
                offset=0,
                min_val=0,
                max_val=1,
                start_val=0,
                enum=None,
                unit="",
                signed=False,
            )
        )

        bit_pos += 1
        meta_data[f"{node}_{alert_type}_{alerts_name}"] = alerts_entry
    return meta_data, signals


def _parse_node_alert_count_signals(
    node: str, alerts: dict[str, _AlertsEntryJson], alert_type: str
) -> List[CanSignal]:
    """
    From a list of strings of alert names, return a list of CAN signals.
    Each signal will represent the number of times the corresponding alert has been set.
    :returns a list of signals representing the signals carrying alert counts
    """
    ALERT_COUNT_BITS = (
        3  # TODO move this up, this is a very important configuration constant
    )
    return [
        CanSignal(
            name=f"{node}_{alert_type}_{alert}Count",
            start_bit=i * ALERT_COUNT_BITS,
            bits=ALERT_COUNT_BITS,
            scale=1,
            offset=0,
            min_val=0,
            max_val=2**ALERT_COUNT_BITS - 1,
            start_val=0,
            enum=None,
            unit="",
            signed=False,
        )
        for i, alert in enumerate(alerts)
    ]


def _parse_node_alerts(
    node: str, alerts_json: _AlertsJson
) -> tuple[
    tuple[CanMessage, CanMessage, CanMessage, CanMessage, CanMessage, CanMessage],
    tuple[
        dict[str, _AlertsEntryJson],
        dict[str, _AlertsEntryJson],
        dict[str, _AlertsEntryJson],
    ],
]:
    node_name = node
    """
    Parse JSON data dictionary representing a node's alerts.
    """
    warnings = {
        name: alert
        for name, alert in alerts_json["warnings"].items()
        if not alert.get("disabled", False)
    }
    faults = {
        name: alert
        for name, alert in alerts_json["faults"].items()
        if not alert.get("disabled", False)
    }
    info = {
        name: alert
        for name, alert in alerts_json["info"].items()
        if not alert.get("disabled", False)
    }

    # Number of alerts can't exceed 21. This is because we transmit a "counts" message for faults and warnings
    # that indicate the number of times an alert has been set. Each signal is allocated 3 bits, and so can count
    # up to 8, meaning we can pack 21 alerts to fit inside a 64-bit CAN payload.
    if max(len(warnings), len(faults), len(info)) > 21:
        raise InvalidCanJson(
            f"Number of alerts for node '{node_name}' cannot exceed 21. Check that warnings, faults and infos have at most 21 items each"
        )

    # Check alert messages ID are unique
    warnings_name = f"{node_name}_Warnings"
    faults_name = f"{node_name}_Faults"
    warnings_counts_name = f"{node_name}_WarningsCounts"
    faults_counts_name = f"{node_name}_FaultsCounts"
    info_name = f"{node_name}_Info"
    info_counts_name = f"{node_name}_InfoCounts"

    # Make alert signals
    warnings_meta_data, warnings_signals = _parse_node_alert_signals(
        node_name, warnings, CanAlertType.WARNING
    )
    faults_meta_data, faults_signals = _parse_node_alert_signals(
        node_name, faults, CanAlertType.FAULT
    )
    info_meta_data, info_signals = _parse_node_alert_signals(
        node_name, info, CanAlertType.INFO
    )
    warnings_counts_signals = _parse_node_alert_count_signals(
        node_name, warnings, CanAlertType.WARNING
    )
    faults_counts_signals = _parse_node_alert_count_signals(
        node_name, faults, CanAlertType.FAULT
    )
    info_counts_signals = _parse_node_alert_count_signals(
        node_name, info, CanAlertType.INFO
    )

    alerts_msgs: tuple[
        CanMessage, CanMessage, CanMessage, CanMessage, CanMessage, CanMessage
    ] = tuple(
        CanMessage(
            name=name,
            id=msg_id,
            description=description,
            cycle_time=cycle_time,
            log_cycle_time=cycle_time,
            telem_cycle_time=cycle_time,
            signals=signals,
            tx_node_name=node,
            modes=None,
        )
        for name, msg_id, description, signals, cycle_time in [
            (
                warnings_name,
                (alerts_json["warnings_id"]),
                f"Status of warnings for the {node_name}.",
                warnings_signals,
                WARNINGS_ALERTS_CYCLE_TIME,
            ),
            (
                faults_name,
                (alerts_json["faults_id"]),
                f"Status of faults for the {faults}.",
                faults_signals,
                FAULTS_ALERTS_CYCLE_TIME,
            ),
            (
                warnings_counts_name,
                (alerts_json["warnings_counts_id"]),
                f"Number of times warnings have been set for the {node_name}.",
                warnings_counts_signals,
                WARNINGS_ALERTS_CYCLE_TIME,
            ),
            (
                faults_counts_name,
                (alerts_json["faults_counts_id"]),
                f"Number of times faults have been set for the {node_name}.",
                faults_counts_signals,
                FAULTS_ALERTS_CYCLE_TIME,
            ),
            (
                info_name,
                (alerts_json["info_id"]),
                f"Status of info for the {info}.",
                info_signals,
                INFO_ALERTS_CYCLE_TIME,  # TODO: what will be the cycle time for info?
            ),
            (
                info_counts_name,
                (alerts_json["info_counts_id"]),
                f"Number of times info have been set for the {node_name}.",
                info_counts_signals,
                INFO_ALERTS_CYCLE_TIME,
            ),
        ]
    )

    # note the assert is very important because we are breaking the type system's ability to enforce this
    assert len(alerts_msgs) == 6, "There should be 6 alert messages"

    return alerts_msgs, (faults_meta_data, warnings_meta_data, info_meta_data)


def parse_alert_data(
    can_data_dir: str, node_name: str
) -> Optional_t[tuple[List[CanMessage], list[CanAlert]]]:
    try:
        node_alerts_json_data = _validate_alerts_json(
            load_json_file(f"{can_data_dir}/{node_name}/{node_name}_alerts")
        )
    except SchemaError:
        raise InvalidCanJson(f"Alerts JSON file is not valid for node {node_name}")
    # TODO catch file not found error? I imagine for it to be truly optional that must - how do you want to handle that? return a None?
    except FileNotFoundError:
        return None

    if len(node_alerts_json_data) <= 0:
        return None

    (
        warnings_msg,
        faults_msg,
        warnings_counts_msg,
        faults_counts_msg,
        info_msg,
        info_counts_msg,
    ), (
        faults_meta_data,
        warnings_meta_data,
        info_meta_data,
    ) = _parse_node_alerts(
        node_name, node_alerts_json_data
    )

    can_alerts: list[CanAlert] = [
        *[
            CanAlert(
                alert.name,
                CanAlertType.WARNING,
                warnings_meta_data[alert.name]["id"],
                warnings_meta_data[alert.name]["description"],
            )
            for alert in warnings_msg.signals
        ],
        *[
            CanAlert(
                alert.name,
                CanAlertType.FAULT,
                faults_meta_data[alert.name]["id"],
                faults_meta_data[alert.name]["description"],
            )
            for alert in faults_msg.signals
        ],
        *[
            CanAlert(
                alert.name,
                CanAlertType.INFO,
                info_meta_data[alert.name]["id"],
                info_meta_data[alert.name]["description"],
            )
            for alert in info_msg.signals
        ],
    ]
    return [
        warnings_msg,
        faults_msg,
        warnings_counts_msg,
        faults_counts_msg,
        info_msg,
        info_counts_msg,
    ], can_alerts
