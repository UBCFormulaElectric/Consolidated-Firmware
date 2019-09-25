#!/bin/bash
# ------------------------------------------------------------------------------
# This script is intended to run in the 'script' phase of a Travis CI build
# ------------------------------------------------------------------------------
# Import shared bash scripts
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source $SCRIPT_DIR/travis_shared.sh
source $SCRIPT_DIR/../shared_funcs.sh

# Currently supported boards
BOARD_NAMES=(
    FSM
    DCM
    PDM
)

if [ "$RUN_BUILD" = "true" ]; then
    # Build each project
    for BOARD_NAME in "${BOARD_NAMES[@]}"
    do
        travis_run "cmake boards/$BOARD_NAME/CMakeLists.txt && make -C boards/$BOARD_NAME"
    done
fi

if [ "$RUN_TESTS" = "true" ]; then
    :
fi

if [ "$RUN_FORMATTING_CHECKS" = "true" ]; then
    # Run clang-format recursively
    travis_run python clang-format/fix_formatting.py
    # Check if there is any difference
    CHANGED_FILES=(`git diff --name-only`)
    if [ "$CHANGED_FILES" ]; then
        echo ""
        echo_border
        echo "FAILED - Git diff was non-zero!"
        echo "Run clang_format/fix_formatting.py to format your code properly:"
        echo ""
        echo $CHANGED_FILES
        echo_border
        echo ""
        exit 1;
    else
        echo_border
        echo "PASSED - Code is correctly formatted!"
        echo_border
    fi
fi

echo_border
echo "Travis CI Script has finished successfully!"
echo_border
exit 0
