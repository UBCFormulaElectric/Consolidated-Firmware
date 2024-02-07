if("${TARGET}" STREQUAL "test")
    list(APPEND HDRS_TO_FAKE 
        "${SHARED_IO_INCLUDE_DIR}/io_time.h"
        "${SHARED_IO_INCLUDE_DIR}/io_buzzer.h"
        "${SHARED_IO_INCLUDE_DIR}/io_led.h"
        "${SHARED_IO_INCLUDE_DIR}/io_rgbLed.h"
        "${SHARED_IO_INCLUDE_DIR}/io_switch.h"
    )

    create_fake_library(
        "thruna_shared_fakes"
        "${HDRS_TO_FAKE}"
    )

    jsoncan_library(
        "thruna_shared_can"
        "shared"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
        "thruna"
    )

    set(CAN_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/app)
    add_library("thruna_shared_can" STATIC "${CAN_SRCS}")
    target_include_directories("thruna_shared_can" PUBLIC "${CAN_INCLUDE_DIRS}")

    file(GLOB_RECURSE TEST_SRCS 
        "${SHARED_THRUNA_DIR}/test/*.c"
        "${SHARED_THRUNA_DIR}/test/*.cpp"
    )
    set(TEST_INCLUDE_DIR "${SHARED_THRUNA_DIR}/test")

    file(GLOB_RECURSE SHARED_APP_THRUNA_SRCS
        "${SHARED_THRUNA_DIR}/src/app/*.c"
    )

    set(SRCS ${TEST_SRCS} ${SHARED_APP_SRCS} ${SHARED_APP_THRUNA_SRCS})
    set(INCLUDE_DIRS ${TEST_INCLUDE_DIR} ${SHARED_APP_INCLUDE_DIR} ${SHARED_APP_INCLUDE_THRUNA_DIR} ${FFF_DIR} ${SHARED_TEST_UTILS_INCLUDE_DIRS})
    compile_gtest_executable(
        "thruna_shared_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("thruna_shared_test" "thruna_shared_can" "thruna_shared_fakes")
endif()