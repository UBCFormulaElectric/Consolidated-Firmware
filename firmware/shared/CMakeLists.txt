message("")
message("⚙️ Configuring Shared")

IF ("${TARGET}" STREQUAL "test")
    # C tests (test/*.c, test/*.cpp) and C++ tests (test/cpp/*.cpp) targeted individually via layout
    file(GLOB SHARED_TEST_C_SRCS
            "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
    )
    file(GLOB SHARED_TEST_CPP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/cpp/*.cpp")
    # Only C++ app sources that do not depend on C++ io (e.g. io_time.hpp) to avoid undefined refs in shared_test
    set(SHARED_APP_SRCS_CPP_TEST "${SHARED_APP_INCLUDE_DIR_CPP}/app_sensor_filter.cpp")
    set(SRCS ${SHARED_TEST_C_SRCS} ${SHARED_TEST_CPP_SRCS} ${SHARED_APP_SRCS} ${SHARED_APP_SRCS_CPP_TEST})
    set(INCLUDE_DIRS ${SHARED_APP_INCLUDE_DIR} ${SHARED_APP_INCLUDE_DIR_CPP} ${SHARED_IO_INCLUDE_DIR_CPP} ${FFF_DIR} ${SHARED_TEST_UTILS_INCLUDE_DIRS})
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