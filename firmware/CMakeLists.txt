include(cmake/embedded.cmake)
include(cmake/tests.cmake)
include(cmake/shared.cmake)

# ======== Libraries ========
# protobufs
CPMAddPackage(
        NAME NANOPB_REPO
        GITHUB_REPOSITORY nanopb/nanopb
        GIT_TAG f6187b05a5eaf753faa0e29a5acd77dd0544d474
        GIT_SHALLOW TRUE
        DOWNLOAD_ONLY TRUE
)
list(APPEND CMAKE_MODULE_PATH ${NANOPB_REPO_SOURCE_DIR}/extra)
find_package(Nanopb REQUIRED)
if("${TARGET}" STREQUAL "deploy")
    add_compile_definitions(TARGET_EMBEDDED)

    # STM32F4Cube firmware package: Contains STM32 HAL drivers and FreeRTOS with the CMSIS-RTOS v2 API.
    CPMAddPackage(
        NAME STM32CUBEF4
        GITHUB_REPOSITORY UBCFormulaElectric/STM32CubeF4
        GIT_TAG 3e907f83929363b6909ae1d9a3b7305866a67b82
        GIT_SHALLOW TRUE
    )

    # STM32H7Cube firmware package: Contains STM32 HAL drivers and FreeRTOS with the CMSIS-RTOS v2 API.
    CPMAddPackage(
        NAME STM32CUBEH7
        GITHUB_REPOSITORY UBCFormulaElectric/STM32CubeH7
        GIT_TAG 1e1def98759f14a4977ef6234c90dc10cbf43a48
        GIT_SHALLOW TRUE
    )

    # littlefs: Filesystem for microcontrollers
    CPMAddPackage(
        NAME LITTLEFS
        GITHUB_REPOSITORY littlefs-project/littlefs
        GIT_TAG v2.8.0
        GIT_SHALLOW TRUE
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

    embedded_library(
        "littlefs_cm7"
        "${LITTLEFS_SRCS}"
        "${LITTLEFS_SOURCE_DIR}"
        "cm4"
        TRUE
    )
elseif("${TARGET}" STREQUAL "test")
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

if("${TARGET}" STREQUAL "deploy")
    add_subdirectory(chimera)
endif()

add_subdirectory(logfs)
add_subdirectory(quadruna)
add_subdirectory(dev) 
