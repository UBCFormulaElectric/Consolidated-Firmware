set(SEGGER_RTT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(SEGGER_RTT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

# Make the two variables we just created visible in the parent scope, so that
# any cmake file that includes this one via `add_subdirectory` can see them
set(SEGGER_RTT_INCLUDE_DIRS ${SEGGER_RTT_INCLUDE_DIRS} PARENT_SCOPE)
set(SEGGER_RTT_BASE_DIR ${SEGGER_RTT_BASE_DIR} PARENT_SCOPE)

