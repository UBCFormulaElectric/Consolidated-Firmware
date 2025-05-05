"""
Module for parsing CAN JSON, and returning a CanDatabase object.
"""

from __future__ import annotations

from collections import defaultdict
# types
from typing import Dict

from .parse_alert import parse_alert_data, CanAlert
# new files
from .parse_bus import CanBus, parse_bus_data, ForwarderConfigJson
from .parse_enum import parse_node_enum_data, parse_shared_enums
from .parse_error import InvalidCanJson
from .parse_rx import parse_json_rx_data
from .parse_tx import parse_tx_data
from .parse_utils import list_nodes_from_folders
from ..can_database import (
    CanDatabase,
    CanEnum,
    CanMessage,
    CanNode, CanTxConfigs, CanRxConfigs,
)


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
    _nodes: Dict[str, CanNode]  # _nodes[node_name] gives metadata for node_name
    _busses: Dict[str, CanBus]  # _bus_config[bus_name] gives metadata for bus_name
    _msgs: Dict[str, CanMessage]  # _msgs[msg_name] gives metadata for msg_name
    _alerts: Dict[str, list[CanAlert]]  # _alerts[node_name] = dict[CanAlert, AlertsEntry]

    # internal state
    _node_tx_msgs: Dict[str, list[str]]  # _tx_msgs[node_name] gives a list of all the messages it txs
    _node_rx_msgs: Dict[str, list[str]]  # _rx_msgs[node_name] gives a list of all the messages it rxs

    def __init__(self, can_data_dir: str):
        """
        Parses JSON data
        :param can_data_dir: Location of all the json files
        """
        self._node_tx_msgs = {}
        self._node_rx_msgs = {}

        node_names: list[str] = list_nodes_from_folders(can_data_dir)
        # create node objects for each node
        self._nodes = {
            node_name: CanNode(
                name=node_name,
                bus_names=[],
                tx_config=CanTxConfigs(defaultdict()),
                rx_config=CanRxConfigs(defaultdict(), defaultdict(), []),
            )
            for node_name in node_names
        }

        # parse the bus config
        self._busses, forwarder_config = parse_bus_data(can_data_dir)

        # populate self._nodes[node_name].bus_names
        # add busses each node is on
        for node_name in node_names:
            self._nodes[node_name].bus_names = [
                self._busses[bus_name].name
                for bus_name in self._busses
                if node_name in self._busses[bus_name].node_names
            ]

        # PARSE TX JSON DATA
        # collect shared enums outside of loop
        shared_enums = parse_shared_enums(can_data_dir)
        # populate this boy
        self._msgs = {}
        for node_name in node_names:
            node_enums: dict[str, CanEnum] = {
                **shared_enums,
                **parse_node_enum_data(can_data_dir, node_name)
            }
            for tx_msg in parse_tx_data(can_data_dir, node_name, node_enums):
                self._add_tx_msg(tx_msg, node_name)

        # PARSE RX JSON
        for rx_node in self._nodes.values():
            # multiple buses can be defined in the RX JSON
            bus_rx_msgs_json = parse_json_rx_data(can_data_dir, rx_node)
            if type(bus_rx_msgs_json) == str:
                assert bus_rx_msgs_json == "all", "Schema check has failed"
                # if "all" in messages then add all messages on this bus
                bus_rx_msg_names = list(set(self._msgs.keys()) - set(rx_node.tx_config.list_msg_names()))
            else:
                bus_rx_msg_names = bus_rx_msgs_json["messages"]
            for msg_name in bus_rx_msg_names:
                self._add_rx_msg(msg_name, rx_node)

        # PARSE ALERTS DATA
        self._alerts = {}
        for node_name in node_names:
            # Parse ALERTS
            alerts_json = parse_alert_data(can_data_dir, node_name)
            # since they are optional
            if alerts_json is None:
                continue
            node_alert_msgs, alerts = alerts_json
            assert len(node_alert_msgs) == 6, "Alert messages should be 6"
            for alert_msg in node_alert_msgs:
                self._add_tx_msg(alert_msg, node_name)  # tx handling
                for other_rx_node in self._nodes.values():  # rx handling
                    # skip the node that transmit the message
                    if node_name == other_rx_node.name: continue
                    self._add_rx_msg(alert_msg.name, other_rx_node)
            self._alerts[node_name] = alerts

        # CONSISTENCY TODO work this in?
        # self._consistency_check()

        self._resolve_tx_rx_reroute(forwarder_config)

    def make_database(self) -> CanDatabase:
        """
        Make and return CanDatabase object form the parsed data.
        """
        return CanDatabase(
            nodes=self._nodes,
            busses=self._busses,
            msgs=self._msgs,
            alerts=self._alerts,
        )

    # TODO perhaps add a version which takes a list of msgs idk tho cuz this is not well parallelized
    def _add_tx_msg(self, msg: CanMessage, tx_node_name: str) -> None:
        """
        This function registers a new message, transmitted by a node on the bus

        It
        1. adds the msg to the global dump of messages (self._msgs)
        2. adds the msg name to the list of messages broadcasted by the given node (self._nodes[node_name].tx_msg_names)

        Note this function expects a valid CanMessage object
        """
        # Check if this message name is a duplicate
        if msg.name in self._msgs:
            raise InvalidCanJson(
                f"Message '{msg.name}' transmitted by node '{tx_node_name}' is also transmitted by '{self._msgs[msg.name].tx_node_name}'"
            )

        # Check if this message ID is a duplicate
        find = [m for m in self._msgs.values() if m.id == msg.id]
        if len(find) > 0:
            assert len(find) == 1, "There should only be one message with the same ID"
            raise InvalidCanJson(
                f"Message ID '{msg.id}' transmitted by node '{tx_node_name}' is also transmitted by '{find[0].tx_node_name}'"
            )

        self._msgs[msg.name] = msg
        if tx_node_name not in self._node_tx_msgs:
            self._node_tx_msgs[tx_node_name] = []
        self._node_tx_msgs[tx_node_name].append(msg.name)

    def _add_rx_msg(self, msg_name: str, rx_node: CanNode) -> None:
        """
        This function registers a message which is to be received by a node on the bus

        Note that this function does not know
        1. which bus the sender will tx will send the message on
        2. which bus the reciever will rx the message on

        This function
        1. enters the current node into the list of nodes that the message is rx'd by
        2. adds the message name to the list of messages received by the given node
        """
        # Check if this message is defined
        if msg_name not in self._msgs:
            raise InvalidCanJson(
                f"Message '{msg_name}' received by '{rx_node.name}' is not defined. Make sure it is correctly defined in the TX JSON."
            )
        rx_msg = self._msgs[msg_name]
        # check that the rx node is not the same as the node which txs msg_name
        if rx_msg.tx_node_name == rx_node.name:
            raise InvalidCanJson(
                f"{rx_node.name} cannot both transmit and receive {msg_name}"
            )
        # tell rx_msg that the current node rxs it
        # add the message to the node's rx messages
        if rx_node.name not in self._node_rx_msgs:
            self._node_rx_msgs[rx_node.name] = []
        if rx_node.name in rx_msg.rx_node_names or msg_name in self._node_rx_msgs[rx_node.name]:
            assert rx_node.name in rx_msg.rx_node_names and msg_name in self._node_rx_msgs[
                rx_node.name], "should have been added together"
            raise InvalidCanJson(f"Message {msg_name} is already registered to be received by node {rx_node.name}")

        rx_msg.rx_node_names.append(rx_node.name)  # TODO is this necessary??
        self._node_rx_msgs[rx_node.name].append(msg_name)

    def _resolve_tx_rx_reroute(self, forwarder_config: list[ForwarderConfigJson]) -> None:
        # make all forwarders
        for f_config in forwarder_config:
            node_name = f_config["forwarder"]
            if node_name not in self._nodes:
                raise InvalidCanJson(f"Forwarder node '{node_name}' is not defined in the node JSON.")
            # init the reroute_config
            self._nodes[node_name].reroute_config = {}
            # TODO add edge between f_config["bus1"] and f_config["bus2"]

        for msg in self._msgs.values():
            if len(msg.rx_node_names) <= 0:
                print(f"[WARN] Message {msg.name} has no receiver")
                continue
            tx_node = self._nodes[msg.tx_node_name]
            for rx_node_name in msg.rx_node_names:
                rx_node = self._nodes[rx_node_name]
                # TODO find shortest path between tx_node.bus_names and rx_node.bus_names
                # this will return
                # tx_bus_name: CanBus, rx_bus: CanBus, rerouters: list[CanForward]

                # tx_node.tx_config.add_tx(...)
                # rx_node.rx_config.add_rx(...)

                # since the rerouter will be created with the message metadata, we can guarentee that it is unique with all other messages in the array
                # rerouter_nodes: list[str]
                # for rerouter_node in rerouter_nodes:
                #     add config to rerouter_node.reroute_config

    # def _depr_calculate_reroutes(self) -> List[CanForward]:
    #     # design choice
    #     # all message is on FD bus
    #     # some message from FD bus need to be rerouted to non-FD bus
    #     # TODO just to note that this behaviour can be created by using the all rx message
    #
    #     # a map bus name to set of tx messages
    #     bus_tx_messages = {bus: set() for bus in self._busses}
    #
    #     # a map bus name to set of rx messages
    #     bus_rx_messages = {bus: set() for bus in self._busses}
    #
    #     for _, msg in self._msgs.items():
    #         tx_buses = msg.bus
    #         for bus in tx_buses:
    #             if bus not in bus_tx_messages:
    #                 raise InvalidCanJson(
    #                     f"Message '{msg.name}' is not defined in the bus JSON."
    #                 )
    #
    #             bus_tx_messages[bus].add(msg.name)
    #         for rx_node, _ in self._nodes.items():
    #             rx_bus = self._rx_msgs[rx_node].find_bus(msg.name)
    #             if rx_bus is None:
    #                 continue
    #             bus_rx_messages[rx_bus].add(msg.name)
    #
    #     reroute_msgs = []
    #     for bus_name, bus_obj in self._busses.items():
    #         tx_messages = bus_tx_messages[bus_name]
    #         rx_messages = bus_rx_messages[bus_name]
    #
    #         # rx message that is not transmitted on the current bus
    #         forward_messages = rx_messages - tx_messages
    #         rx_bus = bus_name
    #         # if there is a rx message that is not in the tx message then the message is from another bus so we need to reroute it
    #
    #         for forward_msg in forward_messages:
    #             # find the bus that the message is transmited on
    #             tx_buses = self._msgs[forward_msg].bus
    #
    #             # check forwarder config to figure out the which bus to forward to
    #             for tx_bus in tx_buses:
    #                 if tx_bus == bus_name:
    #                     raise InvalidCanJson(
    #                         "Tx bus and rx bus should not be the same at this stage. Something is wrong"
    #                     )
    #                 for config in forwarders_configs:
    #                     found = False
    #                     if (rx_bus == config["bus1"] and tx_bus == config["bus2"]) or (
    #                             rx_bus == config["bus2"] and tx_bus == config["bus1"]
    #                     ):
    #                         # found = True
    #
    #                         # create a forwarder object
    #                         forwarder = CanForward(
    #                             from_bus=tx_bus,
    #                             to_bus=rx_bus,
    #                             message=forward_msg,
    #                             forwarder=config["forwarder"],
    #                         )
    #
    #                         reroute_msgs.append(forwarder)
    #                         break
    #                     if not found:
    #                         raise InvalidCanJson(
    #                             f"Forwarder config for bus '{rx_bus}' and bus '{tx_bus}' is not defined in the bus JSON for message {forward_msg}."
    #                         )
    #     return reroute_msgs

    # def _consistency_check(self) -> None:
    #     # TODO should this be checked post-hoc, or should it be checked as you parse the messages. - you can add extra check here as the all the private object are closely related to each other
    #     # just in case we need extra checks besides the ones in the parsing functions
    #     # In the latter method, you would be able to guarentee that when the intermediary data is ready, that it is valid.
    #     # however, I don't think this is very much part of the design philosophy at all, as there are many instances
    #     # where data is instantiated but not valid
    #
    #     # no same message name, signal name, enum name TODO I think this is already checked in _add_tx_msg?
    #     # message can't transmit and receive by the same node TODO this is already checked in _add_rx_msg
    #     # no overlapping bus
    #     # object cross reference consistency
    #
    #     for node_name, node_obj in self._nodes.items():
    #         node_buses = node_obj.bus_names
    #         # check if the bus exist in the bus config
    #         for bus in node_buses:
    #             if bus not in self._busses:
    #                 raise InvalidCanJson(f"Bus '{bus}' is not defined in the bus JSON.")
    #
    #         for rx_msg in node_obj.rx_msg_names:
    #             if rx_msg not in self._msgs:
    #                 raise InvalidCanJson(
    #                     f"Message '{rx_msg}' received by '{node_name}' is not defined. Make sure it is correctly defined in the TX JSON."
    #                 )
    #
    #         for tx_msg in node_obj.tx_msg_names:
    #             if tx_msg not in self._msgs:
    #                 raise InvalidCanJson(
    #                     f"Message '{tx_msg}' transmitted by '{node_name}' is not defined. Make sure it is correctly defined in the TX JSON."
    #                 )
    #
    #     # bus consistency check
    #     for bus_name, bus_obj in self._busses.items():
    #         for node in bus_obj.node_names:
    #             if node not in self._nodes:
    #                 raise InvalidCanJson(
    #                     f"Node '{node}' is not defined in the node JSON."
    #                 )
    #         # need record what message is transmitting on this bus
    #
    #     # message consistency check
    #     for msg_name, msg_obj in self._msgs.items():
    #         for node in msg_obj.rx_node_names:
    #             if node not in self._nodes:
    #                 raise InvalidCanJson(
    #                     f"Node '{node}' is not defined in the node JSON."
    #                 )
    #
    #             if msg_obj.tx_node_name not in self._nodes:
    #                 raise InvalidCanJson(f"Node '{node}' is not defined in the node JSON.")
