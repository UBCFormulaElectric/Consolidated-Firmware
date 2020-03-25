function(build_all_tests
            APP_SRCS
            INCLUDE_DIRS
        )
    build_state_machine_test("${APP_SRCS}" "${INCLUDE_DIRS}")
endfunction()

function(build_state_machine_test
            APP_SRCS
            INCLUDE_DIRS
        )
    add_gtest_binary(
            ${BOARD_NAME}_state_machine_tests
            state_machine
            ${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/main.cpp
            "${INCLUDE_DIRS}"
            ${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/state_machine_tests.cpp
            "${APP_SRCS}"
    )
endfunction()