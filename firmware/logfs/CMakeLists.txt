message("")
message("⚙️ Configuring LogFS")

set(LOGFS_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
file(GLOB_RECURSE LOGFS_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

IF("${TARGET}" STREQUAL "binary")
    if(NOT "${EMBEDDED_CMAKE_INCLUDED}" STREQUAL "TRUE")
        message(FATAL_ERROR "❌ embedded.cmake must be included before logfs is subdirectoried")
    endif()
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
    
    set(LOGFS_SRC_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/python")
    IF(MSVC)
        set_target_properties(logfs_src PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${LOGFS_SRC_OUTPUT_DIR}"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${LOGFS_SRC_OUTPUT_DIR}"
        )
    ELSE()
        set_target_properties(logfs_src PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${LOGFS_SRC_OUTPUT_DIR}")
        target_compile_options(logfs_src PRIVATE 
            -O3
            -Wall
            -Werror
            -Wextra
            -pedantic
        )
    ENDIF()

    # Invoke setuptools to install the Python library.
    add_custom_target(install_python_logfs
        COMMAND ${PYTHON_COMMAND} -m pip install "${CMAKE_CURRENT_SOURCE_DIR}/python"
        DEPENDS logfs_src)
ENDIF()
