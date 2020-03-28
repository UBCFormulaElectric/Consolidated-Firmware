#!/bin/bash
# ------------------------------------------------------------------------------
# This script contains shared functions for environment setup scripts.
# ------------------------------------------------------------------------------
# Exit immediately if a command exits with a non-zero status because each step
set -e

# Echo an 80 character length border
echo_border() {
    echo "==============================================================================="
}

# Scripts that install binaries shall take the installation path as an argument
check_installation_path_argument() {
    num_arguments=$1
    if [ "$num_arguments" -ne 1 ]; then
        echo "Please provide the installation path as argument!"
        exit 2
    fi
}

get_valid_board_names() {
    echo $(pipenv run python -c 'from scripts.utilities.supported_boards import print_space_delimited_board_names; print_space_delimited_board_names()')
}
