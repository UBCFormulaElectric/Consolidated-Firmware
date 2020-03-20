#!/bin/bash
# ------------------------------------------------------------------------------
# This script is intended to run in the 'script' phase of a Travis CI build
# ------------------------------------------------------------------------------
# Import shared bash scripts
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source $SCRIPT_DIR/travis_shared.sh
source $SCRIPT_DIR/../shared_funcs.sh

# The user must specify the STM32CubeMX binary pth
if [ "$#" -ne 1 ]; then
    echo "Please provide the STM32CubeMX binary path as argument!"
    exit 2
else
    STM32CUBEMX_BIN_PATH=$1
fi

# Currently supported boards
BOARD_NAMES=(
    FSM
    DCM
    PDM
    BMS
)

if [ "$RUN_BUILD" = "true" ]; then
    travis_run cmake -S boards -B boards/build -DPLATFORM=arm
    for BOARD_NAME in "${BOARD_NAMES[@]}"
    do
        travis_run make --directory=boards/build $BOARD_NAME.elf
    done
fi

if [ "$RUN_TESTS" = "true" ]; then
    travis_run cmake -S boards -B boards/build -DPLATFORM=x86
    travis_run make --directory=boards/build
fi

if [ "$RUN_FORMATTING_CHECKS" = "true" ]; then
    # Run clang-format recursively
    travis_run python clang-format/fix_formatting.py
    # Check if there is any difference
    GIT_DIFF_RESULT="$(git --no-pager diff --color)"
    if [ "$GIT_DIFF_RESULT" ]; then
        echo ""
        echo_border
        echo "FAILED - Git diff was non-zero!"
        echo "Run clang_format/fix_formatting.py to format your code properly:"
        echo ""
        echo "$GIT_DIFF_RESULT"
        echo_border
        echo ""
        exit 1;
    else
        echo_border
        echo "PASSED - Code is correctly formatted!"
        echo_border
    fi
fi

if [ "$RUN_CUBE_CODEGEN_CHECKS" = "true" ]; then
    # Use .ioc to update STM32CubeMX auto-generated code
    for BOARD_NAME in "${BOARD_NAMES[@]}"
      do travis_run python scripts/utilities/generate_cube_code.py --board $BOARD_NAME --log4j_properties_output ~/.stm32cubemx/log4j.properties --ioc boards/$BOARD_NAME/$BOARD_NAME.ioc --codegen_output_dir boards/$BOARD_NAME --cube_bin $STM32CUBEMX_BIN_PATH
    done
    # Auto-generated STM32CubeMX code doesn't conform to our clang-format so we
    # need to manually format them
    travis_run python clang-format/fix_formatting.py
    # Check if there is any difference
    GIT_DIFF_RESULT="$(git --no-pager diff --color)"
    if [ "$GIT_DIFF_RESULT" ]; then
        echo ""
        echo_border
        echo "FAILED - Git diff was non-zero!"
        echo "STM32CubeMX auto-generated code is not up-to-date with .ioc "
        echo ""
        echo "$GIT_DIFF_RESULT"
        echo_border
        echo ""
        exit 1;
    else
        echo_border
        echo "PASSED - STM32CubeMX auto-generated code is up-to-date with .ioc"
        echo_border
    fi
fi

echo_border
echo "Travis CI Script has finished successfully!"
echo_border
exit 0
