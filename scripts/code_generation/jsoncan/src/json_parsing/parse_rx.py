from typing import TypedDict, Dict
from schema import Schema, Or, SchemaError

from ..can_database import CanMessage, CanBusConfig, CanNode, CanRxMessages
from .parse_error import InvalidCanJson
from .parse_utils import load_json_file


class RxBusEntryJson(TypedDict):
    bus: str
    messages: list[str]


RxEntry_schema = Schema({
    "bus": str,
    "messages": [str],  # Use schema.List to define a list of strings
})
rx_entries_schema = Schema(
    Or(
        Schema([]),  # Allow an empty list
        Schema([RxEntry_schema]),
    )
)


def validate_rx_json(json: Dict) -> list[RxBusEntryJson]:
    return rx_entries_schema.validate(json)


def parse_json_rx_data(can_data_dir: str, _msgs: dict[str, CanMessage], _bus_config: dict[str, CanBusConfig],
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
