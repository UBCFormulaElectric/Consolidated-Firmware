# Shared code that UBC Formula Electric wrote
file(GLOB_RECURSE SHARED_APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Src/App/*.c")
file(GLOB_RECURSE SHARED_IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Src/Io/*.c")
set(SHARED_APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/App")
set(SHARED_IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/Io")

# Root directory of where the third-party code is stored
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# Import include directories and source files for Cortex M4 profiler
add_subdirectory(${THIRD_PARTY_DIR}/profiler-cortex-m4)
list(APPEND SHARED_IO_INCLUDE_DIRS ${PROFILER_INCLUDE_DIRS})
file(GLOB_RECURSE PROFILER_SRCS ${PROFILER_BASE_DIR}/*.c)
list(APPEND SHARED_IO_SRCS ${PROFILER_SRCS})

# Import include directories and source files for TraceRecorder
add_subdirectory(${THIRD_PARTY_DIR}/TraceRecorder)
list(APPEND SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_INCLUDE_DIRS ${TRACE_RECORDER_INCLUDE_DIRS})
file(GLOB_RECURSE TRACE_RECORDER_SRCS ${TRACE_RECORDER_BASE_DIR}/*.c)
list(APPEND SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_SRCS ${TRACE_RECORDER_SRCS})

# Import include directories and source files for SEGGER_RTT
add_subdirectory(${THIRD_PARTY_DIR}/SEGGER_RTT)
list(APPEND SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_INCLUDE_DIRS ${SEGGER_RTT_INCLUDE_DIRS})
file(GLOB_RECURSE SEGGER_RTT_SRCS ${SEGGER_RTT_BASE_DIR}/*.c)
list(APPEND SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_SRCS ${SEGGER_RTT_SRCS})

# Import include directory for fff.h
list(APPEND SHARED_GOOGLETEST_TEST_INCLUDE_DIRS ${THIRD_PARTY_DIR}/fff)

# Import source files for Newlib-FreeRTOS patch
file(GLOB_RECURSE NEWLIB_FREERTOS_PATCH_SRCS ${THIRD_PARTY_DIR}/newlib_freertos_patch/*.c)
list(APPEND SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_SRCS ${NEWLIB_FREERTOS_PATCH_SRCS})

# Import include directory for list.h
set(LIST_H_INCLUDE_DIR ${THIRD_PARTY_DIR}/list.h/src)

set(SHARED_ARM_BINARY_X86_COMPATIBLE_SRCS
        ${SHARED_APP_SRCS})

set(X86_INCOMPATIBLE_IO_SRCS "${SHARED_IO_SRCS}")
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
set(SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_SRCS
        ${SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_SRCS}
        PARENT_SCOPE)
set(SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_INCLUDE_DIRS
        ${SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_INCLUDE_DIRS}
        PARENT_SCOPE)
set(SHARED_GOOGLETEST_TEST_INCLUDE_DIRS
        ${SHARED_GOOGLETEST_TEST_INCLUDE_DIRS}
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
if ("${PLATFORM}" STREQUAL "x86")
    create_arm_binary_or_tests_for_board(
            "SHARED"
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
