from concurrent.futures import ThreadPoolExecutor
import os
import sys
import requests

_dockerized = os.environ.get("IN_DOCKER_CONTAINER") == "true"
if not _dockerized:
    sys.path.insert(
        0,
        os.path.abspath(
            os.path.join(
                os.path.dirname(__file__), "../../../../scripts/code_generation/"
            )
        ),
    )
# from jsoncan.src.json_parsing.json_can_parsing import JsonCanParser
from jsoncan.src.can_database import CanDatabase

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


bus_path = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "../bus_configs")
)
def fetch_jsoncan_configs(car: str, ecu: str, force = False) -> str:
    """
    Fetches the jsoncan configs for a given car and ecu.
    REQUIRES: ecu_commit to be populated correctly beforehand. If it is None it throws an error. If it is stale it continues with stale data
    RETURNS: the path of the folder containing the jsoncan configs for car/ecu
    """
    commit_sha = ecu_commit[ecu]
    if commit_sha is None:
        raise Exception("Make sure to set ecu_commit with the ECUs commit information before fetching")
    save_dir = os.path.join(bus_path, car, commit_sha, ecu)
    # if save_dir is present, assume that it's contents are valid
    if os.path.exists(save_dir) and not force:
        # this is an important optimization as this is the hot branch
        # namely, every time a commit info message comes in, this runs
        return

    folder_path = f"can_bus/{car}/{ecu}"
    url = f"https://api.github.com/repos/UBCFormulaElectric/Consolidated-Firmware/git/trees/{commit_sha}?recursive=1"
    response = requests.get(url)
    response.raise_for_status()

    # Filter for files in the specified folder
    files = [file for file in response.json().get("tree", []) if file["path"].startswith(folder_path) and file["type"] == "blob"]
    if not os.path.exists(save_dir):
        os.makedirs(save_dir)
    with ThreadPoolExecutor(max_workers=46) as executor:
        executor.map(lambda file: _download_file(commit_sha, file, folder_path, save_dir), files)
    return save_dir

canid_commitinfo: dict[int, str] = {
    406: "BMS",
    506: "VC",
    606: "FSM",
    706: "RSM",
    806: "DAM",
    906: "CRIT"
}

ecu_commit: dict[str, str | None] = {
    "BMS": None,
    "VC": None,
    "FSM": None,
    "RSM": None,
    "DAM": None,
    "CRIT": None
}

can_db = CanDatabase()