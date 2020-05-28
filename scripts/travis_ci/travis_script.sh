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

if [ "$RUN_ARM_BUILD" = "true" ]; then
    BUILD_DIR=boards/arm_build
    travis_run cmake -S boards -B $BUILD_DIR -DPLATFORM=arm
    for BOARD in $(get_valid_board_names)
    do
        travis_run make --directory=$BUILD_DIR $BOARD.elf
        # Upload the text/data/bss size to SeriesCI. For pull-request builds,
        # we use ${TRAVIS_PULL_REQUEST_SHA} to get the branch commit SHA.
        if [ "$TRAVIS_PULL_REQUEST" = "false" ]; then
            # Travis CI Branch build
            SHA=${TRAVIS_COMMIT}
        else
            # Travis CI Pull Request build
            SHA=${TRAVIS_PULL_REQUEST_SHA}
        fi
        # For now, the maximum RAM and FLASH size are the same for every board.
        MAX_RAM_SIZE=40960 
        MAX_FLASH_SIZE=262144
        travis_run ./scripts/travis_ci/series_ci.sh $BOARD $BUILD_DIR/$BOARD/$BOARD.elf $SHA $MAX_RAM_SIZE $MAX_FLASH_SIZE
    done
fi

if [ "$RUN_X86_TESTS" = "true" ]; then
    # The heartbeat monitor logic is different in Release and Debug build type,
    # so we run the tests twice.
    RELEASE_BUILD_DIR=boards/x86_build_release
    travis_run cmake -S boards -B $RELEASE_BUILD_DIR -DPLATFORM=x86 -DCMAKE_BUILD_TYPE=Release
    travis_run make --directory=$RELEASE_BUILD_DIR
    travis_run "cd $RELEASE_BUILD_DIR && ctest -T memcheck --verbose && cd -"
    CTEST_RETURN_CODE=$?

    if [ $CTEST_RETURN_CODE -ne 0 ]; then
        echo "Ctest (Release Build) failed. Please check the test log for more information."
        exit 1
    fi

    DEBUG_BUILD_DIR=boards/x86_build_debug
    travis_run cmake -S boards -B $DEBUG_BUILD_DIR -DPLATFORM=x86 -DCMAKE_BUILD_TYPE=Debug
    travis_run make --directory=$DEBUG_BUILD_DIR
    travis_run "cd $DEBUG_BUILD_DIR && ctest -T memcheck --verbose && cd -"
    CTEST_RETURN_CODE=$?

    if [ $CTEST_RETURN_CODE -ne 0 ]; then
        echo "Ctest (Debug Build) failed. Please check the test log for more information."
        exit 1
    fi
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
    for BOARD in $(get_valid_board_names)
    do travis_run python \
      scripts/utilities/generate_cube_code.py \
        --board $BOARD \
        --log4j_properties_output ~/.stm32cubemx/log4j.properties \
        --ioc boards/$BOARD/$BOARD.ioc \
        --codegen_output_dir boards/$BOARD \
        --cube_bin $STM32CUBEMX_BIN_PATH
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
