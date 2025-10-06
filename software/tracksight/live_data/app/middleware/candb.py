import os
from concurrent.futures import ThreadPoolExecutor
import requests
from time import time

from jsoncan import JsonCanParser, CanDatabase
from settings import CAR_NAME
from logger import logger

def _download_file(commit_sha, file, folder_path, save_dir):
    file_url = f"https://raw.githubusercontent.com/UBCFormulaElectric/Consolidated-Firmware/{commit_sha}/{file['path']}"
    file_path = os.path.join(
        save_dir, os.path.relpath(file["path"], folder_path))
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
        return save_dir

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
            lambda file: _download_file(
                commit_sha, file, folder_path, save_dir), files
        )
    _cached_commit_sha = commit_sha
    return save_dir

def make_can_db(jsoncan_config_path: str) -> CanDatabase:
    start = time()
    out = JsonCanParser(jsoncan_config_path).make_database()
    logger.info(f"Loaded CAN database in {time() - start:.2f} seconds from {jsoncan_config_path}")
    return out

# fetch_jsoncan_configs("e12121d", True)
# can_db = CanDatabase()

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
    raise Exception(
        "json can path does not exist, did you pass correct CAN_NAME")

live_can_db = make_can_db(json_can_config_root)
# board_start_time: datetime.datetime = None


def update_can_db(path):
    global live_can_db
    live_can_db = make_can_db(path)


# def update_base_time(time: datetime.datetime):
#     global board_start_time
#     board_start_time = time
