import os.path
from os import path, listdir


def pathIsFolderAndHasIOCFile(pathName: str):
    isFolder = not path.isfile(pathName)
    if not isFolder:
        return False
    hasIOC = any(i.endswith(".ioc") for i in listdir(pathName)) or any(
        [i.endswith(".ioc") for i in listdir(path.join(pathName, "src/cubemx"))])
    return hasIOC


def get_board_names() -> tuple[str]:
    """
    Return a tuple of board names supported in our current build, whose STM code can be generated (only usage)
    """
    pathToBoards = path.join(path.dirname(__file__), '..', '..', 'firmware', 'thruna')
    SUPPORTED_BOARD_NAMES: tuple[str, ...] = tuple(
        filter(lambda x: pathIsFolderAndHasIOCFile(os.path.join(pathToBoards, x)), listdir(pathToBoards)))
    return SUPPORTED_BOARD_NAMES


def print_space_delimited_board_names():
    """
    Print a space-delimited string which can be parsed as an array in bash
    """
    print(' '.join(get_board_names()))


if __name__ == '__main__':
    print(get_board_names())
