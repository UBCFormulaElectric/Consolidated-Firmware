"""
commit_info.py
-----------------
Fetches the commit information for the commits for each ECU on the CAN bus.

"""

import argparse

import can
from github import Github, Commit


def get_commit_info(sha: str) -> Commit:
    """
    Fetches the commit information for a given commit SHA.
    :param sha: The given commit SHA.
    :return: Commit Object from the GitHub API.
    """
    g = Github()
    repo = g.get_repo("UBCFormulaElectric/Consolidated-Firmware")
    c = repo.get_commit(sha=sha)
    return c


bus_mapping: dict[int, str] = {
    409: "BMS",
    909: "CRIT",
    609: "FSM",
    709: "RSM",
    509: "VC",
    809: "DAM",
}

bus_found: dict[str, bool] = {
    "BMS": False,
    "CRIT": False,
    "dimos": False,
    "FSM": False,
    "RSM": False,
    "VC": False,
    "DAM": False,
    "VC_boot": False,
    "BMS_boot": False,
    "RSM_boot": False,
    "FSM_boot": False,
    "CRIT_boot": False,
}

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--bus", type=str, default="vector", help="python-can bus type")
    parser.add_argument(
        "--channel", type=str, default="0", help="python-can channel"
    )
    parser.add_argument("--bit_rate", type=int, default=1_000_000, help="CAN bus bit rate")
    args = parser.parse_args()

    with can.interface.Bus(
        interface=args.bus, channel=args.channel, bitrate=args.bit_rate, fd=True
    ) as bus:
        try:
            while not all(bus_found.values()):
                rx_message = bus.recv()
                if (
                    rx_message.arbitration_id in bus_mapping.keys()
                    and not bus_found[bus_mapping[rx_message.arbitration_id]]
                ):
                    message_board = bus_mapping[rx_message.arbitration_id]
                    commit_hex = "".join(
                        "{:02x}".format(x) for x in reversed(rx_message.data[0:4])
                    ).capitalize()[1:]
                    clean = (
                        "{:02x}".format(rx_message.data[-1]) == "01"
                        and "Clean"
                        or "Dirty"
                    )
                    bus_found[message_board] = True

                    commit = get_commit_info(commit_hex)

                    print()
                    print(
                        f'{message_board}: "{commit.commit.message}" by {commit.author.name}'
                    )
                    print(f"Date: {commit.commit.author.date}")
                    print(f"SHA: {commit.sha} ({clean})")
        except KeyboardInterrupt:
            print("Exiting...")
        bus.shutdown()
