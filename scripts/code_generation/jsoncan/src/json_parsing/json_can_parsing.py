"""
Module for parsing CAN JSON, and returning a CanDatabase object. 
"""

from __future__ import annotations

import json
import os
from math import ceil
from typing import Any, Tuple

from ..can_database import *
from ..can_database import CanMessage, CanSignal
from ..utils import max_uint_for_bits
from .schema_validation import (AlertsJson, validate_alerts_json,
                                validate_bus_json, validate_enum_json,
                                validate_rx_json, validate_tx_json)

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


class JsonCanParser:
    def __init__(self, can_data_dir: str):
        self._bus_cfg: dict[str, CanBusConfig] = {}  # Set of bus configurations
        self._nodes: dict[str, CanNode] = {}  # List of node names
        self._messages: dict[str, CanMessage] = {}  # Dict of msg names to msg objects
        self._enums: dict[str, CanEnum] = {}  # Dict of enum names to enum objects
        self._shared_enums: dict[str, CanEnum] = {}  # Set of shared enums
        self._alerts: dict[str, dict[CanAlert, AlertsEntry]] = (
            {}
        )  # Dict of node names to node's alerts
        self._reroute: List[CanForward] = []
        self._forwarder_node: CanNode = None
        self._can_rx: dict[str, CanRxMessages] = {}
        self._parse_json_data(can_data_dir=can_data_dir)

    def make_database(self) -> CanDatabase:
        """
        Make and return CanDatabase object form the parsed data.
        """
        return CanDatabase(
            nodes=self._nodes,
            bus_config=self._bus_cfg,
            msgs=self._messages,
            shared_enums=self._shared_enums,
            alerts=self._alerts,
            reroute_msgs=self._reroute,
            forwarder=self._forwarder_node,
            rx_msgs=self._can_rx,
        )

    def _parse_json_data(self, can_data_dir: str):
        """
        Load all CAN JSON data from specified directory.

        """
        # Load shared JSON data

        nodes = self._parse_json_node_data(can_data_dir)
        bus_configs = self._parse_json_bus_data(can_data_dir)
        shared_enums = self._parse_json_shared_enum_data(can_data_dir)

        # Parse node's TX, ALERTS, and ENUM JSON
        alerts_messages = []
        for node, node_obj in nodes.items():

            enums = self._parse_json_node_enum_data(can_data_dir, node)

            # Parse ALERTS
            alerts = self._parse_json_alert_data(can_data_dir, node)
            # Parse TX messages
            tx_msgs = self._parse_json_tx_data(can_data_dir, node_obj)
            if alerts is not None:
                # add the alert messages to the tx messages
                tx_msgs.append(alerts[0].name)
                tx_msgs.append(alerts[1].name)
                tx_msgs.append(alerts[2].name)
                tx_msgs.append(alerts[3].name)
                tx_msgs.append(alerts[4].name)
                tx_msgs.append(alerts[5].name)
                alerts_messages.extend(alerts)  # save for RX parsing

            # update node object
            node_obj.buses = [
                bus_configs[bus].name
                for bus in bus_configs
                if node in bus_configs[bus].nodes
            ]
            node_obj.tx_msgs = tx_msgs
            node_obj.alerts = []

        # Parse node's RX JSON (have to do this last so all messages on this bus are already found, from TX JSON)
        self._parse_json_rx_data(can_data_dir, alerts_messages)

        # Consistency check
        self._consistency_check()

        # find all message transmitting on one bus but received in another bus
        reroute = self._find_reroute(self._messages.values())
        self._reroute = reroute

    def _parse_json_bus_data(self, can_data_dir) -> List[CanBusConfig]:
        """
        Parse bus JSON data from specified directory.
        """
        assert self._nodes is not None  # need nodes to be parsed first
        bus_json_data = validate_bus_json(self._load_json_file(f"{can_data_dir}/bus"))
        # dynamic validation of bus data
        buses = bus_json_data["buses"]
        for bus in buses:
            if bus["default_mode"] not in bus["modes"]:
                raise InvalidCanJson(f"Default CAN mode is not in the list of modes.")

        buses = {
            bus["name"]: CanBusConfig(
                name=bus["name"],
                default_mode=bus["default_mode"],
                modes=bus["modes"],
                bus_speed=bus["bus_speed"],
                nodes=bus["nodes"],  # string for now
                fd=bus.get("FD", False),
            )
            for bus in buses
        }

        self._bus_cfg = buses
        self._forwarder_node = bus_json_data["forwarder"]
        return buses

    def _parse_json_shared_enum_data(self, can_data_dir) -> List[CanEnum]:
        """
        Parse shared enum JSON data from specified directory.
        """
        shared_enum_json_data = validate_enum_json(
            self._load_json_file(f"{can_data_dir}/shared_enum")
        )
        # Parse shared enum JSON

        enums = {}
        for enum_name, enum_entries in shared_enum_json_data.items():
            # Check if this enum name is a duplicate

            can_enum: CanEnum = self._get_parsed_can_enum(
                enum_name=enum_name, enum_entries=enum_entries
            )

            if can_enum in self._shared_enums or can_enum in self._enums:
                raise InvalidCanJson(
                    f"Enum '{enum_name}' is a duplicate, enums must have unique names."
                )

            self._shared_enums[enum_name] = can_enum
            self._enums[enum_name] = can_enum
            enums[enum_name] = can_enum

        return enums

    def _parse_json_node_data(self, can_data_dir) -> Dict[str, CanNode]:
        """
        Parse node JSON data from specified directory.
        """
        node_names = [f.name for f in os.scandir(can_data_dir) if f.is_dir()]
        # leave other node empty for now
        node_objs = {
            node: CanNode(
                name=node,
                buses=[],
                tx_msgs=[],
                rx_msgs=[],
                alerts=[],
            )
            for node in node_names
        }
        self._nodes = node_objs
        return node_objs

    def _parse_json_node_enum_data(self, can_data_dir, node) -> Dict[str, CanMessage]:
        node_enum_json_data = validate_enum_json(
            self._load_json_file(f"{can_data_dir}/{node}/{node}_enum")
        )
        enums = {}
        for enum_name, enum_entries in node_enum_json_data.items():

            e = self._get_parsed_can_enum(
                enum_name=enum_name, enum_entries=enum_entries
            )

            if e in self._shared_enums or e in self._enums:
                raise InvalidCanJson(
                    f"Enum '{enum_name}' is a duplicate, enums must have unique names."
                )

            self._enums[enum_name] = e
            enums[enum_name] = e
        return enums

    def _parse_json_tx_data(self, can_data_dir, node) -> List[str]:

        node_name = node.name
        node_tx_json_data = validate_tx_json(
            self._load_json_file(f"{can_data_dir}/{node_name}/{node_name}_tx")
        )

        m = []
        for tx_node_msg_name, msg_data in node_tx_json_data.items():
            # Skip if message is disabled
            msg_disabled, _ = self._get_optional_value(
                data=msg_data, key="disabled", default=False
            )
            if msg_disabled:
                continue

            tx_node_msg_name = f"{node_name}_{tx_node_msg_name}"

            # Check if this message name is a duplicate
            if tx_node_msg_name in self._messages:
                raise InvalidCanJson(
                    f"Message '{tx_node_msg_name}' transmitted by node '{node_name}' is a duplicate, messages must have unique names."
                )
            self._messages[tx_node_msg_name] = self._get_parsed_can_message(
                msg_name=tx_node_msg_name, msg_json_data=msg_data, node=node
            )
            m.append(tx_node_msg_name)

        return m

    def _parse_json_alert_data(self, can_data_dir, node) -> List[CanMessage]:
        node_name = node
        node_alerts_json_data = validate_alerts_json(
            self._load_json_file(f"{can_data_dir}/{node_name}/{node_name}_alerts")
        )
        if len(node_alerts_json_data) > 0:
            (
                warnings,
                faults,
                warnings_counts,
                faults_counts,
                info,
                info_counts,
            ), (
                faults_meta_data,
                warnings_meta_data,
                info_meta_data,
            ) = self._parse_node_alerts(node, node_alerts_json_data)

            # Make sure alerts are received by all other boards
            # other_nodes = [other for other in self._nodes if other != node_name]
            # warnings.rx_nodes.extend(other_nodes)
            # faults.rx_nodes.extend(other_nodes)

            self._messages[warnings.name] = warnings
            self._messages[faults.name] = faults
            self._messages[warnings_counts.name] = warnings_counts
            self._messages[faults_counts.name] = faults_counts
            self._messages[info.name] = info
            self._messages[info_counts.name] = info_counts

            self._alerts[node_name] = {
                **{
                    CanAlert(alert.name, CanAlertType.WARNING): warnings_meta_data[
                        alert.name
                    ]
                    for alert in warnings.signals
                },
                **{
                    CanAlert(alert.name, CanAlertType.FAULT): faults_meta_data[
                        alert.name
                    ]
                    for alert in faults.signals
                },
                **{
                    CanAlert(alert.name, CanAlertType.INFO): info_meta_data[alert.name]
                    for alert in info.signals
                },
            }

            return [warnings, faults, warnings_counts, faults_counts, info, info_counts]

    def _parse_json_rx_data(self, can_data_dir, alert_messages) -> List[str]:

        for alert in alert_messages:
            tx_node = alert.tx_node
            tx_buses = alert.bus
            # add rx nodes to alert messages
            # rx nodes are all node on the same bus
            for tx_bus in tx_buses:
                tx_bus_obj = self._bus_cfg[tx_bus]  # get the bus object
                for rx_node in tx_bus_obj.nodes:  # iterate all nodes on the bus
                    if rx_node == tx_node:
                        continue
                    rx_node_obj = self._nodes[rx_node]
                    if alert.name not in rx_node_obj.rx_msgs:
                        rx_node_obj.rx_msgs.append(alert.name)
                    if rx_node not in alert.rx_nodes:  # add the tx node to the rx nodes
                        alert.rx_nodes.append(rx_node)

        for node, node_obj in self._nodes.items():
            node_rx_json_data = validate_rx_json(
                self._load_json_file(f"{can_data_dir}/{node}/{node}_rx")
            )

            rx_msgs_obj_map = {}  # bus name -> list of messages
            for rx_msg_data in node_rx_json_data:
                bus = rx_msg_data["bus"]

                messages = rx_msg_data["messages"]
                for message in messages:
                    # Check if this message is defined
                    if message not in self._messages:
                        raise InvalidCanJson(
                            f"Message '{message}' received by '{node}' is not defined. Make sure it is correctly defined in the TX JSON."
                        )
                    rx_msg = self._messages[message]
                    if node not in rx_msg.rx_nodes:
                        rx_msg.rx_nodes.append(node)

                    # add the message to the node's rx messages
                    rx_node = self._nodes[node]
                    if rx_msg.name not in rx_node.rx_msgs:
                        rx_node.rx_msgs.append(rx_msg.name)
                rx_msgs_obj_map[bus] = messages

            # make a CanRxMessages object
            rx_msgs_obj = CanRxMessages(node=node, messages=rx_msgs_obj_map)
            self._can_rx[node] = rx_msgs_obj

    def _find_node_bus(self, tx_msg: Set[CanMessage]) -> Set[CanBusConfig]:
        """
        Find the all the bus that a node is transmitting on.
        """
        bus = set()
        for msg in tx_msg:
            bus.update(msg.bus)
        return bus

    def _consistency_check(self):
        # no same message name, signal name, enum name
        # message can't transmit and receive by the same node
        # no overlapping bus
        # object cross reference consistency

        overlap_set = set()
        for node_name, node_obj in self._nodes.items():
            node_buses = node_obj.buses
            # check if the bus exist in the bus config
            for bus in node_buses:
                if bus not in self._bus_cfg:
                    raise InvalidCanJson(f"Bus '{bus}' is not defined in the bus JSON.")

            for rx_msg in node_obj.rx_msgs:
                if rx_msg not in self._messages:
                    raise InvalidCanJson(
                        f"Message '{rx_msg}' received by '{node_name}' is not defined. Make sure it is correctly defined in the TX JSON."
                    )

            for tx_msg in node_obj.tx_msgs:
                if tx_msg not in self._messages:
                    raise InvalidCanJson(
                        f"Message '{tx_msg}' transmitted by '{node_name}' is not defined. Make sure it is correctly defined in the TX JSON."
                    )

        # bus consistency check
        for bus_name, bus_obj in self._bus_cfg.items():
            for node in bus_obj.nodes:
                if node not in self._nodes:
                    raise InvalidCanJson(
                        f"Node '{node}' is not defined in the node JSON."
                    )
            # need record what message is transmitting on this bus

        # message consistency check
        for msg_name, msg_obj in self._messages.items():
            for bus in msg_obj.bus:
                if bus not in self._bus_cfg:
                    raise InvalidCanJson(f"Bus '{bus}' is not defined in the bus JSON.")

            for node in msg_obj.rx_nodes:
                if node not in self._nodes:
                    raise InvalidCanJson(
                        f"Node '{node}' is not defined in the node JSON."
                    )

            if msg_obj.tx_node not in self._nodes:
                raise InvalidCanJson(f"Node '{node}' is not defined in the node JSON.")

    def _find_reroute(self, messages: List[CanMessage]) -> List[CanForward]:
        # design choice
        # all message is on FD bus
        # some message from FD bus need to be rerouted to non-FD bus

        # assume only one forwarder node
        assert self._forwarder_node is not None
        # assume only one FD bus
        # multiple non-FD bus
        forward_list = []
        fd_bus_obj = None
        for bus in self._bus_cfg.values():
            if bus.fd:
                fd_bus_obj = bus
                break
        assert fd_bus_obj is not None

        #
        # if a message is transmitted on a non-FD bus then forward it to the FD bus
        # for every message
        # for all the bus that the message is transmitted on
        # if exists a FD bus then skip
        # else add the message to the reroute list and reroute it to the FD bus
        # pick a non-fd bus and reroute the message to FD bus
        for message in messages:
            tx_buses = message.bus
            # skip if the message is already on a FD bus
            if any(bus in self._bus_cfg for bus in tx_buses):
                continue

            from_bus = message.bus[0]
            to_bus = fd_bus_obj.name
            forward = CanForward(
                message=message.name,
                from_bus=from_bus,
                to_bus=to_bus,
            )

            forward_list.append(forward)

        # for every message on system
        # for every rx node of the message
        #

        for message in messages:
            tx_buses = message.bus
            rx_nodes = message.rx_nodes
            for rx_node in rx_nodes:  # for all message received by the node
                rx_node_obj = self._nodes[rx_node]

                # each message can only be received one bus by the node
                rx_bus = self._can_rx[rx_node].find_bus(message.name)

                if rx_bus is None:
                    raise InvalidCanJson(
                        f"Message '{message.name}' is received by node '{rx_node}', but is not defined in the RX JSON."
                    )

                # if message is recieved on fd bus then skip as it is handled by the frist for loop
                if rx_bus == fd_bus_obj.name:
                    continue
                # recieved on the same bus as transmitted
                if rx_bus in tx_buses:
                    continue
                
                from_bus = tx_buses[0]

                # reroute the message to the FD bus
                forward = CanForward(
                    message=message.name,
                    to_bus=rx_bus,
                    from_bus=from_bus,
                )
                forward_list.append(forward)

        return forward_list

    def _get_parsed_can_message(
        self, msg_name: str, msg_json_data: Dict, node: CanNode
    ) -> CanMessage:
        """
        Parse JSON data dictionary representing a CAN message.
        """
        node_name = node.name
        msg_id = msg_json_data["msg_id"]
        description, _ = self._get_optional_value(msg_json_data, "description", "")
        msg_cycle_time = msg_json_data["cycle_time"]
        bus_names = msg_json_data["bus"]

        # will use mode from bus if none
        msg_modes, _ = self._get_optional_value(msg_json_data, "allowed_modes", [])

        log_cycle_time = msg_cycle_time
        telem_cycle_time = msg_cycle_time
        if "data_capture" in msg_json_data:
            log_cycle_time, _ = self._get_optional_value(
                msg_json_data["data_capture"], "log_cycle_time", msg_cycle_time
            )
            telem_cycle_time, _ = self._get_optional_value(
                msg_json_data["data_capture"], "telem_cycle_time", msg_cycle_time
            )

        # Check if message ID is unique

        signals = []
        next_available_bit = 0
        occupied_bits = [None] * 64
        require_start_bit_specified = False

        # Parse message signals
        for signal_name, signal_data in msg_json_data["signals"].items():
            signal_node_name = f"{node_name}_{signal_name}"
            signal, specified_start_bit = self._get_parsed_can_signal(
                signal_name=signal_node_name,
                signal_json_data=signal_data,
                next_available_bit=next_available_bit,
                msg_name=msg_name,
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
            rx_nodes=[],  # rx nodes will be updated later
            # modes=msg_modes,
            log_cycle_time=log_cycle_time,
            telem_cycle_time=telem_cycle_time,
        )

    def _get_parsed_can_signal(
        self,
        signal_name: str,
        signal_json_data: Dict,
        next_available_bit: int,
        msg_name: str,
    ) -> tuple[CanSignal, Any]:
        """
        Parse JSON data dictionary representing a CAN signal.
        """

        max_val = 0
        min_val = 0
        scale = 0
        offset = 0
        bits = 0
        enum = None

        # Parse unit and starting bit position
        unit, _ = self._get_optional_value(signal_json_data, "unit", "")
        start_bit, specified_start_bit = self._get_optional_value(
            signal_json_data, "start_bit", next_available_bit
        )
        signed, _ = self._get_optional_value(signal_json_data, "signed", False)

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
        elif "bits" in signal_json_data:
            bits = signal_json_data["bits"]
            if signed:
                max_val = max_uint_for_bits(bits - 1) - 1
                min_val = -max_uint_for_bits(bits - 1)
            else:
                max_val = max_uint_for_bits(bits)
                min_val = 0
            scale = self._get_optional_value(signal_json_data, "scale", 1)[0]
            offset = 0

        # Otherwise, payload data was not inputted correctly
        else:
            raise InvalidCanJson(
                f"Signal '{signal_name}' has invalid payload representation, and could not be parsed."
            )

        # Parse start value
        start_val, _ = self._get_optional_value(
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

    @staticmethod
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

    def _parse_node_alerts(self, node: str, alerts_json: AlertsJson):
        node_name = node
        """
        Parse JSON data dictionary representing a node's alerts.
        """
        warnings = {
            name: alert
            for name, alert in alerts_json["warnings"].items()
            if not self._get_optional_value(data=alert, key="disabled", default=False)[
                0
            ]
        }
        faults = {
            name: alert
            for name, alert in alerts_json["faults"].items()
            if not self._get_optional_value(data=alert, key="disabled", default=False)[
                0
            ]
        }

        info = {
            name: alert
            for name, alert in alerts_json["info"].items()
            if not self._get_optional_value(data=alert, key="disabled", default=False)[
                0
            ]
        }

        # Number of alerts can't exceed 21. This is because we transmit a "counts" message for faults and warnings
        # that indicate the number of times an alert has been set. Each signal is allocated 3 bits, and so can count
        # up to 8, meaning we can pack 21 alerts to fit inside a 64-bit CAN payload.
        if max(len(warnings), len(faults)) > 21:
            raise InvalidCanJson(
                f"Number of alerts for node '{node_name}' cannot exceed 21."
            )

        # Check alert messages ID are unique
        warnings_id = alerts_json["warnings_id"]
        faults_id = alerts_json["faults_id"]
        warnings_counts_id = alerts_json["warnings_counts_id"]
        faults_counts_id = alerts_json["faults_counts_id"]
        info_id = alerts_json["info_id"]
        info_counts_id = alerts_json["info_counts_id"]

        if any(
            msg_id in {msg.id for msg in self._messages.values()}
            for msg_id in (warnings_id, faults_id, warnings_counts_id, faults_counts_id)
        ):
            conflicting_node = [
                msg
                for msg in self._messages.values()
                for i in (warnings_id, faults_id, warnings_counts_id, faults_counts_id)
                if msg.id == i
            ][0]
            raise InvalidCanJson(
                f"ID for alerts message transmitted by '{node_name}' is a duplicate with '{conflicting_node.name}'. Messages "
                f"must have unique IDs."
            )

        # Check if message name is unique
        warnings_name = f"{node_name}_Warnings"
        faults_name = f"{node_name}_Faults"
        warnings_counts_name = f"{node_name}_WarningsCounts"
        faults_counts_name = f"{node_name}_FaultsCounts"
        info_name = f"{node_name}_Info"
        info_counts_name = f"{node_name}_InfoCounts"

        if any(
            msg_name in self._messages
            for msg_name in [
                warnings_name,
                faults_name,
                warnings_counts_name,
                faults_counts_name,
            ]
        ):
            raise InvalidCanJson(
                f"Name for alerts message transmitted by '{node_name}' is a duplicate, messages must have unique names."
            )

        # Make alert signals
        warnings_meta_data, warnings_signals = self._node_alert_signals(
            node_name, warnings, CanAlertType.WARNING
        )
        faults_meta_data, faults_signals = self._node_alert_signals(
            node_name, faults, CanAlertType.FAULT
        )
        info_meta_data, info_signals = self._node_alert_signals(
            node_name, info, CanAlertType.INFO
        )
        warnings_counts_signals = self._node_alert_count_signals(
            node_name, warnings, CanAlertType.WARNING
        )
        faults_counts_signals = self._node_alert_count_signals(
            node_name, faults, "Fault"
        )
        info_counts_signals = self._node_alert_count_signals(
            node_name, info, CanAlertType.INFO
        )

        # noinspection PyTypeChecker
        alerts_msgs: tuple[CanMessage, CanMessage, CanMessage, CanMessage] = (
            CanMessage(
                name=name,
                id=msg_id,
                description=description,
                cycle_time=cycle_time,
                log_cycle_time=cycle_time,
                telem_cycle_time=cycle_time,
                signals=signals,
                rx_nodes=[],  # will be updated later
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
        return alerts_msgs, (faults_meta_data, warnings_meta_data, info_meta_data)

    @staticmethod
    def _node_alert_signals(
        node: str, alerts: dict[str, AlertsEntry], alert_type: CanAlertType
    ):
        """
        From a list of strings of alert names, return a list of CAN signals that will make up the frame for an alerts msg.
        """
        signals = []
        meta_data = {}
        bit_pos = 0

        for alerts_name, alerts_id in alerts.items():
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
            meta_data[f"{node}_{alert_type}_{alerts_name}"] = alerts_id
        return meta_data, signals

    @staticmethod
    def _node_alert_count_signals(
        node: str, alerts: Dict, alert_type: str
    ) -> List[CanSignal]:
        """
        From a list of strings of alert names, return a list of CAN signals.
        Each signal will represent the number of times the corresponding alert has been set.
        """
        COUNT_BITS = 3
        return [
            CanSignal(
                name=f"{node}_{alert_type}_{alert}Count",
                start_bit=i * COUNT_BITS,
                bits=COUNT_BITS,
                scale=1,
                offset=0,
                min_val=0,
                max_val=2**COUNT_BITS - 1,
                start_val=0,
                enum=None,
                unit="",
                signed=False,
            )
            for i, alert in enumerate(alerts)
        ]

    @staticmethod
    def _build_node_obj(
        node: str,
    ) -> CanNode:
        """
        Build a CanNode object from the parsed JSON data.
        """
        return CanNode(name=node)

    @staticmethod
    def _load_json_file(file_path: str) -> Dict:
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

    @staticmethod
    def _get_optional_value(data: Dict, key: str, default: any) -> (str or any, bool):
        """
        Parse a value from a key in data. If key not found, return default.
        """
        if key in data:
            return data[key], True
        else:
            return default, False
