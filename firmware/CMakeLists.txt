include(cmake/embedded.cmake)
include(cmake/tests.cmake)
include(cmake/shared.cmake)

# ======== Host system specific settings ========
IF ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
    set(BINARY_SUFFIX ".exe")
ELSEIF (APPLE)
    set(BINARY_SUFFIX "")
ELSEIF ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
    set(BINARY_SUFFIX "")
ELSEIF (UNIX)
    set(BINARY_SUFFIX "")
ELSE ()
    message(FATAL_ERROR "❌ Unsupported host system: ${CMAKE_HOST_SYSTEM_NAME}")
ENDIF ()

# ======== Dependency Handling ========
IF (HANDLE_DEPS)
    include(cmake/dependencies.cmake)
ENDIF()

# ======== Compiler ========
IF ("${PLATFORM}" STREQUAL "arm")
    set(CMAKE_C_COMPILER_WORKS 1)
    set(TOOLCHAIN_PREFIX ${TOOLCHAIN_PREFIX}arm-none-eabi)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc${BINARY_SUFFIX})
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++${BINARY_SUFFIX})
    set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc${BINARY_SUFFIX})
    set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy${BINARY_SUFFIX})
    set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump${BINARY_SUFFIX})
    set(SIZE ${TOOLCHAIN_PREFIX}-size${BINARY_SUFFIX})
    message("✅ ARM Toolchain Included")
ENDIF ()

# ======== Libraries ========
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

    # We assume this cmake is running on a x86 platform, so the
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