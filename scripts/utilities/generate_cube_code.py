"""
Generate a script that can be used by STM32CubeMX command line interface. And
generate STM32CubeMX code accordingy.
"""
import sys
import os
import subprocess
import argparse

CUBE_SCRIPT = '''\
###############################################################################
# This file is auto-generated. DO NOT MODIFY!
#
# Note: The paths in this files are relative to the where the STM32CubeMX is
#       invoked, rather than relative to where this .script file is.
###############################################################################
# Load the project-specific config file (.ioc)
config load {ioc}

# Generate the peripheral initializations in main.c
project couplefilesbyip 0

# Generate code in the project direcotry
generate code {codegen_dir}

# Exit the program
exit
'''

def generate_cubemx_code(board, ioc, codegen_dir, cubemx):
    """
    Generate STM32CubeMX code

    @param board: Name of the board
    @param ioc: Path to .ioc file
    @param codegen_dir: Directory in which STM32CubeMX code is to be generated
    @param cubemx: Path to STM32CubeMX binary
    """
    # Generate output folder if they don't exist already
    cube_script_dir = os.path.join(codegen_dir, 'auto_generated')
    if not os.path.exists(cube_script_dir):
        os.mkdir(cube_script_dir)

    # Generate a temporary STM32CubeMX configuration file
    cube_script = os.path.join(cube_script_dir, board + '.stm32cubemx.script')
    cube_script_f = open(cube_script, 'w+')
    cube_script_f.write(CUBE_SCRIPT.format(ioc=ioc, codegen_dir=codegen_dir))
    cube_script_f.close()

    # Generate STM32CubeMX code
    proc = subprocess.Popen(['java', '-jar', cubemx, '-q', cube_script_f.name])
    # Note: If the STM32CubeMX script encounters an exception (e.g. It can't
    # find the a valid script), the process may never exit so there is no status
    # code to check at all. Account for this by setting a time out.
    timeout_sec = 120
    try:
        proc.wait(timeout_sec)
    except subprocess.TimeoutExpired:
        raise Exception('STM32CubeMX execution has timed out after {} seconds.'.format(str(timeout_sec)))
    if proc.returncode is not 0:
        raise Exception('An error occured while executing STM32CubeMX.')

if __name__ == '__main__':
    # Parse arguments
    parser = argparse.ArgumentParser()
    valid_boards = ['DCM', 'FSM', 'PDM', 'BMS']
    parser.add_argument('board', help='Choose one of the following: ' + ' '.join(valid_boards))
    parser.add_argument('ioc', help='STM32CubeMX .ioc file')
    parser.add_argument('output_dir', help='Code generation output folder')
    parser.add_argument('cube_bin', help='STM32CubeMX binary')
    args = parser.parse_args()
    if args.board not in valid_boards:
        print('Error: Invalid board name. Valid options: ' + ' '.join(valid_boards))
        sys.exit(1)

    generate_cubemx_code(args.board, args.ioc, args.output_dir, args.cube_bin)
