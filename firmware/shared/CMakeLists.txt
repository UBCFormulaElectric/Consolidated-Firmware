# Shared code that UBC Formula Electric wrote
file(GLOB_RECURSE SHARED_APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
file(GLOB_RECURSE SHARED_IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
file(GLOB_RECURSE SHARED_HW_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c")
set(SHARED_APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app")
set(SHARED_IO_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
)

set(SHARED_ARM_BINARY_X86_COMPATIBLE_SRCS
    ${SHARED_APP_SRCS})

set(X86_INCOMPATIBLE_IO_SRCS
    ${SHARED_IO_SRCS}
    ${SHARED_HW_SRCS}
)
set(SHARED_ARM_BINARY_X86_INCOMPATIBLE_SRCS ${X86_INCOMPATIBLE_IO_SRCS})

set(SHARED_ARM_BINARY_INCLUDE_DIRS
    ${SHARED_APP_INCLUDE_DIRS}
    ${SHARED_IO_INCLUDE_DIRS}
    ${LIST_H_INCLUDE_DIR})

# Expose the following variables to the parent scope (i.e. The scope of any
# other CMakeLists.txt that uses add_subdirectory() on this CMakeLists.txt).
set(SHARED_ARM_BINARY_X86_COMPATIBLE_SRCS
    ${SHARED_ARM_BINARY_X86_COMPATIBLE_SRCS}
    PARENT_SCOPE)
set(SHARED_ARM_BINARY_X86_INCOMPATIBLE_SRCS
    ${SHARED_ARM_BINARY_X86_INCOMPATIBLE_SRCS}
    PARENT_SCOPE)
set(SHARED_ARM_BINARY_INCLUDE_DIRS
    ${SHARED_ARM_BINARY_INCLUDE_DIRS}
    PARENT_SCOPE)

file(GLOB GOOGLETEST_TEST_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
)
list(REMOVE_ITEM GOOGLETEST_TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp")
set(GOOGLETEST_TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test")

# We use `create_arm_binary_or_tests_for_board` to generate App_CanMsgs.h, which
# is required by certain tests in SHARED_test. Since the shared library does not
# have an embedded target to compile for, we only call
# `create_arm_binary_or_tests_for_board` when the platform is x86. Any embedded-
# specific arguments (e.g. IOC_FILE) can be omitted by passing in an
# empty string.
if("${PLATFORM}" STREQUAL "x86")
    list(APPEND HDRS_TO_FAKE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_time.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_led.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_rgbLed.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_switch.h
    )
    create_fake_library(
        "shared_fakes"
        "${HDRS_TO_FAKE}"
    )

    create_arm_binary_or_tests_for_board(
        "shared"
        ""
        ""
        ""
        ""
        ""
        ""
        ""
        ""
        ""
        "${GOOGLETEST_TEST_SRCS}"
        "${GOOGLETEST_TEST_INCLUDE_DIRS}"
    )
endif()
