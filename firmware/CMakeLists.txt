include(cmake/embedded.cmake)
include(cmake/tests.cmake)
include(cmake/shared.cmake)

# ======== Libraries ========
if("${TARGET}" STREQUAL "arm")
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
elseif("${TARGET}" STREQUAL "x86")
    add_compile_definitions(TARGET_TEST)

    # We assume this cmake is running on a x86 TARGET, so the
    # *default compiler* settings should be fine

    # If compiling on x86, fetch GoogleTest for unit testing.
    CPMAddPackage(GITHUB_REPOSITORY "google/googletest" GIT_TAG "release-1.12.1")
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # For ctest to return a non-zero code when Valgrind fails, we must use
    # --error-exitcode. On the other hand, --leak-check=full ensures that any
    # memory leak issue will cause ctest to return a non-zero code.
    #
    # This must be set before include(CTest) is called, or ELSE
    # MEMORYCHECK_COMMAND_OPTIONS won't be overwritten correctly!
    set(MEMORYCHECK_COMMAND_OPTIONS "--error-exitcode=1 --leak-check=full")
    include(CTest)
    enable_testing()
    message("✅ GTest Included")
endif()



add_subdirectory(third_party)
add_subdirectory(shared)
add_subdirectory(thruna)
add_subdirectory(dev)