include(cmake/embedded.cmake)
include(cmake/tests.cmake)
include(cmake/shared.cmake)

if("${PLATFORM}" STREQUAL "arm")
    add_compile_definitions(TARGET_EMBEDDED)

    # STM32F4Cube firmware package: Contains STM32 HAL drivers and FreeRTOS with the CMSIS-RTOS v2 API.
    CPMAddPackage(
        NAME STM32CUBEF4
        GITHUB_REPOSITORY UBCFormulaElectric/STM32CubeF4
        GIT_TAG 3e907f8
    )

    # littlefs: Filesystem for microcontrollers
    CPMAddPackage(
        NAME LITTLEFS
        GITHUB_REPOSITORY littlefs-project/littlefs
        GIT_TAG v2.8.0
    )
    set(LITTLEFS_SRCS 
        "${LITTLEFS_SOURCE_DIR}/lfs.c"
        "${LITTLEFS_SOURCE_DIR}/lfs_util.c"
    )
    embedded_library(
        "littlefs_cm4"
        "${LITTLEFS_SRCS}"
        "${LITTLEFS_SOURCE_DIR}"
        "cm4"
        TRUE
    )
elseif("${PLATFORM}" STREQUAL "x86")
    add_compile_definitions(TARGET_TEST)
endif()

add_subdirectory(third_party)
add_subdirectory(shared)
add_subdirectory(thruna)