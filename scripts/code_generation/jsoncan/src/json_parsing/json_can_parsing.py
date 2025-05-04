"""
Module for parsing CAN JSON, and returning a CanDatabase object.
"""

from __future__ import annotations

# types
from typing import List, Optional

from schema import SchemaError

from ..can_database import (
    CanAlert,
    CanDatabase,
    CanEnum,
    CanForward,
    CanMessage,
    CanNode,
    CanRxMessages,
)
from .parse_alert import AlertsEntry, parse_alert_data

# new files
from .parse_bus import CanBusConfig, _validate_bus_json, parse_bus_data
from .parse_enum import parse_node_enum_data, parse_shared_enums
from .parse_error import InvalidCanJson
from .parse_rx import parse_json_rx_data
from .parse_tx import parse_tx_data
from .parse_utils import list_nodes_from_folders, load_json_file


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
    _alerts: dict[
        str, dict[CanAlert, AlertsEntry]
    ]  # Dict of node names to node's alerts
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
                bus_names=[],
            )
            for node_name in node_names
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
            # TODO i wonder why this is not per board like literally everything else? - check app_can_utils_module only used enum will be generated
            node_enums = parse_node_enum_data(can_data_dir, node_name)
            enums.update(node_enums)

        # tx messages
        self._msgs = {}
        for node_name in node_names:
            for msg_name in parse_tx_data(can_data_dir, node_name, enums):
                self._add_tx_msg(msg_name, node_name)

        # Parse Alerts
        self._alerts = {}
        alert_msgs: list[CanMessage] = []
        for node_name in node_names:
            # Parse ALERTS
            out = parse_alert_data(can_data_dir, node_name)
            # since they are optional
            if out is None:
                continue
            node_alert_msgs, alerts = out
            assert (
                    len(node_alert_msgs) == 6
            ), "Alert messages should be 6 (unless we add more types of alerts)"

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
                bus, bus_rx_msg_names = (
                    rx_bus_metadata["bus"],
                    rx_bus_metadata["messages"],
                )
                # check bus is present
                if bus not in self._bus_config.keys():
                    raise InvalidCanJson(f"Bus '{bus}' is not defined in the bus JSON.")
                if "all" in bus_rx_msg_names:
                    # TODO maybe we just make the type of messages : list[str] | "all"
                    # if "all" in messages then add all messages on this bus
                    bus_rx_msg_names = list(set(self._msgs) - set(rx_node.tx_msg_names))
                for msg_name in bus_rx_msg_names:
                    self._add_rx_msg(msg_name, rx_node, bus)

        for alerts_msg in alert_msgs:
            for rx_node in self._nodes.values():
                if alerts_msg.tx_node == rx_node.name:
                    # skip the node that transmit the message
                    continue
                # check if the alert is broadcasted on a bus that is directly connected to the node
                overlap_bus: set[str] = set(alerts_msg.bus) & set(rx_node.bus_names)
                # if the message is not on the bus then it is not received by this node
                # need to reroute
                # random pick a rx port from the node
                rx_bus: Optional[str] = (
                    list(overlap_bus)[0]
                    if len(overlap_bus) > 0
                    else rx_node.bus_names[0] if rx_node.bus_names else None
                )
                if rx_bus is None:
                    continue
                self._add_rx_msg(alerts_msg.name, rx_node, rx_bus)

        # Consistency check
        self._consistency_check()

        # find all message transmitting on one bus but received in another bus
        # IMPORTANT: reroutes can only be calculated after all the RXs are figured out
        self._reroute_msgs = self._calculate_reroutes(can_data_dir)

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

        Note this function expects a valid CanMessage object
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
        # This function registers a message which is to be received by a node on the bus

        This function
        1. enters the current node into the list of nodes that the message is rx'd by
        2. adds the message name to the list of messages received by the given node
        3. adds an entry into _rx_msgs
        :return:
        """
        # Check if this message is defined
        if msg_name not in self._msgs.keys():
            raise InvalidCanJson(
                f"Message '{msg_name}' received by '{rx_node.name}' is not defined. Make sure it is correctly defined in the TX JSON."
            )
        msg_to_rx = self._msgs[msg_name]

        # check that the rx node is not the same as the node which txs msg_name
        if msg_to_rx.tx_node == rx_node.name:
            raise InvalidCanJson(
                f"Message '{msg_name}' is transmitted by '{msg_to_rx.tx_node}' and received by '{rx_node.name}'. A node cannot transmit and receive the same message."
            )

        # tell msg_to_rx that the current node rxs it
        if rx_node.name not in msg_to_rx.rx_node_names:
            msg_to_rx.rx_node_names.append(rx_node.name)
        # add the message to the node's rx messages
        if (
                msg_to_rx.name not in rx_node.rx_msg_names
        ):  # TODO why do we need to check uniqueness? if they need to be unique just enforce with a set, and error if twice? - can be a way to do it but I argue the set takes way more memory than a list. 
            rx_node.rx_msg_names.append(msg_to_rx.name)

        if bus not in self._rx_msgs[rx_node.name].messages:
            self._rx_msgs[rx_node.name].messages[bus] = []
        self._rx_msgs[rx_node.name].messages[bus].append(msg_name)

    def _consistency_check(self) -> None:
        # TODO should this be checked post-hoc, or should it be checked as you parse the messages. - you can add extra check here as the all the private object are closely related to each other 
        # just in case we need extra checks besides the ones in the parsing functions
        # In the latter method, you would be able to guarentee that when the intermediary data is ready, that it is valid.
        # however, I don't think this is very much part of the design philosophy at all, as there are many instances
        # where data is instantiated but not valid

        # no same message name, signal name, enum name TODO I think this is already checked in _add_tx_msg?
        # message can't transmit and receive by the same node TODO this is already checked in _add_rx_msg
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

                if msg_obj.tx_node not in self._nodes:
                    raise InvalidCanJson(f"Node '{node}' is not defined in the node JSON.")

    def _calculate_reroutes(self, can_data_dir) -> List[CanForward]:
        # design choice
        # all message is on FD bus
        # some message from FD bus need to be rerouted to non-FD bus
        # TODO just to note that this behaviour can be created by using the all rx message

        try:
            forwarders_configs = _validate_bus_json(load_json_file(f"{can_data_dir}/bus"))["forwarders"]
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

        reroute_msgs = []
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
                                forwarder=config["forwarder"],
                            )

                            reroute_msgs.append(forwarder)
                            break
                        if not found:
                            raise InvalidCanJson(
                                f"Forwarder config for bus '{rx_bus}' and bus '{tx_bus}' is not defined in the bus JSON for message {forward_msg}."
                            )
        return reroute_msgs
