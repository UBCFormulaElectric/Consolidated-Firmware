"""
Module for parsing CAN JSON, and returning a CanDatabase object.
"""

from __future__ import annotations

import json
import os
from math import ceil
from typing import Any, Tuple, List, Dict, Optional

from schema import SchemaError

from .schema_validation import (
    AlertsJson,
    validate_alerts_json,
    validate_bus_json,
    validate_enum_json,
    validate_rx_json,
    validate_tx_json,
)
from ..can_database import (CanMessage, CanSignal, CanBusConfig, CanNode, CanEnum, CanAlert, AlertsEntry, CanForward,
                            CanRxMessages, CanDatabase, CanAlertType)
from ..utils import max_uint_for_bits

WARNINGS_ALERTS_CYCLE_TIME = 1000  # 1Hz
FAULTS_ALERTS_CYCLE_TIME = 100  # 10Hz


class InvalidCanJson(Exception):
    """
    General exception if anything is wrong with the JSON.
    """

    ...


def calc_signal_scale_and_offset(
        max_val: int, min_val: int, num_bits: int
) -> Tuple[float, float]:
    """
    Calculate scale and offset for DBC file.
    """
    scale = (max_val - min_val) / max_uint_for_bits(num_bits)
    offset = min_val
    return scale, offset


def load_json_file(file_path: str) -> Dict:
    """
    Load an individual JSON file from specified path.
    """
    with open(f"{file_path}.json") as file:
        try:
            data = json.load(file)
            return data
        except json.JSONDecodeError:
            raise InvalidCanJson(
                f"Error parsing JSON data from file path '{file_path}'."
            )


def get_optional_value(data: Dict, key: str, default: any) -> tuple[str or any, bool]:
    """
    Parse a value from a key in data. If key not found, return default.
    """
    if key in data:
        return data[key], True
    else:
        return default, False


def _get_parsed_can_enum(enum_name: str, enum_entries: dict[str, int]) -> CanEnum:
    """
    Parse JSON data dictionary representing a CAN enum.
    """
    items = {}
    for name, value in enum_entries.items():
        if value < 0:
            raise InvalidCanJson(
                f"Negative enum value found for enum '{enum_name}', which is not supported. Use only positive integers or zero."
            )

        if value in items:
            raise InvalidCanJson(
                f"Repeated value {value} for enum '{enum_name}', which is not allowed (values must be unique)."
            )

        items[value] = name

    if 0 not in items:
        raise InvalidCanJson(f"Enum '{enum_name}' must start at 0.")

    return CanEnum(name=enum_name, items=items)


def _get_parsed_can_signal(
        signal_name: str,
        signal_json_data: Dict,
        next_available_bit: int,
        enums: dict[str, CanEnum]
) -> tuple[CanSignal, Any]:
    """
    Parse JSON data dictionary representing a CAN signal.
    """
    # max_val = 0
    # min_val = 0
    # scale = 0
    # offset = 0
    # bits = 0
    enum = None

    # Parse unit and starting bit position
    unit, _ = get_optional_value(signal_json_data, "unit", "")
    start_bit, specified_start_bit = get_optional_value(
        signal_json_data, "start_bit", next_available_bit
    )
    signed, _ = get_optional_value(signal_json_data, "signed", False)

    # Get signal value data. Method depends on which data provided in JSON file.
    # Option 1: Provide DBC data
    if all(
            datum in signal_json_data
            for datum in ("min", "max", "scale", "offset", "bits")
    ):
        bits = signal_json_data["bits"]
        max_val = signal_json_data["max"]
        min_val = signal_json_data["min"]
        scale = signal_json_data["scale"]
        offset = signal_json_data["offset"]

    # Option 2: Provide min, max, and bit length. Scale and offset are calculated.
    elif all(datum in signal_json_data for datum in ("min", "max", "bits")):
        max_val = signal_json_data["max"]
        min_val = signal_json_data["min"]
        bits = signal_json_data["bits"]

        scale, offset = calc_signal_scale_and_offset(
            max_val=max_val, min_val=min_val, num_bits=bits
        )

    # Option 3: Provide min, max, and resolution (scale). Offset and bit length are calculated.
    elif all(datum in signal_json_data for datum in ("min", "max", "resolution")):
        max_val = signal_json_data["max"]
        min_val = signal_json_data["min"]
        sig_resolution = signal_json_data["resolution"]
        scale = sig_resolution
        offset = min_val

        max_raw_val = ceil((max_val - min_val) / sig_resolution)
        bits = max_raw_val.bit_length()

    # Option 4: Provide an enum. Min, max, bits, have to be calculated.
    # Scale and offset are assumed to be 1 and 0, respectively (so start your enums at 0!)
    elif "enum" in signal_json_data:
        enum_name = signal_json_data["enum"]
        if enum_name not in enums.keys():
            raise InvalidCanJson(
                f"Signal '{signal_name}' requests an enum named '{enum_name}', but an enum by that name was not defined."
            )

        enum = enums[enum_name]
        max_val = enum.max_val()
        min_val = enum.min_val()
        bits = enum.num_bits()
        scale = 1
        offset = 0

    # Option 5: Just provide bits, and will be considered to be an unsigned int of however many bits.
    elif "bits" in signal_json_data:
        bits = signal_json_data["bits"]
        if signed:
            max_val = max_uint_for_bits(bits - 1) - 1
            min_val = -max_uint_for_bits(bits - 1)
        else:
            max_val = max_uint_for_bits(bits)
            min_val = 0
        scale = get_optional_value(signal_json_data, "scale", 1)[0]
        offset = 0

    # Otherwise, payload data was not inputted correctly
    else:
        raise InvalidCanJson(
            f"Signal '{signal_name}' has invalid payload representation, and could not be parsed."
        )

    # Parse start value
    start_val, _ = get_optional_value(
        signal_json_data, "start_value", min_val
    )

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


def _get_parsed_can_message(
        msg_name: str,
        msg_json_data: Dict,
        node_name: str,
        enums: dict[str, CanEnum]
) -> CanMessage:
    """
    Parse JSON data dictionary representing a CAN message.
    """
    msg_id = msg_json_data["msg_id"]
    description, _ = get_optional_value(msg_json_data, "description", "")
    msg_cycle_time = msg_json_data["cycle_time"]
    bus_names = msg_json_data["bus"]

    # will use mode from bus if none
    msg_modes, _ = get_optional_value(msg_json_data, "allowed_modes", [])

    log_cycle_time = msg_cycle_time
    telem_cycle_time = msg_cycle_time
    if "data_capture" in msg_json_data:
        log_cycle_time, _ = get_optional_value(
            msg_json_data["data_capture"], "log_cycle_time", msg_cycle_time
        )
        telem_cycle_time, _ = get_optional_value(
            msg_json_data["data_capture"], "telem_cycle_time", msg_cycle_time
        )

    # Check if message ID is unique

    signals = []
    next_available_bit = 0
    occupied_bits: list[Optional[str]] = [None] * 64
    require_start_bit_specified = False

    # Parse message signals
    for signal_name, signal_data in msg_json_data["signals"].items():
        signal_node_name = f"{node_name}_{signal_name}"
        signal, specified_start_bit = _get_parsed_can_signal(
            signal_name=signal_node_name,
            signal_json_data=signal_data,
            next_available_bit=next_available_bit,
            enums=enums
        )

        # If we specify one start bit, we require that the rest of the message specify start bit too
        if specified_start_bit:
            require_start_bit_specified = True
        elif require_start_bit_specified:
            raise InvalidCanJson(
                f"Signal '{signal.name}' in '{msg_name}' must specify a start bit positions, because other signals in this message have specified start bits."
            )

        # Mark a signal's bits as occupied, by inserting the signal's name
        for idx in range(signal.start_bit, signal.start_bit + signal.bits):
            if idx < 0 or idx > 63:
                raise InvalidCanJson(
                    f"Signal '{signal.name}' in '{msg_name}' is requesting to put a bit at invalid position {idx}. Messages have a maximum length of 64 bits."
                )
            elif occupied_bits[idx] is not None:
                raise InvalidCanJson(
                    f"Signal '{signal.name}' in '{msg_name}' is requesting to put a bit at invalid position {idx}. That position is already occupied by the signal '{occupied_bits[idx]}'."
                )

            occupied_bits[idx] = signal.name

        signals.append(signal)
        next_available_bit += signal.bits

    return CanMessage(
        name=msg_name,
        id=msg_id,
        description=description,
        signals=signals,
        bus=bus_names,
        cycle_time=msg_cycle_time,
        tx_node=node_name,
        rx_node_names=[],  # rx nodes will be updated later
        # modes=msg_modes,
        log_cycle_time=log_cycle_time,
        telem_cycle_time=telem_cycle_time,
    )


def _parse_node_alert_count_signals(node: str, alerts: dict[str, AlertsEntry], alert_type: str) -> List[CanSignal]:
    """
    From a list of strings of alert names, return a list of CAN signals.
    Each signal will represent the number of times the corresponding alert has been set.
    :returns a list of signals representing the signals carrying alert counts
    """
    ALERT_COUNT_BITS = 3  # TODO move this up, this is a very important configuration constant
    return [
        CanSignal(
            name=f"{node}_{alert_type}_{alert}Count",
            start_bit=i * ALERT_COUNT_BITS,
            bits=ALERT_COUNT_BITS,
            scale=1,
            offset=0,
            min_val=0,
            max_val=2 ** ALERT_COUNT_BITS - 1,
            start_val=0,
            enum=None,
            unit="",
            signed=False,
        )
        for i, alert in enumerate(alerts)
    ]


def _parse_node_alert_signals(
        node: str, alerts: dict[str, AlertsEntry], alert_type: CanAlertType
) -> tuple[dict[str, AlertsEntry], list[CanSignal]]:
    """
    From a list of strings of alert names, return a list of CAN signals that will make up the frame for an alerts msg.
    :returns metadata which maps names of alert signals to their AlertsEntry, list of CanSignals for the alert message
    """
    signals: list[CanSignal] = []
    meta_data: dict[str, AlertsEntry] = {}
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


class JsonCanParser:
    """
    A few notes about this class
    The purpose of this class is to
    Any functions prefixed with "_parse_" are used to directly parse the JSON data from files
        - by directly, i mean it pretty much returns exactly what is in the json data without any interpretation
        - duplication checks are not interpretation
    Other functions consist the logical representation
    """

    # popoulate these for CanDatabase
    # TODO make these dicts other objects with better querying, rather than deciding for the user
    _nodes: dict[str, CanNode]  # List of node names
    _bus_config: dict[str, CanBusConfig]  # Set of bus configurations
    _msgs: dict[str, CanMessage]  # Dict of msg names to msg objects
    _shared_enums: dict[str, CanEnum]  # Set of shared enums
    _alerts: dict[str, dict[CanAlert, AlertsEntry]]  # Dict of node names to node's alerts
    _reroute_msgs: List[CanForward]
    _rx_msgs: dict[str, CanRxMessages]

    def __init__(self, can_data_dir: str):
        """
        Parses JSON data
        :param can_data_dir: Location of all the json files
        """
        node_names = self._list_nodes_from_folders(can_data_dir)
        # create node objects for each node
        self._nodes = {
            node_name: CanNode(
                name=node_name,
                tx_msg_names=[],
                rx_msg_names=[],
                alerts=[],
                bus_names=[]
            ) for node_name in node_names
        }

        # parse the bus config
        self._bus_config = self._parse_bus_data(can_data_dir)

        # populate self._nodes[node_name].bus_names
        # add busses each node is on
        for node_name in node_names:
            self._nodes[node_name].bus_names = [
                self._bus_config[bus].name
                for bus in self._bus_config
                if node_name in self._bus_config[bus].nodes
            ]

        # parse enums
        # updates self._shared_enums
        self._shared_enums = self._parse_shared_enums(can_data_dir)
        enums = self._shared_enums.copy()
        # enum data
        for node_name in node_names:
            # writes node enums into global enum bucket
            node_enums = self._parse_node_enum_data(can_data_dir, node_name)
            enums.update(node_enums)

        # tx messages
        # updates self._msgs
        # update self._nodes[node].tx_msg_names
        self._msgs = {}
        for node_name in node_names:
            node_msgs = self._parse_tx_data(can_data_dir, node_name, enums)
            # TODO globalize this functionality
            for msg in node_msgs:
                # Check if this message name is a duplicate
                if msg.name in self._msgs.keys():
                    # TODO find the other node which also transmits the given message name
                    raise InvalidCanJson(
                        f"Message '{msg.name}' transmitted by node '{node_name}' is a duplicate, messages must have unique names. {self._msgs.keys()}"
                    )
                # TODO check if the message ID is unique
                self._msgs[msg.name] = msg
            self._nodes[node_name].tx_msg_names.extend([msg.name for msg in node_msgs])

        # Parse Alerts
        self._alerts = {}
        alert_msgs = []
        for node_name in node_names:
            # Parse ALERTS
            node_alert_msgs, alerts = self._parse_alert_data(can_data_dir, node_name)
            # since they are optional
            if node_alert_msgs is None or alerts is None:
                assert node_alert_msgs is None and alerts is None
                continue
            assert len(node_alert_msgs) == 6, "Alert messages should be 6 (unless we add more types of alerts)"

            # mutate
            self._alerts[node_name] = alerts
            self._msgs.update({msg.name: msg for msg in node_alert_msgs})
            self._nodes[node_name].tx_msg_names.extend([msg.name for msg in node_alert_msgs])
            alert_msgs.extend(node_alert_msgs)  # save for RX parsing

        # Parse all nodes' RX JSON (have to do this last so all messages on this bus are already found, from TX JSON)
        # IMPORTANT: make sure to handle RX only after all the TX msgs are handled
        self._rx_msgs = {}
        for rx_node in self._nodes.values():
            self._rx_msgs[rx_node.name] = self._parse_json_rx_data(can_data_dir, self._msgs, self._bus_config, rx_node)

        # TODO I think we should unify the interface where we add tx messages in general
        self._register_alert_messages(alert_msgs)

        # Consistency check
        # TODO why does it have to be here?
        self._consistency_check()

        # find all message transmitting on one bus but received in another bus
        # IMPORTANT: reroutes can only be calculated after all the RXs are figured out
        self._reroute_msgs = []
        self._calculate_reroutes(can_data_dir)

    def make_database(self) -> CanDatabase:
        """
        Make and return CanDatabase object form the parsed data.
        """
        return CanDatabase(
            nodes=self._nodes,
            bus_config=self._bus_config,
            msgs=self._msgs,
            shared_enums=self._shared_enums,
            alerts=self._alerts,
            reroute_msgs=self._reroute_msgs,
            rx_msgs=self._rx_msgs,
        )

    @staticmethod
    def _parse_bus_data(can_data_dir: str) -> dict[str, CanBusConfig]:
        """
        Parses data about buses from global configuration
        CONSISTENCY: bus.default_mode not in bus.modes
        """
        try:
            bus_json_data = validate_bus_json(load_json_file(f"{can_data_dir}/bus"))
        except SchemaError:
            raise InvalidCanJson("Bus JSON file is not valid")

        # dynamic validation of bus data
        buses = bus_json_data["buses"]

        for bus in buses:
            if bus["default_mode"] not in bus["modes"]:
                raise InvalidCanJson(f"Error on bus {bus['name']}: Default CAN mode is not in the list of modes.")

        return {
            bus["name"]: CanBusConfig(
                name=bus["name"],
                default_mode=bus["default_mode"],
                modes=bus["modes"],
                bus_speed=bus["bus_speed"],
                nodes=bus["nodes"],
                fd=bus.get("FD", False),
            )
            for bus in buses
        }

    @staticmethod
    def _parse_shared_enums(can_data_dir) -> dict[str, CanEnum]:
        """
        Parse shared enum JSON data from specified directory.
        """
        try:
            shared_enum_json_data = validate_enum_json(load_json_file(f"{can_data_dir}/shared_enum"))
        except SchemaError:
            raise InvalidCanJson("Shared enum JSON file is not valid")
        enums = {}
        # Parse shared enum JSON
        for enum_name, enum_entries in shared_enum_json_data.items():
            # Check if this enum name is a duplicate
            can_enum: CanEnum = _get_parsed_can_enum(
                enum_name=enum_name, enum_entries=enum_entries
            )
            if can_enum in enums:
                raise InvalidCanJson(
                    f"Enum '{enum_name}' is a duplicate, enums must have unique names."
                )
            enums[enum_name] = can_enum
        return enums

    @staticmethod
    def _list_nodes_from_folders(can_data_dir: str) -> list[str]:
        """
        From a specified directory, creates empty CanNode objects for each represented nodes (from folders)
        """
        return [f.name for f in os.scandir(can_data_dir) if f.is_dir()]

    @staticmethod
    def _parse_node_enum_data(can_data_dir: str, node_name: str) -> dict[str, CanEnum]:
        """
        Adds node_name's enums to the given _enum dictionary
        """
        try:
            node_enum_json_data = validate_enum_json(load_json_file(f"{can_data_dir}/{node_name}/{node_name}_enum"))
        except SchemaError:
            raise InvalidCanJson(f"Enum JSON file is not valid for node {node_name}")

        new_enums = {}
        for enum_name, enum_entries in node_enum_json_data.items():
            enum = _get_parsed_can_enum(enum_name=enum_name, enum_entries=enum_entries)
            if enum in new_enums:
                raise InvalidCanJson(
                    f"Enum '{enum_name}' is a duplicate, enums must have unique names."
                )
            new_enums[enum_name] = enum
        return new_enums

    @staticmethod
    def _parse_tx_data(can_data_dir: str, node_name: str, enums: dict[str, CanEnum]) -> list[CanMessage]:
        """
        Parses TX messages from file, adds them to message list
        :param can_data_dir: :|
        :param node_name: name of the node
        :return: list of names of messages associated with the given node
        """
        try:
            node_tx_json_data = validate_tx_json(load_json_file(f"{can_data_dir}/{node_name}/{node_name}_tx"))
        except:
            raise InvalidCanJson(f"TX json file is not valid for {node_name}")

        msgs: list[CanMessage] = []
        for tx_msg_name_json, tx_msg_json in node_tx_json_data.items():
            # Skip if message is disabled
            msg_disabled, _ = get_optional_value(
                data=tx_msg_json, key="disabled", default=False
            )
            if bool(msg_disabled):
                continue

            tx_msg_name = f"{node_name}_{tx_msg_name_json}"
            msgs.append(_get_parsed_can_message(
                msg_name=tx_msg_name, msg_json_data=tx_msg_json, node_name=node_name, enums=enums
            ))
        return msgs

    @staticmethod
    def _parse_alert_data(can_data_dir, node_name) -> Optional[
        tuple[List[CanMessage], dict[CanAlert, AlertsEntry]]]:
        try:
            node_alerts_json_data = validate_alerts_json(
                load_json_file(f"{can_data_dir}/{node_name}/{node_name}_alerts")
            )
        except SchemaError:
            raise InvalidCanJson(f"Alerts JSON file is not valid for node {node_name}")
        # TODO catch file not found error? I imagine for it to be truly optional that must happen

        if len(node_alerts_json_data) <= 0:
            return None

        (
            warnings_msg,
            faults_msg,
            warnings_counts_msg,
            faults_counts_msg,
            info_msg,
            info_counts_msg
        ), (
            faults_meta_data,
            warnings_meta_data,
            info_meta_data,
        ) = JsonCanParser._parse_node_alerts(node_name, node_alerts_json_data)

        # TODO is this comment still relevant?
        # Make sure alerts are received by all other boards
        # other_nodes = [other for other in self._nodes if other != node_name]
        # warnings.rx_nodes.extend(other_nodes)
        # faults.rx_nodes.extend(other_nodes)

        can_alerts: dict[CanAlert, AlertsEntry] = {
            **{
                CanAlert(alert.name, CanAlertType.WARNING): warnings_meta_data[alert.name]
                for alert in warnings_msg.signals
            },
            **{
                CanAlert(alert.name, CanAlertType.FAULT): faults_meta_data[alert.name]
                for alert in faults_msg.signals
            },
            **{
                CanAlert(alert.name, CanAlertType.INFO): info_meta_data[alert.name]
                for alert in info_msg.signals
            },
        }
        return [warnings_msg, faults_msg, warnings_counts_msg, faults_counts_msg, info_msg, info_counts_msg], can_alerts

    @staticmethod
    def _parse_json_rx_data(can_data_dir: str, _msgs: dict[str, CanMessage], _bus_config: dict[str, CanBusConfig],
                            rx_node: CanNode) -> CanRxMessages:
        """
        :param can_data_dir:
        :param _msgs: READONLY this is required as rx message names map to created (tx) messages
        :param _bus_config: READONlY this is required as rx messages specify which bus they are on
        note: this function validates that the rx_msg reads on a existant bus
        :param rx_node: rx node we are processing messages for
        :return:
        """
        try:
            node_rx_json_data = validate_rx_json(load_json_file(f"{can_data_dir}/{rx_node.name}/{rx_node.name}_rx"))
        except SchemaError:
            raise InvalidCanJson(f"RX JSON file is not valid for node {rx_node.name}")

        rx_msgs_obj_map: dict[str, list[str]] = {}  # bus name -> list of messages names

        # iterate through each bus that you are listening on
        for rx_bus_metadata in node_rx_json_data:
            bus = rx_bus_metadata["bus"]
            # check bus is present
            if bus not in _bus_config:
                raise InvalidCanJson(f"Bus '{bus}' is not defined in the bus JSON.")

            # TODO check that the node is on the bus (or is this a routing functionality)
            # i think regardless it is trivial enough, and this is a very useful check

            messages = rx_bus_metadata["messages"]
            # if "all" in messages then add all messages on this bus
            # TODO maybe we just make the type of messages : list[str] | "all"
            if "all" in messages:
                messages = list(set(_msgs) - set(rx_node.tx_msg_names))

            for message in messages:
                # Check if this message is defined
                if message not in _msgs:
                    raise InvalidCanJson(
                        f"Message '{message}' received by '{rx_node.name}' is not defined. Make sure it is correctly defined in the TX JSON."
                    )

                msg_to_rx = _msgs[message]

                # tell msg_to_rx that the current node rxs it
                if rx_node.name not in msg_to_rx.rx_node_names:
                    msg_to_rx.rx_node_names.append(rx_node.name)

                # add the message to the node's rx messages
                if msg_to_rx.name not in rx_node.rx_msg_names:  # TODO why do we need to check uniqueness? if they need to be unique just enforce with a set, and error if twice?
                    rx_node.rx_msg_names.append(msg_to_rx.name)

            rx_msgs_obj_map[bus] = messages

        # make a CanRxMessages object
        return CanRxMessages(node=rx_node.name, messages=rx_msgs_obj_map)

    def _consistency_check(self) -> None:
        # TODO should this be checked post-hoc, or should it be checked as you parse the messages.
        # In the latter method, you would be able to guarentee that when the intermediary data is ready, that it is valid.
        # however, I don't think this is very much part of the design philosophy at all, as there are many instances
        # where data is instantiated but not valid

        # no same message name, signal name, enum name
        # message can't transmit and receive by the same node
        # no overlapping bus
        # object cross reference consistency

        for node_name, node_obj in self._nodes.items():
            node_buses = node_obj.bus_names
            # check if the bus exist in the bus config
            for bus in node_buses:
                if bus not in self._bus_config:
                    raise InvalidCanJson(f"Bus '{bus}' is not defined in the bus JSON.")

            for rx_msg in node_obj.rx_msg_names:
                if rx_msg not in self._msgs:
                    raise InvalidCanJson(
                        f"Message '{rx_msg}' received by '{node_name}' is not defined. Make sure it is correctly defined in the TX JSON."
                    )

            for tx_msg in node_obj.tx_msg_names:
                if tx_msg not in self._msgs:
                    raise InvalidCanJson(
                        f"Message '{tx_msg}' transmitted by '{node_name}' is not defined. Make sure it is correctly defined in the TX JSON."
                    )

        # bus consistency check
        for bus_name, bus_obj in self._bus_config.items():
            for node in bus_obj.nodes:
                if node not in self._nodes:
                    raise InvalidCanJson(
                        f"Node '{node}' is not defined in the node JSON."
                    )
            # need record what message is transmitting on this bus

        # message consistency check
        for msg_name, msg_obj in self._msgs.items():
            for bus in msg_obj.bus:
                if bus not in self._bus_config:
                    raise InvalidCanJson(f"Bus '{bus}' is not defined in the bus JSON.")

            for node in msg_obj.rx_node_names:
                if node not in self._nodes:
                    raise InvalidCanJson(
                        f"Node '{node}' is not defined in the node JSON."
                    )

            # TODO double check that this is correct, node below is saying that it might be used before assignment
            if msg_obj.tx_node not in self._nodes:
                raise InvalidCanJson(f"Node '{node}' is not defined in the node JSON.")

    def _register_alert_messages(self, alerts_msgs: List[CanMessage]):
        """
        Register the alert message to the node.
        Every single board should recieve every other fault/warning
        much of the work here is to automatically decide which port should receive the alert message
        """
        # TODO??? fix the node, message, rx object

        # alerts is recieved by all nodes
        for alerts_msg in alerts_msgs:
            # for all nodes
            for node in self._nodes.values():
                if alerts_msg.tx_node == node.name:
                    # skip the node that transmit the message
                    continue

                # check if the alert is broadcasted on a bus that is directly connected to the node
                overlap_bus = set(alerts_msg.bus) & set(node.bus_names)
                is_raw_connection = len(overlap_bus) > 0
                if is_raw_connection:
                    overlap_bus = list(overlap_bus)[0]
                    # add the message to the node's rx messages
                    if alerts_msg.name not in node.rx_msg_names:
                        node.rx_msg_names.append(alerts_msg.name)

                    # add rx message obj
                    alerts_msg.rx_node_names.extend(
                        [key for key in self._nodes.keys() if key != node]
                    )
                    self._rx_msgs[node.name].messages.setdefault(overlap_bus, []).append(
                        alerts_msg.name
                    )
                else:
                    # if the message is not on the bus then it is not received by this node
                    # need to reroute
                    # random pick a rx port from the node

                    rx_bus = node.bus_names[0] if node.bus_names else None
                    if rx_bus is None:
                        continue
                    alerts_msg.rx_node_names.extend(
                        [key for key in self._nodes.keys() if key != node]
                    )
                    if alerts_msg.name not in node.rx_msg_names:
                        node.rx_msg_names.append(alerts_msg.name)

                    # add rx message obj
                    self._rx_msgs[node.name].messages.setdefault(rx_bus, []).append(
                        alerts_msg.name
                    )

    def _calculate_reroutes(self, can_data_dir) -> List[CanForward]:
        # design choice
        # all message is on FD bus
        # some message from FD bus need to be rerouted to non-FD bus

        # TODO do we try to do this with the bus config?
        try:
            forwarders_configs = validate_bus_json(
                load_json_file(f"{can_data_dir}/bus")
            )["forwarders"]
        except SchemaError:
            pass
        # a map bus name to set of tx messages
        bus_tx_messages = {bus: set() for bus in self._bus_config}

        # a map bus name to set of rx messages
        bus_rx_messages = {bus: set() for bus in self._bus_config}

        for _, msg in self._msgs.items():
            tx_buses = msg.bus
            for bus in tx_buses:
                if bus not in bus_tx_messages:
                    raise InvalidCanJson(
                        f"Message '{msg.name}' is not defined in the bus JSON."
                    )

                bus_tx_messages[bus].add(msg.name)
            for rx_node, _ in self._nodes.items():
                rx_bus = self._rx_msgs[rx_node].find_bus(msg.name)
                if rx_bus is None:
                    continue
                bus_rx_messages[rx_bus].add(msg.name)

        for bus_name, bus_obj in self._bus_config.items():
            tx_messages = bus_tx_messages[bus_name]
            rx_messages = bus_rx_messages[bus_name]

            # rx message that is not transmitted on the current bus
            forward_messages = rx_messages - tx_messages
            rx_bus = bus_name
            # if there is a rx message that is not in the tx message then the message is from another bus so we need to reroute it

            for forward_msg in forward_messages:
                # find the bus that the message is transmited on
                tx_buses = self._msgs[forward_msg].bus

                # check forwarder config to figure out the which bus to forward to
                for tx_bus in tx_buses:
                    if tx_bus == bus_name:
                        raise InvalidCanJson(
                            "Tx bus and rx bus should not be the same at this stage. Something is wrong"
                        )
                    for config in forwarders_configs:
                        found = False
                        if (rx_bus == config["bus1"] and tx_bus == config["bus2"]) or (
                                rx_bus == config["bus2"] and tx_bus == config["bus1"]
                        ):
                            # found = True

                            # create a forwarder object
                            forwarder = CanForward(
                                from_bus=tx_bus,
                                to_bus=rx_bus,
                                message=forward_msg,
                                forwarder=config["node"],
                            )

                            self._reroute_msgs.append(forwarder)
                            break
                        if not found:
                            raise InvalidCanJson(
                                f"Forwarder config for bus '{rx_bus}' and bus '{tx_bus}' is not defined in the bus JSON for message {forward_msg}."
                            )
        return self._reroute_msgs

    @staticmethod
    def _parse_node_alerts(node: str, alerts_json: AlertsJson):
        node_name = node
        """
        Parse JSON data dictionary representing a node's alerts.
        """
        warnings = {
            name: alert
            for name, alert in alerts_json["warnings"].items()
            if not get_optional_value(data=alert, key="disabled", default=False)[
                0
            ]
        }
        faults = {
            name: alert
            for name, alert in alerts_json["faults"].items()
            if not get_optional_value(data=alert, key="disabled", default=False)[
                0
            ]
        }
        info = {
            name: alert
            for name, alert in alerts_json["info"].items()
            if not get_optional_value(data=alert, key="disabled", default=False)[
                0
            ]
        }

        # Number of alerts can't exceed 21. This is because we transmit a "counts" message for faults and warnings
        # that indicate the number of times an alert has been set. Each signal is allocated 3 bits, and so can count
        # up to 8, meaning we can pack 21 alerts to fit inside a 64-bit CAN payload.
        if max(len(warnings), len(faults), len(info)) > 21:
            raise InvalidCanJson(
                f"Number of alerts for node '{node_name}' cannot exceed 21. Check that warnings, faults and infos have at most 21 items each"
            )

        # Check alert messages ID are unique
        warnings_id = alerts_json["warnings_id"]
        faults_id = alerts_json["faults_id"]
        warnings_counts_id = alerts_json["warnings_counts_id"]
        faults_counts_id = alerts_json["faults_counts_id"]
        info_id = alerts_json["info_id"]
        info_counts_id = alerts_json["info_counts_id"]

        # TODO check conflict ID elsewhere
        # if any(
        #         msg_id in {msg.id for msg in self._messages.values()}
        #         for msg_id in (warnings_id, faults_id, warnings_counts_id, faults_counts_id)
        # ):
        #     conflicting_node = [
        #         msg
        #         for msg in self._messages.values()
        #         for i in (warnings_id, faults_id, warnings_counts_id, faults_counts_id)
        #         if msg.id == i
        #     ][0]
        #     raise InvalidCanJson(
        #         f"ID for alerts message transmitted by '{node_name}' is a duplicate with '{conflicting_node.name}'. Messages "
        #         f"must have unique IDs."
        #     )

        # Check if message name is unique
        warnings_name = f"{node_name}_Warnings"
        faults_name = f"{node_name}_Faults"
        warnings_counts_name = f"{node_name}_WarningsCounts"
        faults_counts_name = f"{node_name}_FaultsCounts"
        info_name = f"{node_name}_Info"
        info_counts_name = f"{node_name}_InfoCounts"

        # TODO check conflict CANID elsewhere
        # if any(
        #         msg_name in self._messages
        #         for msg_name in [
        #             warnings_name,
        #             faults_name,
        #             warnings_counts_name,
        #             faults_counts_name,
        #         ]
        # ):
        #     raise InvalidCanJson(
        #         f"Name for alerts message transmitted by '{node_name}' is a duplicate, messages must have unique names."
        #     )

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
            node_name, faults, "Fault"
        )
        info_counts_signals = _parse_node_alert_count_signals(
            node_name, info, CanAlertType.INFO
        )

        # noinspection PyTypeChecker
        alerts_msgs: tuple[CanMessage, CanMessage, CanMessage, CanMessage, CanMessage, CanMessage] = tuple(
            CanMessage(
                name=name,
                id=msg_id,
                description=description,
                cycle_time=cycle_time,
                log_cycle_time=cycle_time,
                telem_cycle_time=cycle_time,
                signals=signals,
                rx_node_names=[],  # will be updated later
                tx_node=node,
                bus=alerts_json["bus"],
            )
            for name, msg_id, description, signals, cycle_time in [
                (
                    warnings_name,
                    warnings_id,
                    f"Status of warnings for the {node_name}.",
                    warnings_signals,
                    WARNINGS_ALERTS_CYCLE_TIME,
                ),
                (
                    faults_name,
                    faults_id,
                    f"Status of faults for the {faults}.",
                    faults_signals,
                    FAULTS_ALERTS_CYCLE_TIME,
                ),
                (
                    warnings_counts_name,
                    warnings_counts_id,
                    f"Number of times warnings have been set for the {node_name}.",
                    warnings_counts_signals,
                    WARNINGS_ALERTS_CYCLE_TIME,
                ),
                (
                    faults_counts_name,
                    faults_counts_id,
                    f"Number of times faults have been set for the {node_name}.",
                    faults_counts_signals,
                    FAULTS_ALERTS_CYCLE_TIME,
                ),
                (
                    info_name,
                    info_id,
                    f"Status of info for the {info}.",
                    info_signals,
                    FAULTS_ALERTS_CYCLE_TIME,  # TODO: what will be the cycle time for info?
                ),
                (
                    info_counts_name,
                    info_counts_id,
                    f"Number of times info have been set for the {node_name}.",
                    info_counts_signals,
                    FAULTS_ALERTS_CYCLE_TIME,
                ),
            ]
        )

        # note the assert is very important because we are breaking the type system's ability to enforce this
        assert len(alerts_msgs) == 6, "There should be 6 alert messages"

        return alerts_msgs, (faults_meta_data, warnings_meta_data, info_meta_data)
