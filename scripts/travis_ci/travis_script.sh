#!/bin/bash
# ------------------------------------------------------------------------------
# This script is intended to run in the 'script' phase of a Travis CI build
# ------------------------------------------------------------------------------
# Import shared bash scripts
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_ROOT="$SCRIPT_DIR/../.."
source $SCRIPT_DIR/travis_shared.sh

# Currently supported boards
BOARD_NAMES=(
    FSM
    DCM
    PDM
    BMS
)

if [ "$RUN_BUILD" = "true" ]; then
    # Extract CubeIDE
    cd $PROJECT_ROOT
    mkdir cube_ide
    cd cube_ide
    travis_run ./../tools/st-stm32cubeide_1.2.0_5034_20200108_0926_amd64.sh --noexec 
    travis_run tar -zxf st-stm32cubeide_1.2.0_5034_20200108_0926_amd64.sh.root/st-stm32cubeide_1.2.0_5034_20200108_0926_amd64.tar.gz
    cd $PROJECT_ROOT

    # Build each project
    for BOARD_NAME in "${BOARD_NAMES[@]}"
    do
        ./cube_ide/stm32cubeide --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data workspace -import boards/$BOARD_NAME -cleanBuild $BOARD_NAME
    done
fi

if [ "$RUN_TESTS" = "true" ]; then
    :
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

# TODO: re-add, probably through cubeIDE
#if [ "$RUN_CUBE_CODEGEN_CHECKS" = "true" ]; then
#    # Use .ioc to update STM32CubeMX auto-generated code
#    for BOARD_NAME in "${BOARD_NAMES[@]}"
#      do travis_run python scripts/utilities/generate_cube_code.py $BOARD_NAME boards/$BOARD_NAME/$BOARD_NAME.ioc boards/$BOARD_NAME $STM32CUBEMX_BIN_PATH
#    done
#    # Auto-generated STM32CubeMX code doesn't conform to our clang-format so we
#    # need to manually format them
#    travis_run python clang-format/fix_formatting.py
#    # Check if there is any difference
#    GIT_DIFF_RESULT="$(git --no-pager diff --color)"
#    if [ "$GIT_DIFF_RESULT" ]; then
#        echo ""
#        echo_border
#        echo "FAILED - Git diff was non-zero!"
#        echo "STM32CubeMX auto-generated code is not up-to-date with .ioc "
#        echo ""
#        echo "$GIT_DIFF_RESULT"
#        echo_border
#        echo ""
#        exit 1;
#    else
#        echo_border
#        echo "PASSED - STM32CubeMX auto-generated code is up-to-date with .ioc"
#        echo_border
#    fi
#fi

echo_border
echo "Travis CI Script has finished successfully!"
echo_border
exit 0
