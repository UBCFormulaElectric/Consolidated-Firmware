# Force the user to specify headers as "{App|Io}/<header>"
set(SHARED_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/inc")

# Find all the source files for shared libraries
file(GLOB_RECURSE SHARED_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# Root directory of where the third-party code is stored
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# Import include and source directories for Cortex M4 profiler
add_subdirectory(${THIRD_PARTY_DIR}/profiler-cortex-m4)
list(APPEND SHARED_INCLUDE_DIRS ${PROFILER_INCLUDE_DIRS})
file(GLOB_RECURSE PROFILER_SRCS ${PROFILER_BASE_DIR}/*.c)

# Import include and source directories for TraceRecorder
add_subdirectory(${THIRD_PARTY_DIR}/TraceRecorder)
list(APPEND SHARED_INCLUDE_DIRS ${TRACE_RECORDER_INCLUDE_DIRS})
file(GLOB_RECURSE TRACE_RECORDER_SRCS ${TRACE_RECORDER_BASE_DIR}/*.c)

# Import include and source directories for SEGGER_RTT
add_subdirectory(${THIRD_PARTY_DIR}/SEGGER_RTT)
list(APPEND SHARED_INCLUDE_DIRS ${SEGGER_RTT_INCLUDE_DIRS})
file(GLOB_RECURSE SEGGER_RTT_SRCS ${SEGGER_RTT_BASE_DIR}/*.c)

# Make the two variables we just created visible in the parent scope, so that
# any cmake file that includes this one via `add_subdirectory` can see them
set(SHARED_INCLUDE_DIRS
        ${SHARED_INCLUDE_DIRS}
        PARENT_SCOPE)
set(SHARED_SRCS
        ${SHARED_SRCS}
        ${PROFILER_SRCS}
        ${TRACE_RECORDER_SRCS}
        ${SEGGER_RTT_SRCS}
        PARENT_SCOPE)
