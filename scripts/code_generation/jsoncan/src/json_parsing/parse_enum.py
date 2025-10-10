from schema import Schema, Or, SchemaError
from typing import Dict
from .parse_error import InvalidCanJson
from ..can_signals import CanEnum
from .parse_utils import load_json_file

enum_schema = Schema(
    Or(Schema({str: {str: int}}), Schema({}))
)  # If the node doesn"t define any enums


def validate_enum_json(json: Dict) -> Dict[str, Dict[str, int]]:
    return enum_schema.validate(json)


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


def parse_node_enum_data(can_data_dir: str, node_name: str) -> dict[str, CanEnum]:
    """
    Adds node_name's enums to the given _enum dictionary
    """
    try:
        node_enum_json_data = validate_enum_json(
            load_json_file(f"{can_data_dir}/{node_name}/{node_name}_enum")
        )
    except SchemaError:
        raise InvalidCanJson(f"Enum JSON file is not valid for node {node_name}")

    new_enums = {}
    for enum_name, enum_entries in node_enum_json_data.items():
        enum = _get_parsed_can_enum(enum_name=enum_name, enum_entries=enum_entries)
        if enum in new_enums:
            raise InvalidCanJson(
                f"Enum '{enum_name}' is a duplicate, enums must have unique names."
            )
        new_enums[enum_name] = enum
    return new_enums


def parse_shared_enums(can_data_dir: str) -> dict[str, CanEnum]:
    """
    Parse shared enum JSON data from specified directory.
    """
    try:
        shared_enum_json_data = validate_enum_json(
            load_json_file(f"{can_data_dir}/shared_enum")
        )
    except SchemaError:
        raise InvalidCanJson("Shared enum JSON file is not valid")
    enums = {}
    # Parse shared enum JSON
    for enum_name, enum_entries in shared_enum_json_data.items():
        # Check if this enum name is a duplicate
        can_enum: CanEnum = _get_parsed_can_enum(
            enum_name=enum_name, enum_entries=enum_entries
        )
        if can_enum in enums:
            raise InvalidCanJson(
                f"Enum '{enum_name}' is a duplicate, enums must have unique names."
            )
        enums[enum_name] = can_enum
    return enums
