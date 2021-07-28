import os
import platform
import sys

# The path to the directory this python file is in
PYTHON_EXECUTABLE_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

def _isWindows():
    """
    Check if the operating system is Windows

    :return: True if using Windows, false otherwise
    """
    return os.name == 'nt'

def _isMac():
    """
    Check if the operating system is Mac OS

    :return: True if using Mac OS, false otherwise
    """
    return platform.system() == 'Darwin'
def _wrapStringInQuotes(str):
    """
    Wrap strings in double quotations so paths don't get misinterpreted by bash

    :return: Input string formatted with double quotations around it
    """
    return '"{}"'.format(str)

def runClangFormat():
    """
    Run the clang-format executable over all C/C++ related files inside src
    :return: Exit code 0 if success; any other number for an error code
    """
    # Construct the command line input
    CLANG_FORMAT_VERSION = "10.0"
    CLANG_FORMAT_BINARY = os.path.join(".", "clang-format-") + CLANG_FORMAT_VERSION
    CLANG_FORMAT_OPTIONS = " -i --style=file "

    # Format C/C++ files
    INCLUDE_FILE_EXTENSIONS = (
        '.c',
        '.cc',
        '.cpp',
        '.h',
        '.hpp',
        '.hh'
    )

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
        "Drivers",      # STM32CubeMX generated libraries
        "cmake-build-debug",
        "cmake-build-embedded",
        "cmake-build-gtest",
        "SEGGER_RTT",
        "fff",
        "TraceRecorder",
        "auto_generated",
        "profiler-cortex-m4",
        "cmake-build-x86",
        "list.h"
    ]

    # Print the current working directory since the paths are relative
    print("Current working directory: " + os.getcwd())

    # Append the requisite .exe file ending for Windows
    if _isWindows():
        CLANG_FORMAT_BINARY += ".exe"
    elif _isMac():
        CLANG_FORMAT_BINARY += "-mac"

    # Prepare path to recursive traverse
    SOURCE_DIR = os.path.join("..", "boards")

    # Recursively traverse through file tree and apply clang-format
    print("Apply clang-format to files under {}:".format(os.path.join(os.getcwd(), SOURCE_DIR)))
    for root, dirnames, filenames in os.walk(SOURCE_DIR):
        # Remove directories that we don't want to format from search list
        dirnames[:] = [d for d in dirnames if d not in EXCLUDE_DIRS]

        for filename in filenames:
            if filename.endswith(INCLUDE_FILE_EXTENSIONS) and \
               filename not in EXCLUDE_FILES:

                sourceFile = os.path.join(root, filename)

                if _isWindows():
                    command = _wrapStringInQuotes(CLANG_FORMAT_BINARY + CLANG_FORMAT_OPTIONS + sourceFile)
                else:
                    command = (CLANG_FORMAT_BINARY + CLANG_FORMAT_OPTIONS + _wrapStringInQuotes(sourceFile))

                print(command)
                exitCode = os.system(command)
                
                if exitCode != 0:
                    return exitCode

    return 0

if __name__ == '__main__':
    # Change into the directory this python file is in so we can use relative paths
    os.chdir(PYTHON_EXECUTABLE_DIRECTORY)

    if runClangFormat() != 0:
        print("ERROR: Clang-Format encountered issues!")
        sys.exit(1)
    else:
        print("SUCCESS: Clang-Format ran on all files!")
