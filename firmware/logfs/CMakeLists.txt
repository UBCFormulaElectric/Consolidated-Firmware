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
endif()
