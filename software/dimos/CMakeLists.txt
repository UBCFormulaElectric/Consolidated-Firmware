cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${TARGET}" STREQUAL "deploy")
elseif ("${TARGET}" STREQUAL "dev")
else ()
    message(FATAL_ERROR "❌ Unknown target ${TARGET}")
endif ()
set(EXECUTABLE_NAME dimos)

add_subdirectory(src)