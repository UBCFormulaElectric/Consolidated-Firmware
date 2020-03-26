# Shared code that UBC Formula Electric wrote
file(GLOB_RECURSE SHARED_APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Src/App/*.c")
file(GLOB_RECURSE SHARED_IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Src/Io/*.c")
set(SHARED_APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/App")
set(SHARED_IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/Io")

# Root directory of where the third-party code is stored
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# Import include and source directories for Cortex M4 profiler
add_subdirectory(${THIRD_PARTY_DIR}/profiler-cortex-m4)
list(APPEND SHARED_IO_INCLUDE_DIRS ${PROFILER_INCLUDE_DIRS})
file(GLOB_RECURSE PROFILER_SRCS ${PROFILER_BASE_DIR}/*.c)
list(APPEND SHARED_IO_SRCS ${PROFILER_SRCS})

# Import include and source directories for TraceRecorder
add_subdirectory(${THIRD_PARTY_DIR}/TraceRecorder)
list(APPEND SHARED_IO_INCLUDE_DIRS ${TRACE_RECORDER_INCLUDE_DIRS})
file(GLOB_RECURSE TRACE_RECORDER_SRCS ${TRACE_RECORDER_BASE_DIR}/*.c)
list(APPEND SHARED_IO_SRCS ${TRACE_RECORDER_SRCS})

# Import include and source directories for SEGGER_RTT
add_subdirectory(${THIRD_PARTY_DIR}/SEGGER_RTT)
list(APPEND SHARED_IO_INCLUDE_DIRS ${SEGGER_RTT_INCLUDE_DIRS})
file(GLOB_RECURSE SEGGER_RTT_SRCS ${SEGGER_RTT_BASE_DIR}/*.c)
list(APPEND SHARED_IO_SRCS ${SEGGER_RTT_SRCS})

# Import include directory for fff.h
list(APPEND SHARED_APP_INCLUDE_DIRS ${THIRD_PARTY_DIR}/fff)

# Expose the following variables to the parent scope (i.e. The scope of any
# other CMakeLists.txt that uses add_subdirectory() on this CMakeLists.txt).
set(SHARED_APP_SRCS ${SHARED_APP_SRCS} PARENT_SCOPE)
set(SHARED_APP_INCLUDE_DIRS ${SHARED_APP_INCLUDE_DIRS} PARENT_SCOPE)
set(SHARED_IO_SRCS ${SHARED_IO_SRCS} PARENT_SCOPE)
set(SHARED_IO_INCLUDE_DIRS ${SHARED_IO_INCLUDE_DIRS} PARENT_SCOPE)
