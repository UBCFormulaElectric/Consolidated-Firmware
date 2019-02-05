import os

# The path to the directory this python file is in
PYTHON_EXECUTABLE_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

def runClangFormat():
    """
    Run the clang-format executable over all C/C++ related files inside src
    Return exit code 0 if success; any other number for an error code
    """
    
    # Construct the command line input
    CLANG_FORMAT_VERSION = "7.0"
    CLANG_FORMAT_COMMAND = "clang-format-" + CLANG_FORMAT_VERSION
    CLANG_FORMAT_OPTIONS = " -i --style=file "
    IGNORE_FILES = [
            "CanMsgs.c", 
            "CanMsgs.h"
            ]

    # If the OS is Windows
    if os.name == 'nt':
        # Append the requisite .exe file ending
        CLANG_FORMAT_COMMAND += ".exe" 

    SOURCE_DIR = os.path.join("..", "src")

    # Now iterate through all the files inside the src directory
    for root, dirnames, filenames in os.walk(SOURCE_DIR):
        for filename in filenames:
            # In particular, if it's a C/C++ source file and we're not explicitly ignoring it, run clang-format on it
            if filename.endswith(('.c', '.cc', '.cpp', '.h', '.hpp', '.hh')) \
                    and filename not in IGNORE_FILES:
                # Encapsulate the path to the clang-format in double quotations to 
                # account for spaces in directory names
                pathToClang = '"{}"'.format(os.path.abspath(CLANG_FORMAT_COMMAND))
                
                sourceFile = os.path.join(root, filename)
                if os.name == 'posix':
                    sourceFile = '"{}"'.format(sourceFile)
                    command = (pathToClang + CLANG_FORMAT_OPTIONS + sourceFile) 
                elif os.name == 'nt':
                    command = '"{}"'.format(pathToClang + CLANG_FORMAT_OPTIONS + sourceFile) 
                
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
        if runClangFormat() != 0:
            print("ERROR: Clang-Format encountered issues!")
            sys.exit(1)
        else:
            print("Clang-Format ran successfully on all files!")
