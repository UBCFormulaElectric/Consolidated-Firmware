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
    find_package(pybind11 REQUIRED)

    # Create pybind11 module.
    pybind11_add_module(
        # ${LOGFS_SRCS} 
        "${CMAKE_CURRENT_SOURCE_DIR}/python/bindings.cpp"
    )
    
    # # Invoke setuptools to install the Python library.
    # add_custom_target(install_logfs_python
    #     COMMAND 
    #     LOGFS_BINDINGS_DIR="${CMAKE_CURRENT_BINARY_DIR}/lib" ${PYTHON_RAW_COMMAND} -m pip install "${CMAKE_CURRENT_SOURCE_DIR}/python")
endif()
