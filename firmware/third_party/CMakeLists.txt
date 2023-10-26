include(../cmake/embedded.cmake)

if("${PLATFORM}" STREQUAL "arm")
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
    cm4_library(
        "sbg_ecom"
        "${SBG_ECOM_SRCS}"
        "${SBG_ECOM_INCLUDE_DIRS}"
        TRUE
    )
endif()
