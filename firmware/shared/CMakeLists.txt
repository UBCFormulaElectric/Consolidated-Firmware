# Shared code that UBC Formula Electric wrote
file(GLOB_RECURSE SHARED_APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/App/*.c")
file(GLOB_RECURSE SHARED_IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Io/*.c")
file(GLOB_RECURSE SHARED_HW_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c")
list(APPEND SHARED_IO_SRCS "${SHARED_HW_SRCS}")
set(SHARED_APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/App")
set(SHARED_IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/Io")
set(SHARED_HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw")

set(SHARED_ARM_BINARY_X86_COMPATIBLE_SRCS
    ${SHARED_APP_SRCS})

set(X86_INCOMPATIBLE_IO_SRCS "${SHARED_IO_SRCS}")
set(SHARED_ARM_BINARY_X86_INCOMPATIBLE_SRCS ${X86_INCOMPATIBLE_IO_SRCS})

set(SHARED_ARM_BINARY_INCLUDE_DIRS
    ${SHARED_APP_INCLUDE_DIRS}
    ${SHARED_IO_INCLUDE_DIRS}
    ${SHARED_HW_INCLUDE_DIRS}
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
    "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/*.cpp"
)
list(REMOVE_ITEM GOOGLETEST_TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/main.cpp")
set(GOOGLETEST_TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc")

# We use `create_arm_binary_or_tests_for_board` to generate App_CanMsgs.h, which
# is required by certain tests in SHARED_test. Since the shared library does not
# have an embedded target to compile for, we only call
# `create_arm_binary_or_tests_for_board` when the platform is x86. Any embedded-
# specific arguments (e.g. IOC_FILE) can be omitted by passing in an
# empty string.
if("${PLATFORM}" STREQUAL "x86")
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
        "${GOOGLETEST_TEST_SRCS}"
        "${GOOGLETEST_TEST_INCLUDE_DIRS}"
    )
endif()
