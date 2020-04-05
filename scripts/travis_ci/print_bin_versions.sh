#!/bin/bash
# -----------------------------------------------------------------------------
# This script prints out binary versions relevant to Travis CI
# -----------------------------------------------------------------------------
# Import shared bash scripts
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source $SCRIPT_DIR/travis_shared.sh

if [ "$RUN_ARM_BUILD" = "true" ] || [ "$RUN_X86_TESTS" = "true" ] || [ "$RUN_CUBE_CODEGEN_CHECKS" = "true" ]; then
  # CMake
  travis_run cmake --version
  # gcc-arm-none-eabi
  travis_run arm-none-eabi-gcc --version
  travis_run arm-none-eabi-objcopy --version
  travis_run arm-none-eabi-objdump --version
  travis_run arm-none-eabi-size --version
  travis_run arm-none-eabi-gcc-ar --version
  travis_run arm-none-eabi-gdb --version
  # STM32CubeMX doesn't have a --version option unfortunately, we can only
  # check if it is installed correctly
  travis_run which STM32CubeMX
fi
