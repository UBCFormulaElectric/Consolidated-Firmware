set(SEGGER_RTT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(SEGGER_RTT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

# Expose the following variables to the parent scope (i.e. The scope of any
# other CMakeLists.txt that uses add_subdirectory() on this CMakeLists.txt).
set(SEGGER_RTT_INCLUDE_DIRS ${SEGGER_RTT_INCLUDE_DIRS} PARENT_SCOPE)
set(SEGGER_RTT_BASE_DIR ${SEGGER_RTT_BASE_DIR} PARENT_SCOPE)

