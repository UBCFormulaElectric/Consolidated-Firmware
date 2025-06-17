import os
import sqlite3

from flask import Blueprint

historical_api = Blueprint("historical", __name__)
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DB_PATH = os.path.join(BASE_DIR, "historical.db")
with sqlite3.connect(DB_PATH) as _hdb:
    # check if a table called files exists, if not create it
    _hdb.execute(
        "CREATE TABLE IF NOT EXISTS files (file_name TEXT, commit_sha TEXT, start_iso_time TEXT PRIMARY KEY, duration_iso TEXT)"
    )
    _hdb.execute(
        "CREATE UNIQUE INDEX IF NOT EXISTS file_name ON files(file_name);")
    _hdb.execute("pragma journal_mode=wal")
    _hdb.commit()


# Viewing Historical Data
@historical_api.route("/historical/list", methods=["GET"])
def list_historical_files():
    """
    List all dumped files in influx
    """
    with sqlite3.connect("historical.db") as historical_db:
        c = historical_db.execute("SELECT * FROM files")
        return c.fetchall()


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
    RETURNS the time/value series for a signal in a file. Note that it returns all the data for a given file for a given signal
    Ensure that this data is streamed
    """
    # hit influx db
    return []


@historical_api.route("/historical/<file_name>/delete", methods=["DELETE"])
def delete_historical_file(file_name: str):
    """
    Deletes a file from historical files
    """
    # remember to remove all cached files as well
    # remmeber to remove the signal values from the influx as well
    # remove file from db
    with sqlite3.connect("historical.db") as historical_db:
        try:
            historical_db.execute(
                "DELETE FROM files WHERE file_name = ?", file_name)
            historical_db.commit()
            return None, 200
        except Exception:
            return {"error": f"Failed to delete {file_name}"}, 500
