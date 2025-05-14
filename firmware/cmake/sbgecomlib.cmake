message("")
message("📚 SGB ECOM Library")
# Import sources files for SBG Systems library
set(SBG_ECOM_INCLUDE_DIRS
        "${SBG_ECOM_SOURCE_DIR}/common"
        "${SBG_ECOM_SOURCE_DIR}/src"
)
file(GLOB_RECURSE SBG_ECOM_SRCS
        "${SBG_ECOM_SOURCE_DIR}/src/*.c"
        "${SBG_ECOM_SOURCE_DIR}/common/crc/*.c"
        "${SBG_ECOM_SOURCE_DIR}/common/platform/*.c"
        "${SBG_ECOM_SOURCE_DIR}/common/interfaces/sbgInterface.c"
        "${SBG_ECOM_SOURCE_DIR}/common/sbgErrorCodes.c"
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

# support for newer compilers
target_compile_definitions("sbg_ecom_cm4" PRIVATE _POSIX_TIMERS)
target_compile_definitions("sbg_ecom_cm7" PRIVATE _POSIX_TIMERS)
