"""
Module for creating the can database for the VC telem data
"""

import glob
import json
import os

import can_database
from json_parsing.json_can_parsing import JsonCanParser


def load_json(file_path):
    # Load and ret data from JSON file
    with open(file_path, "r") as file:  # Open file in read mode
        return json.load(file)  # Parses JSON and loades into Python obj


def merge_data(data_list):
    # Merge data together to a dictionary, merges everything together
    merged_data = {}
    for (
        data
    ) in (
        data_list
    ):  # A list of dictionaries. Each dictionary contains separate JSON file. Goes thru each
        for key, value in data.items():  # Iterates key value pairs
            if key in merged_data:
                # Assuming the values are lists and can be extended
                merged_data[key].extend(value)
            else:
                merged_data[key] = value
    return merged_data


def save_json(data, file_path):
    # Save dictionary to JSON in new filecd
    with open(file_path, "w") as file:
        json.dump(data, file, indent=4)


def main():
    # Find path to can_bus jsons
    path_to_json = os.path.abspath("../../../../can_bus/quadruna")
    # Find all files that have .json ending
    file_pattern = os.path.join(path_to_json, "*.json")
    files = glob.glob(file_pattern)

    # Output file
    output_file = "../"


if __name__ == "__main__":
    main()
    # join paths together with abs where you are relative to can directory
    # use os and joins
    can_data = can_database()
    json_can_parser = JsonCanParser()
