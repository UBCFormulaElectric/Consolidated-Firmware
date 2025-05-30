message("")
message("⚙️ Configuring Shared")

IF ("${TARGET}" STREQUAL "test")
    # shared modules tests
    file(GLOB_RECURSE SHARED_TEST_SRCS
            "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
    )
    set(SRCS ${SHARED_TEST_SRCS} ${SHARED_APP_SRCS})
    set(INCLUDE_DIRS ${SHARED_APP_INCLUDE_DIR} ${FFF_DIR} ${SHARED_TEST_UTILS_INCLUDE_DIRS})
    compile_gtest_executable("shared_test" "${SRCS}" "${INCLUDE_DIRS}")

    # jsoncan lib
    jsoncan_library("Debug" "quintuna" "${CMAKE_CURRENT_BINARY_DIR}")

    # io time :)
    set(HDRS_TO_FAKE
            "${SHARED_IO_INCLUDE_DIR}/io_time.h"
            "${SHARED_IO_INCLUDE_DIR}/io_led.h"
            "${SHARED_IO_INCLUDE_DIR}/io_switch.h"
            "${SHARED_IO_INCLUDE_DIR}/io_mechanicalLoad.h"
    )
    create_fake_library("shared_test_fakes" "${HDRS_TO_FAKE}")

    target_compile_definitions("shared_test" PRIVATE STM32H733xx)
    target_link_libraries("shared_test" PRIVATE "quintuna_Debug_jsoncan" "shared_test_fakes")
ENDIF ()