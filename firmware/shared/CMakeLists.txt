if("${PLATFORM}" STREQUAL "x86")
    list(APPEND HDRS_TO_FAKE 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_time.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_led.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_rgbLed.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_switch.h"
    )
    set(SHARED_FAKES_INCLUDE_DIRS "${SHARED_APP_INCLUDE_DIRS}")
    create_fake_library(
        "shared_fakes"
        "${HDRS_TO_FAKE}"
        "${SHARED_FAKES_INCLUDE_DIRS}"
    )

    jsoncan_library(
        "shared_can"
        "asd"
        "${CMAKE_CURRENT_BINARY_DIR}"
        ""
    )

    file(GLOB_RECURSE SHARED_TEST_SRCS 
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
    )
    set(SHARED_TEST_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/test"
    )
    set(SRCS ${SHARED_TEST_SRCS} ${SHARED_APP_SRCS})
    set(INCLUDE_DIRS ${SHARED_TEST_INCLUDE_DIRS} ${SHARED_APP_INCLUDE_DIRS} ${FFF_DIR} ${SHARED_TEST_UTILS_INCLUDE_DIRS})
    compile_gtest_executable(
        "shared_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("shared_test" "shared_can" "shared_fakes")
endif()