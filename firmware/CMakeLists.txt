include(cmake/dependencies.cmake)
include(cmake/shared.cmake)

IF ("${TARGET}" STREQUAL "binary")
    option(BOOTLOAD "Build the bootloader" OFF)
    option(WATCHDOG "Enable watchdog" OFF)
    include(cmake/embedded_libs.cmake)
    include(cmake/stmlib.cmake)
    include(cmake/embedded.cmake)
    include(cmake/bootlib.cmake)
    include(cmake/sbgecomlib.cmake)
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


add_subdirectory(third_party)
add_subdirectory(shared)
IF ("${TARGET}" STREQUAL "binary")
    #    add_subdirectory(chimera)
    add_subdirectory(chimera_v2)
ENDIF ()
add_subdirectory(logfs)
add_subdirectory(quintuna)
add_subdirectory(dev)
add_subdirectory(hexray)
