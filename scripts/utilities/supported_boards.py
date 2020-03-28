def get_board_names():
    """
    Return a tuple of board names supported in our current build
    """
    SUPPORTED_BOARD_NAMES = (
        'FSM',
        'PDM',
        'BMS',
        'DCM',
        'DIM',
    )
    return SUPPORTED_BOARD_NAMES

def print_space_delimited_board_names():
    """
    Print a space-delimited string which can be parsed as an array in bash
    """
    print(' '.join(get_board_names()))
