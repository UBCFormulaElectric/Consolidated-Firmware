# Include directories
SET(CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config)
SET(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(STREAMPORTS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/streamports/Jlink_RTT/include)
SET(TRACE_RECORDER_INCLUDE_DIRS
        ${CONFIG_DIR}
        ${INCLUDE_DIR}
        ${STREAMPORTS_INCLUDE_DIR}
        )

# Base directory of TraceRecorder
SET(TRACE_RECORDER_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Expose the following variables to the parent scope (i.e. The scope of any
# other CMakeLists.txt that uses add_subdirectory() on this CMakeLists.txt).
SET(TRACE_RECORDER_INCLUDE_DIRS ${TRACE_RECORDER_INCLUDE_DIRS} PARENT_SCOPE)
SET(TRACE_RECORDER_BASE_DIR ${TRACE_RECORDER_BASE_DIR} PARENT_SCOPE)
