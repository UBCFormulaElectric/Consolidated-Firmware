"""
Module for parsing CAN JSON, and returning a CanDatabase object.
"""

from __future__ import annotations

# types
from typing import Dict, List, Set

# types to populate up
from ..can_database import CanDatabase, CanMessage, CanNode, AllRxMsgs, BusForwarder
from ..can_signals import CanEnum

from .parse_alert import parse_alert_data, CanAlert
from .parse_bus import CanBus, parse_bus_data
from .parse_enum import parse_node_enum_data, parse_shared_enums
from .parse_error import InvalidCanJson
from .parse_rx import parse_json_rx_data
from .parse_tx import parse_tx_data
from .parse_utils import list_nodes_from_folders


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
    _alerts: Dict[
        str, List[CanAlert]
    ]  # _alerts[node_name] = dict[CanAlert, AlertsEntry]
    _forwarding: List[BusForwarder]  # _forwarding[bus_name] gives metadata for bus_name
    _enums: Dict[str, CanEnum]  # _enums[enum_name] gives metadata for enum_name
    _collects_data: Dict[
        str, bool
    ]  # _collects_data[node_name] = True if the node collects data
    _signals_to_msgs: Dict[str, CanMessage]  # _signals_to_msgs[signal_name] gives the metadata for the message that contains the signal

    def __init__(self, can_data_dir: str):
        """
        Parses JSON data
        :param can_data_dir: Location of all the json files
        """
        node_names: List[str] = list_nodes_from_folders(can_data_dir)

        # parse the bus config
        self._busses, self._forwarding, loggers = parse_bus_data(
            can_data_dir, node_names
        )
        self._collects_data = {
            node_name: node_name in loggers for node_name in node_names
        }

        # create node objects for each node
        self._nodes = {
            node_name: CanNode(
                name=node_name,
                bus_names=[
                    bus_name
                    for bus_name, bus in self._busses.items()
                    if node_name in bus.node_names
                ],
                rx_msgs_names=set(),
                fd=any(
                    node_name in bus.node_names and bus.fd
                    for bus in self._busses.values()
                ),
            )
            for node_name in node_names
        }

        # ngl im not the biggest fan of executing things inside declarations
        for node_name, node in self._nodes.items():
            if len(node.bus_names) == 0:
                raise InvalidCanJson("Node %s has no bus", node_name)

        # PARSE TX JSON DATA
        # collect shared enums outside of loop
        self._enums = {}
        shared_enums = parse_shared_enums(can_data_dir)
        self._enums.update(shared_enums)
        # populate this boy
        self._msgs = {}
        self._signals_to_msgs = {}
        for node_name in node_names:
            node_specific_enums = parse_node_enum_data(can_data_dir, node_name)
            self._enums.update(node_specific_enums)
            node_enums: dict[str, CanEnum] = {**shared_enums, **node_specific_enums}
            for tx_msg in parse_tx_data(can_data_dir, node_name, node_enums):
                self._add_tx_msg(tx_msg, node_name)

        # PARSE RX JSON
        for rx_node_name in node_names:
            bus_rx_msgs_json = parse_json_rx_data(can_data_dir, rx_node_name)
            if type(bus_rx_msgs_json) == str:
                assert bus_rx_msgs_json == "all", "Schema check has failed"
                # if "all" in messages then add all messages on all busses
                self._nodes[rx_node_name].rx_msgs_names = AllRxMsgs()
            else:
                assert type(bus_rx_msgs_json) == list, "Schema check has failed"
                for msg_name in bus_rx_msgs_json:
                    self._add_rx_msg(msg_name, rx_node_name)

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
                # skip the node that transmit the message
                if alert_msg.tx_node_name == other_rx_node_name:
                    continue
                self._add_rx_msg(alert_msg.name, other_rx_node_name)

        # CONSISTENCY TODO work this in?
        # self._consistency_check()

    def make_database(self) -> CanDatabase:
        """
        Make and return CanDatabase object form the parsed data.
        """
        return CanDatabase(
            nodes=self._nodes,
            buses=self._busses,
            msgs=self._msgs,
            alerts=self._alerts,
            forwarding=self._forwarding,
            enums=self._enums,
            collects_data=self._collects_data,
            signals_to_msgs=self._signals_to_msgs,
        )

    # TODO perhaps add a version which takes a list of msgs idk tho cuz this is not well parallelized
    def _add_tx_msg(self, msg: CanMessage, tx_node_name: str) -> None:
        """
        This function registers a new message, transmitted by a node on the bus

        It
        1. adds the msg to the global dump of messages (self._msgs)
        2. adds the msg name to the list of messages broadcasted by the given node (self._nodes[node_name].tx_msg_names)
        3. adds all the signals into the global dump of signals (self._signals_to_msgs)

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

        if msg.requires_fd() and not self._nodes[tx_node_name].fd:
            raise InvalidCanJson(
                f"Message '{msg.name}' is an FD message, but its TX node '{tx_node_name}' isn't FD-capable and so can't transmit it!"
            )

        # register the message with the database of all messages
        self._msgs[msg.name] = msg

        for signal in msg.signals:
            # register the signal with the database of all signals
            if signal.name in self._signals_to_msgs:
                raise InvalidCanJson(
                    f"Signal '{signal.name}' is already registered in message '{self._signals_to_msgs[signal.name].name}'"
                )
            self._signals_to_msgs[signal.name] = msg

    def _add_rx_msg(self, msg_name: str, rx_node_name: str) -> None:
        """
        This function registers a message which is to be received by a node on the bus

        Note that this function does not know
        1. which bus the sender will tx will send the message on
        2. which bus the receiver will rx the message on

        This function
        1. enters the current node into the list of nodes that the message is rx'd by
        2. adds the message name to the list of messages received by the given node
        """
        # if we are already listening to all, we don't need to register this specific message
        # in particular, this is useful for alerts which will blindly make everyone accept them
        if type(self._nodes[rx_node_name].rx_msgs_names) == AllRxMsgs:
            return

        # Check if this message is defined
        if msg_name not in self._msgs:
            raise InvalidCanJson(
                f"Message '{msg_name}' received by '{rx_node_name}' is not defined. Make sure it is correctly defined in the TX JSON."
            )

        rx_msg = self._msgs[msg_name]
        if rx_msg.tx_node_name == rx_node_name:
            raise InvalidCanJson(
                f"{rx_node_name} cannot both transmit and receive {msg_name}"
            )

        # we already check if self._nodes[rx_node_name].rx_msgs_names is AllRxMsgs
        if msg_name in self._nodes[rx_node_name].rx_msgs_names: # type: ignore
            raise InvalidCanJson(
                f"Message {msg_name} is already registered to be received by node {rx_node_name}"
            )

        if rx_msg.requires_fd() and not self._nodes[rx_node_name].fd:
            raise InvalidCanJson(
                f"Message '{msg_name}' is an FD message, but an RX node '{rx_node_name}' isn't FD-capable and so can't receive it!"
            )

        # we already check if self._nodes[rx_node_name].rx_msgs_names is AllRxMsgs
        self._nodes[rx_node_name].rx_msgs_names.add(msg_name) # type: ignore

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
