#!/bin/bash
# ------------------------------------------------------------------------------
# This script prints out binary versions relevant to Travis CI
# ------------------------------------------------------------------------------
# Import shared bash scripts
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source $SCRIPT_DIR/travis_shared.sh
source $SCRIPT_DIR/../shared_funcs.sh

# Verify arguments
num_arguments=$#
if [ "$num_arguments" -ne 2 ]; then
    echo "Please provide the installation path and STM32CubeMX installation path as arguments!"
    exit 2
fi

if [ "$RUN_ARM_BUILD" = "true" ] || [ "$RUN_X86_TESTS" = "true" ] || [ "$RUN_CUBE_CODEGEN_CHECKS" = "true" ]; then
    INSTALL_DIR=$1
    CUBE_INSTALL_DIR=$2
    # Install binaries
    travis_run sudo scripts/environment_setup/install_cmake.sh $INSTALL_DIR
    travis_run sudo scripts/environment_setup/install_gcc_arm_none_eabi.sh $INSTALL_DIR
    travis_run pipenv run sudo python scripts/environment_setup/install_cube.py $CUBE_INSTALL_DIR ./tools/en.STM32CubeMX_v5-3-0.zip
fi
