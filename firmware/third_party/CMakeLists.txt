include(../cmake/embedded.cmake)

# Import littlefs
CPMAddPackage(
  NAME LITTLEFS
  GITHUB_REPOSITORY littlefs-project/littlefs
  GIT_TAG v2.8.0
)
# # Import sources files for littlefs
# list(APPEND SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_INCLUDE_DIRS ${LITTLEFS_SOURCE_DIR})
# file(GLOB_RECURSE LITTLEFS_SRC "${LITTLEFS_SOURCE_DIR}/lfs.c" "${LITTLEFS_SOURCE_DIR}/lfs_util.c" )
# list(APPEND SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_SRCS ${LITTLEFS_SRC})

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
>>>>>>> fd95e62c (actually works)
