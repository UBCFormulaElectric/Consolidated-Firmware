#!/bin/bash
# ------------------------------------------------------------------------------
# This script contains shared functions for environment setup scripts.
# ------------------------------------------------------------------------------
# Save the parent dir of this so we can always run commands relative to the
# location of this script, no matter where it is called from. This
# helps prevent bugs and odd behaviour if this script is run through a symlink
# or from a different directory.
CURR_DIR=$(dirname -- "$(readlink -f -- "$BASH_SOURCE")")

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
    echo $(python $CURR_DIR/valid_boards.py)
}
