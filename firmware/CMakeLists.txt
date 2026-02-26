include(cmake/dependencies.cmake)
include(../scripts/code_generation/jsoncan/jsoncan.cmake)
include(../scripts/code_generation/jsoncan-rust/jsoncan.cmake)
include(../scripts/code_generation/commit_info_gen/commit_info.cmake)
include(cmake/eigenlib.cmake)   
include(cmake/autodiff.cmake)

IF ("${TARGET}" STREQUAL "binary")
    option(BOOTLOAD "Build the bootloader" OFF)
    option(WATCHDOG "Enable watchdog" OFF)
    include(cmake/embedded.cmake)
    include(cmake/embedded_libs.cmake)
    include(cmake/stmlib.cmake)
    include(cmake/bootlib.cmake)
    include(cmake/sbgecomlib.cmake)
    include(cmake/chimeralib.cmake)
    add_compile_definitions(TARGET_EMBEDDED)
ELSEIF ("${TARGET}" STREQUAL "test")
    include(cmake/test_libs.cmake)
    add_subdirectory(test)
    add_compile_definitions(TARGET_TEST)
    add_compile_definitions(_USE_MATH_DEFINES)
    # We assume this cmake is running on a x86 TARGET, so the
    # *default compiler* settings should be fine

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
ENDIF ()

# libraries
add_subdirectory(third_party)
add_subdirectory(shared)
add_subdirectory(logfs)

# board binaries
add_subdirectory(dev)
add_subdirectory(quintuna)
add_subdirectory(hexray)
