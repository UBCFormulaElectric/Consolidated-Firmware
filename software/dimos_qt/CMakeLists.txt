cmake_minimum_required(VERSION 3.22.3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF("${TARGET}" STREQUAL "deploy")
ELSEIF("${TARGET}" STREQUAL "dev")
ELSE()
    message(FATAL_ERROR "❌ Unknown target ${TARGET}")
ENDIF()
set(EXECUTABLE_NAME dimos)

add_subdirectory(src)