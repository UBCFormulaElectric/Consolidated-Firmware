import os

# The path to the directory this python file is in
PYTHON_EXECUTABLE_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

# Format C/C++ files
INCLUDE_FILE_EXTENSIONS = (".c", ".cc", ".cpp", ".h", ".hpp", ".hh")


def refactor() -> bool:
    # Print the current working directory since the paths are relative
    print("Current working directory: " + os.getcwd())

    # Prepare path to recursive traverse
    SOURCE_DIR = os.path.join("boards", "BMS")

    print(f"Refactor all files under {os.path.join(os.getcwd(), SOURCE_DIR)}:")

    for root, _, filenames in os.walk(SOURCE_DIR):
        for file in filenames:
            file_path = os.path.join(root, file)
            if file.endswith(INCLUDE_FILE_EXTENSIONS) and "App" in file_path:
                file_name = file.split(".")[0]
                module = file_name.split("_")[1]
                print(file_name)
                print(module)
                return

                # Rename file.
                new_file_path = file_path.replace("hello", "world")
                if file_path != new_file_path:
                    os.rename(file_path, new_file_path)

                return
                # Replace occurences.
                with open(new_file_path, "r") as f:
                    content = f.read()

                new_content = content.replace("hello", "world")

                with open(new_file_path, "w") as f:
                    f.write(new_content)


if __name__ == "__main__":
    # Change into the directory this python file is in so we can use relative paths
    os.chdir(PYTHON_EXECUTABLE_DIRECTORY)

    refactor()
