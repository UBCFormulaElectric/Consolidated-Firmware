import os.path
from os import path, listdir


def path_is_folder_and_has_ioc_file(pathName: str):
    is_folder = not path.isfile(pathName)
    if not is_folder:
        return False
    has_ioc = any(file.endswith(".ioc") for file in listdir(pathName)
                  or any(file.endswith(".ioc") for file in listdir(path.join(pathName, "src/cubemx"))))
    return has_ioc


def get_board_names() -> tuple[str]:
    """
    Return a tuple of board names supported in our current build, whose STM code can be generated (only usage)
    """
    path_to_boards = path.join(path.dirname(__file__), '..', '..', 'firmware', 'thruna')
    supported_board_names: tuple[str, ...] = tuple(
        filter(lambda x: path_is_folder_and_has_ioc_file(os.path.join(path_to_boards, x)), listdir(path_to_boards)))
    return supported_board_names


def print_space_delimited_board_names():
    """
    Print a space-delimited string which can be parsed as an array in bash
    """
    print(' '.join(get_board_names()))


if __name__ == '__main__':
    print(print_space_delimited_board_names())
