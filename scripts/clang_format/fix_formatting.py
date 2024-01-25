import os
import sys
import multiprocessing


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
CLANG_FORMAT_BINARY = f'{os.path.join(".", "clang-format-")}{CLANG_FORMAT_VERSION}{CLANG_PLATFORM_SUFFIX}'
CLANG_FORMAT_OPTIONS = "-i --style=file"
CLANG_FORMAT_CMD = f"{CLANG_FORMAT_BINARY} {CLANG_FORMAT_OPTIONS}"

# TODO make this (configs and function) work with multiple directories/make it compatible with multiple directories

# Format C/C++ files
INCLUDE_FILE_EXTENSIONS = (".c", ".cc", ".cpp", ".h", ".hpp", ".hh")

# Files ignored by clang-format
EXCLUDE_FILES = [
    # Everytime we format these 2 files, we get an unwanted '\' at the end.
    # Ignore them from clang-format as a workaround.
    "stm32f3xx_hal_conf.h",
    "system_stm32f3xx.c",
]

# Directories ignored by clang-format
EXCLUDE_DIRS = [
    "Middlewares",  # STM32CubeMX generated libraries
    "Drivers",  # STM32CubeMX generated libraries
    "cmake-build-embedded",
    "cmake-build-gtest",
    "third_party",
]


def find_all_files() -> list[str]:
    """
    Find and return all files to run formatting on.

    """
    # Print the current working directory since the paths are relative
    print("Current working directory: " + os.getcwd())

    # Prepare path to recursive traverse
    SOURCE_DIR = os.path.join("..", "..", "firmware")

    # Recursively traverse through file tree and apply clang-format
    print(f"Apply clang-format to files under {os.path.join(os.getcwd(), SOURCE_DIR)}:")

    source_files = []
    for root, dirnames, filenames in os.walk(SOURCE_DIR):
        # Remove directories that we don't want to format from search list
        dirnames[:] = [d for d in dirnames if d not in EXCLUDE_DIRS]

        # Add all found files, filtering out any that should be excluded
        source_files += [
            f'"{os.path.join(root, file)}"'
            for file in filenames
            if file.endswith(INCLUDE_FILE_EXTENSIONS) and file not in EXCLUDE_FILES
        ]

    return source_files


def run_clang_format(source_file: str) -> bool:
    """
    Run clang format against a C/C++ source file, returning True if successful.
    """
    # Append the requisite .exe file ending for Windows
    # Construct and invoke clang-format
    if platform == "windows":
        command = f"\"{CLANG_FORMAT_CMD} {source_file}\""
    else:
        command = f"{CLANG_FORMAT_CMD} \"{source_file}\""

    try:
        return os.system(command) == 0
    except KeyboardInterrupt:
        return False


if __name__ == "__main__":
    # Change into the DIRECTORY OF THIS PYTHON FILE is in so we can use relative paths
    PYTHON_EXECUTABLE_DIRECTORY = os.path.dirname(os.path.abspath(__file__))
    os.chdir(PYTHON_EXECUTABLE_DIRECTORY)

    source_files = find_all_files()  # Find all valid files
    pool = multiprocessing.Pool()  # Start a multiprocessing pool to speed up formatting
    try:
        results = pool.map(run_clang_format, source_files)
        pool.close()
        success = all(results)
        for i, result in enumerate([result for result in results if not result]):
            print(f"Encountered an error running clang-format against {source_files[i]}")
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
