# Macro to serach for subdirectories within a directory.  This macro is not
# recursive, traverses only one level deep, and filters out non-directory
# files. If we had used
#
# FILE(GLOB SHARED_INCLUDE_DIRS LIST_DIRECTORIES false CONFIGURE_DEPENDS
#         ${CMAKE_CURRENT_SOURCE_DIR}/*
# 
# we would have globbed non-directory files (such as CMakeLists.txt) that are
# not valid include directories.
MACRO(SUBDIRLIST result top_dir)
    FILE(GLOB children ${top_dir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${child})
          LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

# Find all the include directories for shared libraries
SUBDIRLIST(SHARED_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Find all the source files for shared libraries
file(GLOB SHARED_SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp
        )

# Import include and source directories for Cortex M4 profiler
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/profiler-cortex-m4)
LIST(APPEND SHARED_INCLUDE_DIRS ${PROFILER_INCLUDE_DIRS})
file(GLOB PROFILER_SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS
        ${PROFILER_SOURCE_DIRS}/*.c)

# Import include and source directories for TraceRecorder
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/TraceRecorder)
LIST(APPEND SHARED_INCLUDE_DIRS ${TRACE_RECORDER_INCLUDE_DIRS})
file(GLOB TRACE_RECORDER_SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS
        ${TRACE_RECORDER_SOURCE_DIRS}/*.c)

# Make the two variables we just created visible in the parent scope, so that
# any cmake file that includes this one via `add_subdirectory` can see them
set(SHARED_INCLUDE_DIRS ${SHARED_INCLUDE_DIRS} PARENT_SCOPE)
set(SHARED_SRCS ${SHARED_SRCS} ${TRACE_RECORDER_SRCS} ${PROFILER_SRCS} PARENT_SCOPE)
