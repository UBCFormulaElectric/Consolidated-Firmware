# Include directories
SET(PROFILER_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/profiler)
SET(PROFILER_SOURCE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/profiler)

# Make the two variables we just created visible in the parent scope, so that
# any cmake file that includes this one via `add_subdirectory` can see them
SET(PROFILER_INCLUDE_DIRS ${PROFILER_INCLUDE_DIRS} PARENT_SCOPE)
SET(PROFILER_SOURCE_DIRS ${PROFILER_SOURCE_DIRS} PARENT_SCOPE)
