IF("${TARGET}" STREQUAL "binary")
    # Import sources files for SBG Systems library
    set(SBG_ECOM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sbgECom")
    set(SBG_ECOM_INCLUDE_DIRS 
        "${SBG_ECOM_DIR}/common"
        "${SBG_ECOM_DIR}/src"
    )
    file(GLOB_RECURSE SBG_ECOM_SRCS
        "${SBG_ECOM_DIR}/src/*.c"
        "${SBG_ECOM_DIR}/common/crc/*.c"
        "${SBG_ECOM_DIR}/common/platform/*.c"
        "${SBG_ECOM_DIR}/common/interfaces/sbgInterface.c"
    )
    embedded_library(
        "sbg_ecom_cm4"
        "${SBG_ECOM_SRCS}"
        "${SBG_ECOM_INCLUDE_DIRS}"
        "cm4"
        TRUE
    )
    embedded_library(
        "sbg_ecom_cm7"
        "${SBG_ECOM_SRCS}"
        "${SBG_ECOM_INCLUDE_DIRS}"
        "cm7"
        TRUE
    )
ELSE()
    add_subdirectory("pybind11")
ENDIF()



