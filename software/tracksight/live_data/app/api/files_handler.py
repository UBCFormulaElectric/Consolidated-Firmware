"""
Assortment of API endpoints used to manage SD cards attached to the computer,
"""

import sqlite3

import psutil
from flask import Blueprint
from logfs import LogFs, LogFsDiskFactory
from logger import logger

sd_api = Blueprint('sd', __name__)


class NoSDCard(Exception):
    pass


@sd_api.errorhandler(NoSDCard)
def no_sd_card_error_handler(err):
    return {"error": "No such SD Card"}, 400


class TooManySDCards(Exception):
    pass


@sd_api.errorhandler(TooManySDCards)
def too_many_sd_card_error_handler(err):
    return {"error": "Many SD Cards Matched This Description"}, 500


def find_sd_card(sd_device: str):
    """
    Finds an SD card based on the device name
    Raises NoSDCard if no such device exists
    Raises TooManySDCards if multiple of such device exists (this should not happen)
    """

    candidate_partitions = list(filter(
        lambda d: d.device == sd_device and "removable" in d.opts.split(","), psutil.disk_partitions()))
    if len(candidate_partitions) != 1:
        if len(candidate_partitions) == 0:
            raise NoSDCard()
        else:
            logger.critical(
                f"Found multiple for {sd_device}, namely {candidate_partitions}")
            raise TooManySDCards()
    return candidate_partitions[0]


def create_logfs(mountpoint: str):
    return LogFs(
        block_size=512,
        block_count=1024 * 1024 * 15,
        disk=LogFsDiskFactory.create_disk(
            block_size=512,
            block_count=1024 * 1024 * 15,
            disk_path=mountpoint,
        ),
        rd_only=True,
    )


@sd_api.route("/sd/list", methods=["GET"])
def list_sd_cards():
    removable_cards = [i for i in psutil.disk_partitions(
    ) if "removable" in i.opts.split(",")]
    return [card.device for card in removable_cards]

# IMPORTANT NOTE: sd_device must be adequately escaped when placed in a url string (use %2F)


@sd_api.route("/sd/<sd_device>/files/list", methods=["GET"])
def list_files_in_card(sd_device: str):
    """
    Lists all files within an SD card's filesystem
    """
    sd_card = find_sd_card(sd_device)
    logfs = create_logfs(sd_card.mountpoint)
    file_names: list[str] = [file for file in logfs.list_dir() if file not in [
        "/bootcount.txt"]]
    return [{
        "name": file_name,
        "start_iso_time": "2025-01-06"
    } for file_name in file_names]


@sd_api.route("/sd/<sd_device>/files/<file_id>/dump", methods=["POST"])
def dump_file(sd_device: str, file_id: str):
    """
    Dumps a given file to the influx database (historical bucket)
    Streams progress
    """
    sd_card = find_sd_card(sd_device)
    logfs = create_logfs(sd_card.mountpoint)
    try:
        with logfs.open(file_id) as file:
            raw_data = file.read()
            metadata = file.read_metadata()
    except Exception:
        return {"error": "File Does Not Exist"}, 400

    # take each signal in this file and
    # 1. convert to mf4 and cache to disk
    # 2. add to influx database
    # make sure to in the process stream the progress

    # finally, add the file to the historical sqlite db
    with sqlite3.connect("historical.db") as historical_db:
        historical_db.execute("INSERT INTO files (file_name, commit_sha, start_iso_time, duration_iso) VALUES (:file_name, :commit_sha, :start_iso_time, :duration_iso)", {
            "file_name": "",
            "commit_sha": "",
            "start_iso_time": "",
            "duration_iso": ""
        })
        historical_db.commit()

    return "", 200


@sd_api.route("/sd/<sd_device>/files/<file_id>/get", methods=["GET"])
def download_file(sd_device: str, file_id: str):
    """
    Sends the user the file requested
    """
    sd_card = find_sd_card(sd_device)
    logfs = create_logfs(sd_card.mountpoint)
    try:
        with logfs.open(file_id) as file:
            raw_data = file.read()
            return ""  # return a file somehow, preferablly streamed
    except Exception:
        return {"error": "File Does Not Exist"}, 400
