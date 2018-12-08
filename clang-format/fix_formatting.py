import os

def runClangFormat():

    """
    Run the clang-format executable over all C/C++ related files inside src
    Return exit code 0 if success; any other number for an error code
    """
    
    # Construct the command line input
    CLANG_FORMAT_VERSION = "7.0"
    CLANG_FORMAT_COMMAND = "clang-format-" + CLANG_FORMAT_VERSION
    CLANG_FORMAT_OPTIONS = " -i --style=file "

    # If the OS is Windows
    if os.name == 'nt':
        # Append the requisite .exe file ending
        CLANG_FORMAT_COMMAND += ".exe" 

    # Check that user is running from the clang-format directory
    SOURCE_DIR = os.path.join("..", "src")
    if not os.path.isdir(SOURCE_DIR):
        print("Must run check_formatting from inside clang-format directory!")
        return 1

    # Now iterate through all the files inside the src directory
    for root, dirnames, filenames in os.walk(SOURCE_DIR):
        for filename in filenames:
            # In particular, if it's a C/C++ source file, run clang-format on it
            if filename.endswith(('.c', '.cc', '.cpp', '.h', '.hpp', '.hh')):
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
