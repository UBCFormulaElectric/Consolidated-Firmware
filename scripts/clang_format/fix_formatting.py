import os
import sys
import multiprocessing

# The path to the directory this python file is in
PYTHON_EXECUTABLE_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

# Construct the command line input
CLANG_FORMAT_VERSION = "17.0.1"
CLANG_FORMAT_BINARY = os.path.join(".", "clang-format-") + CLANG_FORMAT_VERSION
CLANG_FORMAT_OPTIONS = " -i --style=file "

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
    platform = get_platform()
    clang_format_binary = (
            CLANG_FORMAT_BINARY + {"windows": ".exe", "mac": "-mac", "linux": ""}[platform]
    )
    clang_format_cmd = clang_format_binary + CLANG_FORMAT_OPTIONS

    # Construct and invoke clang-format
    if platform == "windows":
        command = wrap_in_quotes(clang_format_cmd + source_file)
    else:
        command = clang_format_cmd + wrap_in_quotes(source_file)

    try:
        return os.system(command) == 0
    except KeyboardInterrupt:
        return False


def get_platform() -> str:
    """
    Get platform this script is running on.
    """
    if sys.platform.startswith("win"):
        return "windows"
    elif sys.platform.startswith("darwin"):
        return "mac"
    else:
        return "linux"


def wrap_in_quotes(s: str) -> str:
    """
    Wrap strings in double quotations so paths don't get misinterpreted by bash.
    """
    return f'"{s}"'


if __name__ == "__main__":
    # Change into the directory this python file is in so we can use relative paths
    os.chdir(PYTHON_EXECUTABLE_DIRECTORY)

    try:
        # Find all valid files
        source_files = find_all_files()
        # Start a multiprocessing pool to speed up formatting
        pool = multiprocessing.Pool()
        results = pool.map(run_clang_format, source_files)
        pool.close()
        success = all(results)
        for i, result in enumerate([result for result in results if not result]):
            print(f"Encountered an error running clang-format against {source_files[i]}")
    except KeyboardInterrupt:
        pool.terminate()
        success = False
        print("Interrupted by user")
    finally:
        pool.join()

    if success:
        print("SUCCESS: clang-format ran on all files!")
    else:
        print("ERROR: clang-format encountered issues!")
