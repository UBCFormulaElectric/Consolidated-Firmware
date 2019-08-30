#!/bin/bash
# ------------------------------------------------------------------------------
# This script prints out binary versions relevant to Travis CI
# ------------------------------------------------------------------------------
# Import shared bash scripts
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source $SCRIPT_DIR/travis_shared.sh
source $SCRIPT_DIR/../shared_funcs.sh

check_installation_path_argument $#

if [ "$RUN_BUILD" = "true" ] || [ "$RUN_TESTS" = "true" ]; then
  # Prepend bin path to $PATH in order to override the built-in CMake
  INSTALL_DIR=$1
  PATH=$INSTALL_DIR/bin:$PATH
  # Install binaries
  travis_run sudo scripts/environment_setup/install_cmake.sh $INSTALL_DIR
  travis_run sudo scripts/environment_setup/install_gcc_arm_none_eabi.sh $INSTALL_DIR
fi
