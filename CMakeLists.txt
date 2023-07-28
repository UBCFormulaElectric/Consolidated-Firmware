
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.11)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

message("Platform: ${PLATFORM}")

# Target-platform specific settings
if("${PLATFORM}" STREQUAL "x86")
# We assume this cmake is running on a x86 platform, so the
# default compiler settings should be fine
elseif("${PLATFORM}" STREQUAL "arm")
    set(CMAKE_C_COMPILER_WORKS 1)
    set(TOOLCHAIN_PREFIX arm-none-eabi-)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
    set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
    set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
    set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump)
    set(SIZE ${TOOLCHAIN_PREFIX}size)
else()
    message(FATAL_ERROR
        "ERROR: Please set the PLATFORM variable to 'x86' or 'arm'.")
endif()

# Setup project.
if("${PLATFORM}" STREQUAL "x86")
    project("UBC Formula Electric Software" C CXX)
elseif("${PLATFORM}" STREQUAL "arm")
    project("UBC Formula Electric Software" C ASM)
endif()

# Globally accessible paths
set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(BOARDS_DIR ${CMAKE_SOURCE_DIR}/boards)
set(SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/scripts)
set(PIPENV_PROJECT_DIR ${REPO_ROOT_DIR})

# Add option to disable invoking python from a virtual environment.
# Invocation of python should be done with PYTHON_CMD.
# Used in CI where packages are installed system-wide in a Docker container at build time.
# This was done as a workaround, since I couldn't figure out how to pre-install packages into
# a Pipenv virtual environment when building the container.
option(NO_VENV "Disable invoking python scripts from virtual environment" OFF)

if(NO_VENV)
    set(PYTHON_CMD python3)
else()
    set(PYTHON_CMD pipenv run python)
endif()

function(download_and_unpack_google_test GOOGLETEST_DOWNLOAD_SCRIPT)
    # Download and unpack googletest at configure time
    configure_file(${GOOGLETEST_DOWNLOAD_SCRIPT} googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)

    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)

    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)
endfunction()

if("${PLATFORM}" STREQUAL "x86")
    download_and_unpack_google_test(${CMAKE_SOURCE_DIR}/CMakeLists.txt.in)

    # For ctest to return a non-zero code when Valgrind fails, we must use
    # --error-exitcode. On the other hand, --leak-check=full ensures that any
    # memory leak issue will cause ctest to return a non-zero code.
    #
    # This must be set before include(CTest) is called, or else
    # MEMORYCHECK_COMMAND_OPTIONS won't be overwritten correctly!
    set(MEMORYCHECK_COMMAND_OPTIONS "--error-exitcode=1 --leak-check=full")
    include(CTest)
    enable_testing()
endif()

add_subdirectory(boards)