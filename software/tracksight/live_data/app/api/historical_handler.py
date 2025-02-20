from flask import Blueprint

historical_api = Blueprint("historical", __name__)

# Viewing Historical Data
@historical_api.route("/historical/list", methods=["GET"])
def list_historical_files():
    """
    List all dumped files in influx
    """
    return []

@historical_api.route("/historical/<file_name>/signals", methods=["GET"])
def list_signals(file_name: str):
    """
    Returns a list of all the signals that are present in a given file
    """
    # get commit info associated with the file_name
    # create a jsoncan database with the commit info
    # return messages in the jsoncan database
    # note: consider having a unified way of presenting signals from candb
    return []

@historical_api.route("/historical/<file_name>/<signal_name>", methods=["GET"])
def get_historical_signal(file_name: str, signal_name: str):
    """
    Queries Influx Database for a particular signal_name
    RETURNS the time/value series for a signal in a file
    """
    return []

@historical_api.route("/historical/<file_name>/delete", methods=["DELETE"])
def delete_historical_file(file_name: str):
    return None, 200
