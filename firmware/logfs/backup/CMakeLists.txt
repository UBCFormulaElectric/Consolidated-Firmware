include(../cmake/embedded.cmake)

if("${TARGET}" STREQUAL "deploy")
    set(LOGFS_INCLUDE_DIRS 
        "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    file(GLOB_RECURSE LOGFS_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    )
    embedded_library(
        "logfs_cm4"
        "${LOGFS_SRCS}"
        "${LOGFS_INCLUDE_DIRS}"
        "cm4"
        TRUE
    )
elseif("${TARGET}" STREQUAL "test")
    find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
    pybind11_add_module(logfs python/bindings.cpp)
    # install(TARGETS logfs LIBRARY DESTINATION ${Python_SITEARCH})

    execute_process(
        COMMAND python -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())"
        OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    install(TARGETS logfs LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES})
endif()
