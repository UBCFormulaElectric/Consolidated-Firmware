import os
import subprocess
from typing import LiteralString
import re
import argparse

OBJECT_FILE_EXTENSION = ".obj" if os.name == "nt" else ".o"

RED = "\033[91m"
GREEN = "\033[92m"
RESET = "\033[0m"


def find_obj_files(folder_path: str) -> list[LiteralString | str | bytes]:
    """
    Finds all files with a .obj extension within a specified folder and its subfolders.

    Args:
        folder_path (str): The path to the directory to search.

    Returns:
        list[str]: A list of full paths to all found .obj files.
                   Returns an empty list if the folder does not exist or no .obj files are found.
    """
    if not os.path.isdir(folder_path):
        raise ValueError("Folder not found at '{folder_path}'")
    # os.walk generates the file names in a directory tree by walking the tree
    # top-down or bottom-up. For each directory in the tree rooted at directory top,
    # it yields a 3-tuple (dirpath, dirnames, filenames).
    return [
        os.path.join(dirpath, filename)
        for dirpath, dirnames, filenames in os.walk(folder_path)
        for filename in filenames
        if filename.lower().endswith(OBJECT_FILE_EXTENSION)
    ]


LINE_COVERAGE_THRESHOLD = 0.8
BRANCH_COVERAGE_THRESHOLD = 0.8

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Calculate code coverage using gcov.")
    parser.add_argument(
        "--gcov_cmd",
        type=str,
        default="gcov",
        help='Path to the gcov command (default: "gcov")',
    )
    parser.add_argument(
        "--folder_path",
        type=str,
        required=True,
        help="Path to the folder containing .obj files",
    )
    args = parser.parse_args()

    gcov_cmd = args.gcov_cmd
    folder_path = args.folder_path

    obj_files = find_obj_files(folder_path)

    total_lines, total_lines_hit = 0, 0
    total_branches, total_branches_hit = 0, 0

    for obj_file in obj_files:
        obj_dir, obj_filename = os.path.split(obj_file)

        gcov_command = [gcov_cmd, "-b", "-c", "-n", obj_filename]
        process = subprocess.run(
            gcov_command, cwd=obj_dir, capture_output=True, text=True, check=True
        )

        if process.returncode != 0:
            raise Exception("gcov errors:\n", process.stderr)

        lines = process.stdout
        line_coverage, line_count, branch_coverage, branch_count = (
            None,
            None,
            None,
            None,
        )
        line_match = re.search(r"Lines executed:(\d+\.\d+)% of (\d+)", lines)
        if line_match:
            line_coverage = float(line_match.group(1))
            line_count = int(line_match.group(2))

        branch_match = re.search(r"Branches executed:(\d+\.\d+)% of (\d+)", lines)
        if branch_match:
            branch_coverage = float(branch_match.group(1))
            branch_count = int(branch_match.group(2))

        if line_coverage is not None and line_count is not None:
            total_lines += line_count
            total_lines_hit += round(line_coverage * line_count / 100)
        if branch_coverage is not None and branch_count is not None:
            total_branches += branch_count
            total_branches_hit += round(branch_coverage * branch_count / 100)

    if total_lines != 0:
        print(
            f"Overall line coverage: {total_lines_hit / total_lines * 100:.2f}% ({total_lines_hit}/{total_lines})"
        )
    else:
        print("Overall line coverage: N/A (no lines)")

    if total_branches != 0:
        print(
            f"Overall branch coverage: {total_branches_hit / total_branches * 100:.2f}% ({total_branches_hit}/{total_branches})"
        )
    else:
        print("Overall branch coverage: N/A (no branches)")

    error = False
    if total_lines != 0 and total_lines_hit / total_lines < LINE_COVERAGE_THRESHOLD:
        print(
            f"{RED}Line coverage below {LINE_COVERAGE_THRESHOLD * 100:.2f}% threshold{RESET}"
        )
        error = True
    if (
        total_branches != 0
        and total_branches_hit / total_branches < BRANCH_COVERAGE_THRESHOLD
    ):
        print(
            f"{RED}Branch coverage below {BRANCH_COVERAGE_THRESHOLD * 100:.2f}% threshold{RESET}"
        )
        error = True
    if error:
        # raise Exception("Coverage thresholds not met. Please check the coverage report.")
        print("Not enforcing code coverage for now...")
    else:
        print(f"{GREEN}Coverage thresholds met. Good job!{RESET}")
