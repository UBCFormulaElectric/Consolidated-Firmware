"""
Module for parsing CAN JSON, and returning a CanDatabase object.
"""

from __future__ import annotations

# types
from typing import List
from schema import SchemaError
from ..can_database import (CanMessage, CanNode, CanEnum, CanAlert, CanForward,
                            CanRxMessages, CanDatabase)

# new files
from .parse_bus import parse_bus_data, _validate_bus_json, CanBusConfig
from .parse_enum import parse_node_enum_data, parse_shared_enums
from .parse_error import InvalidCanJson
from .parse_utils import list_nodes_from_folders, load_json_file
from .parse_tx import parse_tx_data
from .parse_alert import parse_alert_data, AlertsEntry
from .parse_rx import parse_json_rx_data


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
        node_names = list_nodes_from_folders(can_data_dir)
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
        self._bus_config = parse_bus_data(can_data_dir)

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
        self._shared_enums = parse_shared_enums(can_data_dir)
        enums = self._shared_enums.copy()
        # enum data
        for node_name in node_names:
            # writes node enums into global enum bucket
            # TODO i wonder why this is not per board like literally everything else?
            node_enums = parse_node_enum_data(can_data_dir, node_name)
            enums.update(node_enums)

        # tx messages
        self._msgs = {}
        for node_name in node_names:
            for msg_name in parse_tx_data(can_data_dir, node_name, enums):
                self._add_tx_msg(msg_name, node_name)

        # Parse Alerts
        self._alerts = {}
        alert_msgs = []
        for node_name in node_names:
            # Parse ALERTS
            node_alert_msgs, alerts = parse_alert_data(can_data_dir, node_name)
            # since they are optional
            if node_alert_msgs is None or alerts is None:
                assert node_alert_msgs is None and alerts is None
                continue
            assert len(node_alert_msgs) == 6, "Alert messages should be 6 (unless we add more types of alerts)"

            # mutate
            for msg_name in node_alert_msgs:
                self._add_tx_msg(msg_name, node_name)
            # extra alerts logging?
            self._alerts[node_name] = alerts
            alert_msgs.extend(node_alert_msgs)  # save for RX parsing

        # Parse all nodes' RX JSON (have to do this last so all messages on this bus are already found, from TX JSON)
        # IMPORTANT: make sure to handle RX only after all the TX msgs are handled
        self._rx_msgs = {
            rx_node.name: CanRxMessages(node=rx_node.name, messages={})
            for rx_node in self._nodes.values()
        }
        for rx_node in self._nodes.values():
            # multiple buses can be defined in the RX JSON
            for rx_bus_metadata in parse_json_rx_data(can_data_dir, rx_node):
                bus, bus_rx_msg_names = rx_bus_metadata["bus"], rx_bus_metadata["messages"]
                # check bus is present
                if bus not in self._bus_config.keys():
                    raise InvalidCanJson(f"Bus '{bus}' is not defined in the bus JSON.")
                if "all" in bus_rx_msg_names:
                    # TODO maybe we just make the type of messages : list[str] | "all"
                    # if "all" in messages then add all messages on this bus
                    bus_rx_msg_names = list(set(self._msgs) - set(rx_node.tx_msg_names))
                for msg_name in bus_rx_msg_names:
                    self._add_rx_msg(msg_name, rx_node, bus)

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

    # TODO perhaps add a version which takes a list of msgs idk tho cuz this is not well parallelized
    def _add_tx_msg(self, msg: CanMessage, node_name: str) -> None:
        """
        This function registers a new message, transmitted by a node on the bus

        It
        1. adds the msg to the global dump of messages (self._msgs)
        2. adds the msg name to the list of messages broadcasted by the given node (self._nodes[node_name].tx_msg_names)
        """
        # Check if this message name is a duplicate
        if msg.name in self._msgs.keys():
            raise InvalidCanJson(
                f"Message '{msg.name}' transmitted by node '{node_name}' is also transmitted by '{self._msgs[msg.name].tx_node}'"
            )

        # Check if this message ID is a duplicate
        find = [m for m in self._msgs.values() if m.id == msg.id]
        if len(find) > 0:
            assert len(find) == 1, "There should only be one message with the same ID"
            raise InvalidCanJson(
                f"Message ID '{msg.id}' transmitted by node '{node_name}' is also transmitted by '{find[0].tx_node}'"
            )

        # mutate
        self._msgs[msg.name] = msg
        self._nodes[node_name].tx_msg_names.append(msg.name)

    def _add_rx_msg(self, msg_name: str, rx_node: CanNode, bus: str) -> None:
        """
        This function registers a message which is to be recieved by a node on the bus
        :return:
        """
        # Check if this message is defined
        if msg_name not in self._msgs.keys():
            raise InvalidCanJson(
                f"Message '{msg_name}' received by '{rx_node.name}' is not defined. Make sure it is correctly defined in the TX JSON."
            )
        msg_to_rx = self._msgs[msg_name]
        # tell msg_to_rx that the current node rxs it
        if rx_node.name not in msg_to_rx.rx_node_names:
            msg_to_rx.rx_node_names.append(rx_node.name)
        # add the message to the node's rx messages
        if msg_to_rx.name not in rx_node.rx_msg_names:  # TODO why do we need to check uniqueness? if they need to be unique just enforce with a set, and error if twice?
            rx_node.rx_msg_names.append(msg_to_rx.name)

        if bus not in self._rx_msgs[rx_node.name].messages:
            self._rx_msgs[rx_node.name].messages[bus] = []
        self._rx_msgs[rx_node.name].messages[bus].append(msg_name)

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
                    # TODO NO NO NO NO NO
                    self._rx_msgs[node.name].messages.setdefault(overlap_bus, []).append(
                        alerts_msg.name
                    )
                else:
                    # if the message is not on the bus then it is not received by this node
                    # need to reroute
                    # random pick a rx port from the node
                    # TODO do we need to resolve which routing needs to happen here?

                    rx_bus = node.bus_names[0] if node.bus_names else None
                    if rx_bus is None:
                        continue
                    alerts_msg.rx_node_names.extend(
                        [key for key in self._nodes.keys() if key != node]
                    )
                    if alerts_msg.name not in node.rx_msg_names:
                        node.rx_msg_names.append(alerts_msg.name)

                    # add rx message obj
                    # TODO NO NO NO NO NO
                    self._rx_msgs[node.name].messages.setdefault(rx_bus, []).append(
                        alerts_msg.name
                    )

    def _calculate_reroutes(self, can_data_dir) -> List[CanForward]:
        # design choice
        # all message is on FD bus
        # some message from FD bus need to be rerouted to non-FD bus

        # TODO do we try to do this with the bus config?
        try:
            forwarders_configs = _validate_bus_json(
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
