include(../cmake/embedded.cmake)

set(LOGFS_INCLUDE_DIRS 
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
file(GLOB_RECURSE LOGFS_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

if("${TARGET}" STREQUAL "deploy")
    embedded_library(
        "logfs_cm4"
        "${LOGFS_SRCS}"
        "${LOGFS_INCLUDE_DIRS}"
        "cm4"
        TRUE
    )
elseif("${TARGET}" STREQUAL "test")
    find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

    # Create pybind11 module.
    pybind11_add_module(
        logfs_lib
        ${LOGFS_SRCS} 
        "${CMAKE_CURRENT_SOURCE_DIR}/python/bindings.cpp"
    )
    target_include_directories(logfs_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
    set_target_properties(logfs_lib PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/python"
    )

    # Invoke setuptools to install the Python library.
    # TODO: Make this invocation portable.
    add_custom_target(install_python_logfs
        COMMAND 
        LOGFS_LIB="logfs_lib.cpython-311-darwin.so" pip3 install .
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/python"
        DEPENDS logfs_lib)
endif()
