include(../cmake/embedded.cmake)

set(LOGFS_INCLUDE_DIRS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
file(GLOB_RECURSE LOGFS_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

if("${TARGET}" STREQUAL "deploy")
    embedded_library(
        "logfs_cm7"
        "${LOGFS_SRCS}"
        "${LOGFS_INCLUDE_DIRS}"
        "cm7"
        TRUE
    )
    target_compile_options(logfs_cm7 PRIVATE 
        -O3
    )
elseif("${TARGET}" STREQUAL "test")
    find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
    find_package(pybind11 CONFIG)

    # Create pybind11 module.
    pybind11_add_module(logfs_src ${LOGFS_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/python/bindings.cpp)
    target_include_directories(logfs_src PUBLIC "${LOGFS_INCLUDE_DIRS}")
    set_target_properties(logfs_src PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/python")
    target_compile_options(logfs_src PRIVATE 
        -O3
        -Wall
        -Werror
        -Wextra
        -pedantic
    )

    # Invoke setuptools to install the Python library.
    add_custom_target(install_python_logfs
        COMMAND ${PYTHON_COMMAND} -m pip install "${CMAKE_CURRENT_SOURCE_DIR}/python"
        DEPENDS logfs_src)
endif()
