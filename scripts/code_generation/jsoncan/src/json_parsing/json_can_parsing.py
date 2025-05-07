"""
Module for parsing CAN JSON, and returning a CanDatabase object.
"""

from __future__ import annotations

from collections import deque
from collections import defaultdict
# types
from typing import Dict, Optional, List, Tuple, Set

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
    CanNode, CanTxConfigs, CanRxConfigs, CanForward,
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
    _alerts: Dict[str, List[CanAlert]]  # _alerts[node_name] = dict[CanAlert, AlertsEntry]

    # internal state
    _node_tx_msgs: Dict[str, List[str]]  # _tx_msgs[node_name] gives a list of all the messages it txs
    _node_rx_msgs: Dict[str, List[str]]  # _rx_msgs[node_name] gives a list of all the messages it rxs

    def __init__(self, can_data_dir: str):
        """
        Parses JSON data
        :param can_data_dir: Location of all the json files
        """
        self._node_tx_msgs = {}
        self._node_rx_msgs = {}

        node_names: List[str] = list_nodes_from_folders(can_data_dir)
        # create node objects for each node
        self._nodes = {
            node_name: CanNode(node_name)
            for node_name in node_names
        }

        # parse the bus config
        self._busses, forwarder_config = parse_bus_data(can_data_dir)

        # populate self._nodes[node_name].bus_names
        # add busses each node is on
        for node_name in node_names:
            for bus_name in self._busses:
                if node_name in self._busses[bus_name].node_names:
                    self._nodes[node_name].add_bus(bus_name)

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
            if type(bus_rx_msgs_json["messages"]) == str:
                assert bus_rx_msgs_json["messages"] == "all", "Schema check has failed"
                # if "all" in messages then add all messages on this bus
                bus_rx_msg_names = list(set(self._msgs.keys()) - set(self._node_tx_msgs[rx_node.name]))
            else:
                bus_rx_msg_names = bus_rx_msgs_json["messages"]
            for msg_name in bus_rx_msg_names:
                self._add_rx_msg(msg_name, rx_node)

        # PARSE ALERTS DATA
        self._alerts = {}
        alert_msgs: List[CanMessage] = []
        for node_name in node_names:
            # Parse ALERTS
            alerts_json = parse_alert_data(can_data_dir, node_name)
            # since they are optional
            if alerts_json is None:
                continue
            node_alert_msgs, alerts = alerts_json
            assert len(node_alert_msgs) == 6, "Alert messages should be 6"
            self._alerts[node_name] = alerts

            for alert_msg in node_alert_msgs:
                self._add_tx_msg(alert_msg, node_name)  # tx handling
            alert_msgs.extend(node_alert_msgs)

        # handling alerts rx, note that only other alert broadcasters can receive the alert
        for alert_msg in alert_msgs:
            for other_rx_node_name in self._alerts.keys():  # rx handling
                other_rx_node = self._nodes[other_rx_node_name]
                # skip the node that transmit the message
                if alert_msg.tx_node_name == other_rx_node.name: continue
                self._add_rx_msg(alert_msg.name, other_rx_node)

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

    @staticmethod
    def _fast_fourier_transform_stochastic_gradient_descent(adj_list: Dict[str, List[Tuple[str, str]]],
                                                            tx_node: CanNode,
                                                            rx_node: CanNode) -> \
            Tuple[str, str, List[Tuple[str, str, str]]]:
        previous_node: Dict[str, Optional[str]] = {}
        previous_edge: Dict[str, str] = {}
        destination_nodes: Set[str] = set()
        queue: deque[str] = deque()

        # populate w/ start nodes
        for bus_name in tx_node.bus_names:
            queue.append(bus_name)
            previous_node[bus_name] = None

        # population destination w/ end nodes
        for bus_name in rx_node.bus_names:
            destination_nodes.add(bus_name)

        # basic bfs
        target_node: Optional[str] = None
        while len(queue) > 0:
            cur_node = queue.popleft()
            if cur_node in destination_nodes:
                target_node = cur_node
                break
            for (next_node, edge) in adj_list[cur_node]:
                if next_node not in previous_node:
                    previous_node[next_node] = cur_node
                    previous_edge[next_node] = edge
                    queue.append(next_node)

        # graph is disconnected
        if target_node is None:
            raise InvalidCanJson(f"Unreachable CAN message, likely error in forwarder topology")

        # recover path
        best_path: List[Tuple[str, Optional[str]]] = []
        while previous_node[target_node] is not None:
            best_path.append((target_node, previous_edge[target_node]))
            target_node = previous_node[target_node]
            if target_node in previous_node and target_node == previous_node[target_node]:
                raise InvalidCanJson(f"Unreachable CAN message, likely error in forwarder topology")
        best_path.append((target_node, None))
        best_path.reverse()  # TODO not necessary, just interpret the list backwards

        # if counter == 0:
        #     print(previous_node)
        #     print(previous_edge)
        #     print(tx_node)
        #     print(rx_node)
        #     print(adj_list)
        #     raise InvalidCanJson(f"Unreachable CAN message, likely error in forwarder topology")

        # parse some stuff
        initial_node = best_path[0][0]
        final_node = best_path[-1][0]
        rerouter_nodes: List[Tuple[str, str, str]] = []
        for index in range(1, len(best_path)):
            rerouter_nodes.append((best_path[index][1], best_path[index - 1][0], best_path[index][0]))

        return initial_node, final_node, rerouter_nodes

    def _resolve_tx_rx_reroute(self, forwarder_config: List[ForwarderConfigJson]) -> None:
        # make all forwarders
        # OVERVIEW
        # what we gotta do is make a graph representation of canBuses
        # where buses are connected by edges which are boards
        # then we create start nodes -> buses connected with TX node
        # and destination nodes -> buses connected with RX node
        # and we run BFS, keeping track of three things
        # 1. shortest path to each bus (all destination nodes init as 0)
        # 2. previous bus to each bus on shortest path
        # 3. previous edge to each bus on shortest path
        # we recover the path we want by finding the destination node w/ the shortest distance
        # and backtrack 
        # lastly to update our config we do the following
        # we make the first tx board forward to the first bus
        # for the buses/boards in order
        # we add a rerouter config for board i from {prev bus, next bus}
        # we make the last rx board take shit in from the last bus 

        # graph representation of canBuses
        # im sorry this is about to be just tuples and shit
        adj_list: Dict[str, List[Tuple[str, str]]] = defaultdict(list)

        for forwarder_json in forwarder_config:
            forwarder_node_name = forwarder_json["forwarder"]
            if forwarder_node_name not in self._nodes:
                raise InvalidCanJson(f"Forwarder node '{forwarder_node_name}' is not defined in the node JSON.")
            self._nodes[forwarder_node_name].reroute_config = []
            bus1 = forwarder_json["bus1"]
            bus2 = forwarder_json["bus2"]
            if bus1 not in self._busses:
                raise InvalidCanJson(f"Forwarder bus '{bus1}' is not defined in the node JSON.")
            if bus2 not in self._busses:
                raise InvalidCanJson(f"Forwarder bus '{bus2}' is not defined in the node JSON.")
            if bus1 not in adj_list:
                adj_list[bus1] = []
            if bus2 not in adj_list:
                adj_list[bus2] = []
            adj_list[bus1].append((bus2, forwarder_node_name))
            adj_list[bus2].append((bus1, forwarder_node_name))

        for msg in self._msgs.values():
            if len(msg.rx_node_names) <= 0:
                print(f"[WARN] Message {msg.name} has no receiver")
                continue
            tx_node = self._nodes[msg.tx_node_name]
            tx_node.tx_config.add_tx_msg(msg.name)

            for rx_node_name in msg.rx_node_names:
                rx_node = self._nodes[rx_node_name]
                initial_node_tx_bus, final_node_rx_bus, rerouter_nodes = self._fast_fourier_transform_stochastic_gradient_descent(
                    adj_list, tx_node, rx_node)
                tx_node.tx_config.add_bus_to_tx_msg(msg.name, initial_node_tx_bus)
                rx_node.rx_config.add_rx_msg(msg.name, final_node_rx_bus)
                for rerouter_node in rerouter_nodes:
                    self._nodes[rerouter_node[0]].reroute_config.append(
                        CanForward(msg.name, rerouter_node[0], rerouter_node[1], rerouter_node[2]))

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
