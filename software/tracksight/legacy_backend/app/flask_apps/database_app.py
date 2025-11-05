"""
This module contains the flask app for the database.
"""

import json
import os

from flask import request, jsonify, Blueprint

DB_JSON_PATH = "./dashboards.json"


# HELPERS
def read_json_file():
    """

    :return:
    """
    if not os.path.exists(DB_JSON_PATH):
        return {}  # TODO respond with some error
    with open(DB_JSON_PATH) as file:
        return json.load(file)


def write_json_file(data):
    """

    :param data:
    """
    with open(DB_JSON_PATH, "w") as file:
        json.dump(data, file, indent=4)


app = Blueprint("database_app", __name__)


@app.route("/get-data", methods=["GET"])
def get_data():
    """

    :return:
    """
    path = request.args.get("path")
    data = read_json_file()
    path_data = data.get(path, "No data found")
    return jsonify(path_data)


@app.route("/save-data", methods=["POST"])
def save_data():
    """

    :return:
    """
    content = request.json
    data_to_save = content["data"]
    data = read_json_file()
    # may want to add error handling when there is duplicate name?
    name = data_to_save["dbname"]
    data["dashboards"][name] = data_to_save
    write_json_file(data)
    return jsonify({"message": "Data saved successfully"})


@app.route("/delete-data", methods=["POST"])
def delete_data():
    """

    :return:
    """
    content = request.json
    path = content["path"]
    print(path)
    data = read_json_file()

    path_parts = path.split("/")
    if (
        len(path_parts) == 2
        and path_parts[0] == "dashboards"
        and path_parts[1] in data.get("dashboards", {})
    ):
        del data["dashboards"][path_parts[1]]
        write_json_file(data)
        return jsonify({"message": "Data deleted successfully"})
    else:
        return jsonify({"error": "Data not found"}), 404
