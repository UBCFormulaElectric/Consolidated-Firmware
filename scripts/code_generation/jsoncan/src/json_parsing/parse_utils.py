import json
import os
from typing import Dict

from .parse_error import InvalidCanJson


def load_json_file(file_path: str) -> Dict:
    """
    Load an individual JSON file from specified path.
    """
    with open(f"{file_path}.json") as file:
        try:
            data = json.load(file)
            return data
        except json.JSONDecodeError:
            raise InvalidCanJson(
                f"Error parsing JSON data from file path '{file_path}'."
            )


def get_optional_value(data: Dict, key: str, default: any) -> tuple[str or any, bool]:
    """
    Parse a value from a key in data. If key not found, return default.
    """
    if key in data:
        return data[key], True
    else:
        return default, False


def list_nodes_from_folders(can_data_dir: str) -> list[str]:
    """
    From a specified directory, creates empty CanNode objects for each represented nodes (from folders)
    """
    return [f.name for f in os.scandir(can_data_dir) if f.is_dir()]
