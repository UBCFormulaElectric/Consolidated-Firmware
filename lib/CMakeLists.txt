# Each of the following scripts should build its own static library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SEGGER_RTT)

# This allows user to include headers as follows: #include lib/xxx/yyy.h
list(APPEND LIB_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../")

# Expose the following include directories to the parent scope so TraceRecorder
# can compile without errors
list(APPEND LIB_INCLUDE_DIRS ${SEGGER_RTT_INCLUDE_DIRS})

set(LIB_INCLUDE_DIRS ${LIB_INCLUDE_DIRS} PARENT_SCOPE)
