import argparse
import functools
import multiprocessing
import os
import subprocess
import sys


def _get_platform() -> str:
    """
    Get platform this script is running on.
    """
    if sys.platform.startswith("win"):
        return "windows"
    elif sys.platform.startswith("darwin"):
        return "mac"
    else:
        return "linux"


platform = _get_platform()

# Construct the command line input
CLANG_FORMAT_VERSION = "17.0.6"
CLANG_PLATFORM_SUFFIX = {"windows": ".exe", "mac": "-mac", "linux": ""}[platform]
CLANG_FORMAT_BINARY = f'{os.path.join("..", "clang_format", "clang-format-")}{CLANG_FORMAT_VERSION}{CLANG_PLATFORM_SUFFIX}'
CLANG_FORMAT_OPTIONS = " -i --style=file"

# TODO make this (configs and function) work with multiple directories/make it compatible with multiple directories

# Format C/C++ files
INCLUDE_FILE_EXTENSIONS = (".c", ".cc", ".cpp", ".h", ".hpp", ".hh")


def find_all_files(
        path_from_root: str, EXCLUDE_FILES: list[str], EXCLUDE_DIRS: list[str]
) -> list[str]:
    """
    Find and return all files to run formatting on.

    """
    # Print the current working directory since the paths are relative

    # Prepare path to recursive traverse
    SOURCE_DIR = os.path.join("..", "..", path_from_root)

    # Recursively traverse through file tree and apply clang-format
    print(f"Searching for files under {os.path.join(os.getcwd(), SOURCE_DIR)}:")

    source_files = []
    for root, dirnames, filenames in os.walk(SOURCE_DIR):
        # Remove directories that we don't want to format from search list
        dirnames[:] = [d for d in dirnames if d not in EXCLUDE_DIRS]

        # Add all found files, filtering out any that should be excluded
        source_files += [
            f'{os.path.join(root, file)}'
            for file in filenames
            if file.endswith(INCLUDE_FILE_EXTENSIONS) and file not in EXCLUDE_FILES
        ]

    print(f"Found {len(source_files)} files to format.")
    return source_files

def run_clang_format(source_files: list[str], base_cmd: list[str]) -> bool:
    """
    Run clang format against a list of C/C++ source files, returning True if successful.
    Note: `source_files` needs to be first because pool.map passes the iterable
    """
    try:
        return subprocess.run(base_cmd + source_files).returncode == 0
    except KeyboardInterrupt:
        return False

def fix_formatting(local: str | None):
    # Change into the DIRECTORY OF THIS PYTHON FILE is in so we can use relative paths
    PYTHON_EXECUTABLE_DIRECTORY = os.path.dirname(os.path.abspath(__file__))
    os.chdir(PYTHON_EXECUTABLE_DIRECTORY)
    print("Current working directory: " + os.getcwd())

    # Find all valid files
    source_files = []
    source_files += find_all_files(
        "firmware",
        [
            # Everytime we format these 2 files, we get an unwanted '\' at the end.
            # Ignore them from clang-format as a workaround.
            "stm32f3xx_hal_conf.h",
            "system_stm32f3xx.c",
        ],
        [
            "Middlewares",  # STM32CubeMX generated libraries
            "Drivers",  # STM32CubeMX generated libraries
            "cmake-build-embedded",
            "cmake-build-gtest",
            "third_party",
        ],
    )

    # Build clang-format command.
    executable_name = local if local is not None else CLANG_FORMAT_BINARY
    n = 400
    pool = multiprocessing.Pool()  # Start a multiprocessing pool to speed up formatting
    try:
        results = pool.imap(
            functools.partial(run_clang_format, base_cmd=[executable_name, "-i", "--style=file"]),
            [source_files[i: i+n] for i in range(0, len(source_files), n)])
        pool.close()
        success = all(results)
        for i, result in enumerate([result for result in results if not result]):
            print(
                f"Encountered an error running clang-format against {source_files[i]}"
            )
    except KeyboardInterrupt:
        print("Interruption Detected")
        pool.terminate()
        success = False
    finally:
        pool.join()

    if success:
        print("SUCCESS: clang-format ran on all files!")
    else:
        print("ERROR: clang-format encountered issues!")
        sys.exit(1)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--local", "-l", type=str, required=False, default=None)
    args = parser.parse_args()
    fix_formatting(args.local)
