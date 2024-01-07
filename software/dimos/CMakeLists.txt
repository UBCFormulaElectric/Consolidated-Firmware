cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${TARGET}" STREQUAL "deploy")
    set(EXECUTABLE_NAME dimos)
elseif ("${TARGET}" STREQUAL "dev")
    set(EXECUTABLE_NAME dimos_dev)
else()
    message(FATAL_ERROR "❌ Unknown target ${TARGET}")
endif()

add_subdirectory(src)