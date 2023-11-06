import os.path
from typing import Tuple
from os import path, listdir


def path_is_folder_and_has_ioc_file(pathName: str):
    is_folder = not path.isfile(pathName)
    if not is_folder:
        return False
    has_ioc = (any(file.endswith(".ioc") for file in listdir(pathName))  # TODO remove when refactor is done.
               or any([file.endswith(".ioc") for file in listdir(path.join(pathName, "src/cubemx"))]))

    return has_ioc


def get_board_names() -> Tuple[str]:
    """
    Return a tuple of board names supported in our current build, whose STM code can be generated (only usage)
    """
    path_to_boards = path.join(path.dirname(__file__), '..', '..', 'firmware', 'thruna')
    path_to_dev_boards = path.join(path.dirname(__file__), '..', '..', 'firmware', 'dev')
    list_of_boards_subdirs = list(map(lambda x: os.path.join(path_to_boards, x), listdir(path_to_boards)))
    list_of_dev_subdirs = list(map(lambda x: os.path.join(path_to_dev_boards, x), listdir(path_to_dev_boards)))

    supported_board_names: tuple[str, ...] = tuple(filter(path_is_folder_and_has_ioc_file, list_of_boards_subdirs + list_of_dev_subdirs))
    
    # split the dir 
    supported_board_names = tuple(map(lambda p: p.split("/")[-1], supported_board_names))
     
    return supported_board_names


def print_space_delimited_board_names():
    """
    Print a space-delimited string which can be parsed as an array in bash
    """
    print(' '.join(get_board_names()))


if __name__ == '__main__':
    print_space_delimited_board_names()
