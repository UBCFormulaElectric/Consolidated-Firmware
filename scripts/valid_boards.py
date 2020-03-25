"""
Return a tuple of board names supported in our current build
"""
def get_valid_board_names():
    VALID_BOARD_NAMES = (
        'FSM',
        'PDM',
        'BMS',
        'DCM',
        'DIM'
    )
    return VALID_BOARD_NAMES

if __name__ == '__main__':
    # Print a space-delimited string which can be parsed as an array in bash
    print(' '.join(get_valid_board_names()))
