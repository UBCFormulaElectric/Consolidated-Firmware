
cmake_minimum_required(VERSION 3.11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Target-platform specific settings
message("Platform: ${PLATFORM}")

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

add_subdirectory(boards)