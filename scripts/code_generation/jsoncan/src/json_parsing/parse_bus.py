from typing import Dict, Optional as Optional_t, TypedDict, List, Tuple
from schema import Optional, Or, Schema, SchemaError

from .parse_error import InvalidCanJson
from .parse_utils import load_json_file
from ..can_database import CanBus, BusForwarder


class _BusForwarderJson(TypedDict):
    forwarder: str
    bus1: str
    bus2: str


class _BusConfigJson(TypedDict):
    name: str
    bus_speed: int
    modes: list[str]
    default_mode: str
    nodes: list[str]
    fd: Optional_t[bool]


class _BusJson(TypedDict):
    forwarders: list[_BusForwarderJson]
    buses: list[_BusConfigJson]
    logger: Optional_t[list[str]]


_BusJson_schema = Schema(
    {
        "forwarders": Or(
            Schema([]),
            Schema(
                [
                    {
                        "forwarder": str,
                        "bus1": str,
                        "bus2": str,
                    }
                ]
            ),
        ),
        "buses": Or(
            Schema([]),
            Schema(
                [
                    {
                        "name": str,
                        "bus_speed": int,
                        "modes": [str],
                        "default_mode": str,
                        "nodes": [str],
                        Optional("fd"): bool,
                    }
                ]
            ),
        ),
        Optional("loggers"): Schema([str]),
    }
)


def _validate_bus_json(json: Dict) -> _BusJson:
    return _BusJson_schema.validate(json)


def parse_bus_data(
    can_data_dir: str, node_names: List[str]
) -> Tuple[Dict[str, CanBus], List[BusForwarder], List[str]]:
    """
    Parses data about buses from global configuration
    CONSISTENCY: bus.default_mode not in bus.modes
    """
    try:
        bus_json_data = _validate_bus_json(load_json_file(f"{can_data_dir}/bus"))
    except SchemaError:
        raise InvalidCanJson("Bus JSON file is not valid")

    # dynamic validation of bus data
    busses = {}
    for bus in bus_json_data["buses"]:
        if bus["default_mode"] not in bus["modes"]:
            raise InvalidCanJson(
                f"Error on bus {bus['name']}: Default CAN mode is not in the list of modes."
            )
        # check that the nodes in the busses are all valid
        for node in bus["nodes"]:
            if node not in node_names:
                raise InvalidCanJson(f"Node '{node}' is not defined in the node JSON.")
        busses[bus["name"]] = CanBus(
            name=bus["name"],
            default_mode=bus["default_mode"],
            modes=bus["modes"],
            bus_speed=bus["bus_speed"],
            node_names=bus["nodes"],
            fd=bus.get("fd", False),
        )

    forwarders = []
    for forwarder in bus_json_data["forwarders"]:
        if forwarder["bus1"] not in busses:
            raise InvalidCanJson(
                f"Bus '{forwarder['bus1']}' is not defined in the bus JSON."
            )
        if forwarder["bus2"] not in busses:
            raise InvalidCanJson(
                f"Bus '{forwarder['bus2']}' is not defined in the bus JSON."
            )
        if forwarder["forwarder"] not in node_names:
            raise InvalidCanJson(
                f"Forwarder '{forwarder['forwarder']}' is not defined in the node JSON."
            )
        forwarders.append(
            BusForwarder(forwarder["forwarder"], forwarder["bus1"], forwarder["bus2"])
        )

    loggers = []
    if "logger" in bus_json_data:
        for logger in bus_json_data["logger"]:
            if logger not in node_names:
                raise InvalidCanJson(
                    f"Logger '{logger}' is not defined in the node JSON."
                )
            loggers.append(logger)

    return busses, forwarders, loggers
