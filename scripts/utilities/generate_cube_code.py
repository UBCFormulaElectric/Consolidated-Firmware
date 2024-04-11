"""
Generate a script that can be used by STM32CubeMX command line interface. And
generate STM32CubeMX code accordingy.
"""

import argparse
import os
import subprocess

from generate_md5_checksum import generate_md5_checksum

CUBE_SCRIPT = """\
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

# Generate code in the project directory
generate code {codegen_dir}

# Exit the program
exit
"""


def generate_cubemx_code(board, ioc, codegen_dir, cubemx):
    """
    Generate STM32CubeMX code

    @param board: Name of the board
    @param ioc: Path to .ioc file
    @param codegen_dir: Directory in which STM32CubeMX code is to be generated
    @param cubemx: Path to STM32CubeMX binary
    """
    # Generate output folders if they don't exist already
    cube_script_dir = os.path.join(codegen_dir, "auto_generated")
    if not os.path.exists(cube_script_dir):
        os.makedirs(cube_script_dir)

    # Generate a temporary STM32CubeMX configuration file
    cube_script = os.path.join(cube_script_dir, board + ".stm32cubemx.script")
    cube_script_f = open(cube_script, "w+")
    cube_script_f.write(CUBE_SCRIPT.format(ioc=ioc, codegen_dir=codegen_dir))
    cube_script_f.close()

    # Generate STM32CubeMX code
    cubemx_dir = os.path.dirname(cubemx)
    cubemx_bin = os.path.basename(cubemx)
    # proc = subprocess.Popen([f"{cubemx_dir}/{cubemx_bin}", "-q", cube_script_f.name])
    # Note: If the STM32CubeMX script encounters an exception (e.g. It can't
    # find the a valid script), the process may never exit so there is no status
    # code to check at all. Account for this by setting a time out.
    timeout_sec = 5 * 60
    print(
        "timeout started",
        " ".join([f"{cubemx_dir}/{cubemx_bin}", "-q", cube_script_f.name]),
    )
    retcode = subprocess.check_call(
        [f"{cubemx_dir}/{cubemx_bin}", "-q", cube_script_f.name],
        timeout=timeout_sec,
    )
    print("timeout done")
    # try:
    #     proc.wait(timeout_sec)
    # except subprocess.TimeoutExpired:
    #     raise Exception(
    #         "STM32CubeMX execution has timed out after {} seconds.".format(
    #             str(timeout_sec)
    #         )
    #     )
    if retcode != 0:
        raise Exception("An error occured while executing STM32CubeMX.")
    print("STM32CubeMX code generation completed successfully.")


# Generates the STM32CubeMX code for the board specified in the arguments and then clang-format the generated code
# WILL NOT REGENERATE if the .ioc file has not changed (i.e. the checksum is the same as the file)
if __name__ == "__main__":
    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("--board", help="The board to generate STM32CubeMX code for")
    parser.add_argument("--ioc", help="STM32CubeMX .ioc file")
    parser.add_argument("--md5", help="Location of the MD5 hash file of the .ioc file")
    parser.add_argument("--codegen_output_dir", help="Code generation output folder")
    parser.add_argument("--cube_bin", help="STM32CubeMX binary")
    args = parser.parse_args()

    # check the current checksum to see if it needs to be updated
    if os.path.isfile(f"{args.md5}"):
        with open(f"{args.md5}", "r") as f:
            current_checksum = f.read()
            # if the checksum of args.ioc is the same as the one in the .md5 file, then exit
            ioc_checksum = generate_md5_checksum(args.ioc)
            if current_checksum == ioc_checksum:
                print(f"{current_checksum} == {ioc_checksum}")
                print("No changes in the .ioc file. Code generation skipped.")
                exit(0)

    generate_cubemx_code(
        args.board,
        args.ioc,
        args.codegen_output_dir,
        args.cube_bin,
    )
    # generate the new checksum
    new_ioc_md5 = generate_md5_checksum(args.ioc)
    # Generate output folders if they don't exist already
    output_dir = os.path.dirname(args.codegen_output_dir)
    if not os.path.exists(output_dir):
        os.mkdir(output_dir)
    # Write MD5 checksum to disk
    with open(f"{args.md5}", "w") as f:
        f.write(new_ioc_md5)
        f.close()
