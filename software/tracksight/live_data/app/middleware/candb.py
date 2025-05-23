import datetime
import os
from concurrent.futures import ThreadPoolExecutor

import requests

# _dockerized = os.environ.get("DOCKERIZED") == "1"
# if not _dockerized:
#     import sys

#     sys.path.insert(
#         0,
#         os.path.abspath(
#             os.path.join(
#                 os.path.dirname(__file__), "../../../../../scripts/code_generation/"
#             )
#         ),
#     )


def _download_file(commit_sha, file, folder_path, save_dir):
    file_url = f"https://raw.githubusercontent.com/UBCFormulaElectric/Consolidated-Firmware/{commit_sha}/{file['path']}"
    file_path = os.path.join(save_dir, os.path.relpath(file["path"], folder_path))
    os.makedirs(os.path.dirname(file_path), exist_ok=True)
    with requests.get(file_url, stream=True) as file_response:
        file_response.raise_for_status()
        with open(file_path, "wb") as f:
            for chunk in file_response.iter_content(chunk_size=8192):
                f.write(chunk)
    print(f"Downloaded: {file['path']}")


_car = os.environ.get("CAR_NAME") or "quintuna"
bus_configs_path = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "../bus_configs", _car)
)
_cached_commit_sha: str | None = None


def fetch_jsoncan_configs(commit_sha: str, force=False) -> str:
    """
    Fetches the jsoncan configs for a given sha
    RETURNS: the path of the folder containing the jsoncan configs for car/ecu
    NOTE: this will
    NOTE: this determines which car you are driving with an environment variable.
    """
    global _cached_commit_sha
    save_dir = os.path.join(bus_configs_path, commit_sha)
    # if save_dir is present, assume that it's contents are valid
    if _cached_commit_sha == commit_sha and os.path.exists(save_dir) and not force:
        # this is an important optimization as this is the hot branch
        # namely, every time a commit info message comes in, this runs
        return

    folder_path = f"can_bus/{_car}"
    url = f"https://api.github.com/repos/UBCFormulaElectric/Consolidated-Firmware/git/trees/{commit_sha}?recursive=1"
    response = requests.get(url)
    response.raise_for_status()

    # Filter for files in the specified folder
    files: list[str] = [
        file
        for file in response.json().get("tree", [])
        if file["path"].startswith(folder_path) and file["type"] == "blob"
    ]

    # DISK MUTATIONS HERE AND BELOW
    if not os.path.exists(save_dir):
        os.makedirs(save_dir)
    with ThreadPoolExecutor(max_workers=46) as executor:
        executor.map(
            lambda file: _download_file(commit_sha, file, folder_path, save_dir), files
        )
    _cached_commit_sha = commit_sha
    return save_dir


# fetch_jsoncan_configs("e12121d", True)
from jsoncan import JsonCanParser

# can_db = CanDatabase()
from settings import CAR_NAME

json_can_config_root = os.path.join(
    os.path.dirname(os.path.abspath(__file__)),
    "..",
    "..",
    "..",
    "..",
    "..",
    "can_bus",
    CAR_NAME,
)

if not os.path.lexists(json_can_config_root):
    raise Exception("json can path does not exist, did you pass correct CAN_NAME")

live_can_db = JsonCanParser(json_can_config_root).make_database()
board_start_time: datetime.datetime = None


def update_can_db(path):
    global live_can_db
    live_can_db = JsonCanParser(path).make_database()


def update_base_time(time: datetime.datetime):
    global board_start_time
    board_start_time = time
