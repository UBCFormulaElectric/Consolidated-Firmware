import os
import sys

# The path to the directory this python file is in
PYTHON_EXECUTABLE_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

def _isWindows():
    """
    Check if the operating system is Windows

    :return: True if using Windows, false otherwise
    """
    return os.name == 'nt'

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
    CLANG_FORMAT_VERSION = "7.0"
    CLANG_FORMAT_BINARY = "./clang-format-" + CLANG_FORMAT_VERSION
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

    # Ignore auto-generated files
    EXCLUDE_FILES = [
        "CanMsgs.c",
        "CanMsgs.h"
    ]

    # Ignore auto-generated directories
    EXCLUDE_DIRS = [
        "Middleswares", # STM32CubeMX generated libraries
        "Drivers",      # STM32CubeMX generated libraries
        "cmake-build-debug",
        "CMakeFiles",
        "stm32f3xx_hal_conf.h",
    ]

    # Print the current working directory since the paths are relative
    print("Current working directory: " + os.getcwd())

    # Append the requisite .exe file ending for Windows
    if _isWindows():
        CLANG_FORMAT_BINARY += ".exe"

    # Prepare ${REPO_ROOT}/src directory
    SOURCE_DIR = os.path.join("..", "src")

    # Now iterate through all the files inside the ${REPO_ROOT}/src directory
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

    continueScript = input("THIS WILL FORMAT YOUR CODE! CONTINUE (Y | N)? ")
    if continueScript != 'Y' and continueScript != 'y':
        sys.exit(2)
    else:
        # Print newline because `echo "Y"` in Travis CI doesn't include LF
        print("\r")
        if runClangFormat() != 0:
            print("ERROR: Clang-Format encountered issues!")
            sys.exit(1)
        else:
            print("SUCCESS: Clang-Format ran on all files!")
