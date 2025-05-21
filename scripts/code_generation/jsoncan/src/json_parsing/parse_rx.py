from typing import TypedDict, Dict, List
from schema import Schema, Or, SchemaError

from .parse_error import InvalidCanJson
from .parse_utils import load_json_file
from ..can_database import CanNode


class _RxBusEntry(TypedDict):
    messages: list[str] | str


def _validate_rx_json(json: Dict) -> _RxBusEntry:
    return Schema(
        {
            "messages": Or(
                Schema([str]), Schema("all")
            ),  # Use schema.List to define a list of strings
        }
    ).validate(json)


def parse_json_rx_data(can_data_dir: str, rx_node_name: str) -> List[str] | str:
    """
    :param can_data_dir:
    note: this function validates that the rx_msg reads on a existant bus
    :param rx_node_name: Name of the rx node we are processing messages for
    :return:
    """
    try:
        node_rx_json_data = _validate_rx_json(
            load_json_file(f"{can_data_dir}/{rx_node_name}/{rx_node_name}_rx")
        )
    except SchemaError:
        raise InvalidCanJson(f"RX JSON file is not valid for node {rx_node_name}")
    return node_rx_json_data["messages"]
