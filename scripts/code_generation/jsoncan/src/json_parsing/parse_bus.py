from typing import Dict
from typing import Optional as Optional_t
from typing import TypedDict

from schema import Optional, Or, Schema, SchemaError

from .parse_error import InvalidCanJson
from .parse_utils import load_json_file
from ..can_database import CanBus


class ForwarderConfigJson(TypedDict):
    forwarder: str
    bus1: str
    bus2: str


class _BusConfigJson(TypedDict):
    name: str
    bus_speed: int
    modes: list[str]
    default_mode: str
    nodes: list[str]
    FD: Optional_t[bool]


class _BusJson(TypedDict):
    forwarders: list[ForwarderConfigJson]
    buses: list[_BusConfigJson]


_BusJson_schema = Schema({
    "forwarders": Or(Schema([]), Schema([{
        "forwarder": str,
        "bus1": str,
        "bus2": str,
    }])),
    "buses": Or(Schema([]), Schema([{
        "name": str,
        "bus_speed": int,
        "modes": [str],
        "default_mode": str,
        "nodes": [str],
        Optional("FD"): bool,
    }]))
})


def _validate_bus_json(json: Dict) -> _BusJson:
    return _BusJson_schema.validate(json)


def parse_bus_data(can_data_dir: str) -> tuple[dict[str, CanBus], list[ForwarderConfigJson]]:
    """
    Parses data about buses from global configuration
    CONSISTENCY: bus.default_mode not in bus.modes
    """
    try:
        bus_json_data = _validate_bus_json(load_json_file(f"{can_data_dir}/bus"))
    except SchemaError:
        raise InvalidCanJson("Bus JSON file is not valid")

    # dynamic validation of bus data
    buses = bus_json_data["buses"]

    for bus in buses:
        if bus["default_mode"] not in bus["modes"]:
            raise InvalidCanJson(f"Error on bus {bus['name']}: Default CAN mode is not in the list of modes.")

    return {
        bus["name"]: CanBus(
            name=bus["name"],
            default_mode=bus["default_mode"],
            modes=bus["modes"],
            bus_speed=bus["bus_speed"],
            node_names=bus["nodes"],
            fd=bus.get("FD", False),
        )
        for bus in buses
    }, bus_json_data["forwarders"]
