import argparse
from os import getcwd
import os
import subprocess


CMAKE_PATH_VAR_NAME = 'CMAKE'
OPENOCD_PATH_VAR_NAME = 'OPENOCD'

# CMAKE_PATH="C:/Program Files/JetBrains/CLion 2019.3.6/bin/cmake/win/bin/cmake.exe"
# BUILD_PATH="C:/Users/gtaha/Desktop/Consolidated-Firmware/boards/cmake-build-embedded"


if __name__ == '__main__':
    # Get directory to boards folder, which is just the working dir of this script
    boards_dir = getcwd()
    top_dir = f'{boards_dir}/..' # Main Consolidated-Firmware dir is one level up

    # Load CMake and OpenOCD from path vars
    cmake_dir = os.environ[CMAKE_PATH_VAR_NAME]
    cmake_path = f'{cmake_dir}/cmake.exe'
    open_ocd_dir = os.environ[OPENOCD_PATH_VAR_NAME]
    open_ocd_path = f'{open_ocd_dir}/openocd.exe'

    # Make command line interface
    parser = argparse.ArgumentParser(description='Build UBC Formula Electric firmware from command line.')
    parser.add_argument('--boards', help='names of boards to buld')
    parser.add_argument('--build', help='build specified boards', action='store_true')
    parser.add_argument('--rebuild', help='clean, then build boards', action='store_true')
    parser.add_argument('--clean', help='clean boards', action='store_true')
    parser.add_argument('--flash', help='flash board', action='store_true') # TODO
    parser.add_argument('--test', help='flash board', action='store_true') # TODO
    args = parser.parse_args()

    # Clean if requested
    if args.clean or args.rebuild:
        print('Starting clean...')
        process = subprocess.call([
            cmake_path,
            '--build',
            f'{boards_dir}/cmake-build-embedded',
            '--target',
            'clean',
        ])
        print('Clean finished.')

    if args.boards and (args.build or args.rebuild or args.flash): 
        boards = args.boards.split(',')

        # Build each specified board
        for board in boards:
            print(f'Building {board}...')
            process = subprocess.call([
                cmake_path, 
                '--build',
                f'{boards_dir}/cmake-build-embedded',
                '--target',
                f'{board}.elf',
            ])
            print(f'Finished building {board}.')

        # Flash only the first specified board, since can only flash one board at once
        if args.flash:
            board_to_flash = boards[0]
            print(f'Flashing {board_to_flash}') 
            process = subprocess.call([
                open_ocd_path,
                '-c',
                'tcl_port disabled',
                '-s',
                f'{open_ocd_path}/../share/openocd/scripts', # untested
                '-c',
                'gdb_port 3333', 
                '-c',
                'telnet_port 4444',
                '-f',
                f'{top_dir}/tools/OpenOCD/stm32f3x.cfg',
                '-c',
                f'program /"{boards_dir}/cmake-build-embedded/{board_to_flash}/{board_to_flash}.elf/"',
                '-c',
                'reset',
                '-c',
                'shutdown',
            ])
            print(f'Finished flashing {board_to_flash}.')