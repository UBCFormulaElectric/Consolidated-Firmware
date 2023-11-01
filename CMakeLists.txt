message("💡 Starting UBC Formula Electric Generate")

cmake_minimum_required(VERSION 3.24)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Globally accessible paths
set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(SCRIPTS_DIR "${CMAKE_SOURCE_DIR}/scripts")
set(ENV_BIN_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/environment/bin)
cmake_policy(SET CMP0135 NEW)
Set(FETCHCONTENT_QUIET FALSE)
# On Apple silicon, this line mysteriously causes loading CMake for unit tests to fail.
# However, it is equally mysteriously required for building embedded binaries.
# So, only run this if loading the embedded configuration.
# TODO: Look into what is causing this.
IF ("${PLATFORM}" STREQUAL "arm")
    project("UBC Formula Electric Software" C ASM)
    set(CMAKE_SYSTEM_NAME Generic)
ELSEIF ("${PLATFORM}" STREQUAL "x86")
    project("UBC Formula Electric Software" C CXX)
ELSEIF ("${PLATFORM}" STREQUAL "dimos")
    project("Dimos" C CXX)
ELSE ()
    message(FATAL_ERROR "❌ Please set the PLATFORM variable to 'x86' or 'arm'. To do this, set the -DPLATFORM=<platform>")
ENDIF()
message("⚙️ Configuring for ${PLATFORM} build target")

# ======== CPM ========
option(HANDLE_DEPS "Build build on the server" OFF)
option(NO_VENV "Disable invoking python scripts from virtual environment" OFF)
IF (HANDLE_DEPS)
    message("🏠 Handling the dependencies locally")
ELSE ()
    message("🏗️ Using your your dependencies")
    set(NO_VENV ON)
ENDIF ()
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
message("✅ Download CPM.cmake")

# ======== Python Checks ========
IF (NO_VENV)
    IF (WIN32)
        set(PIP_COMMAND pip)
        set(PYTHON_COMMAND python)
    ELSE ()
        set(PIP_COMMAND pip3)
        set(PYTHON_COMMAND python3)
    ENDIF ()
ELSE ()
    IF (WIN32)
        set(PYTHON_COMMAND pipenv run python)
    ELSE ()
        set(PYTHON_COMMAND pipenv run python3)
    ENDIF ()
ENDIF ()

find_package(Python3 COMPONENTS Interpreter)
find_program(PYTHON_HAS_COMMAND ${PYTHON_COMMAND})
IF (Python3_VERSION VERSION_LESS 3.8)
    message(FATAL_ERROR "❌ Python version 3.8 or greater is required")
ELSEIF (Python3_VERSION VERSION_GREATER_EQUAL 3.12)
    message(WARNING "⚠️ Python version 3.12 or greater is not supported as pyOCD does not support it")
ENDIF ()
message("🐍 Found Python ${Python3_VERSION}")

# ======== Python Checks ========

IF ("${PLATFORM}" STREQUAL "x86" OR "${PLATFORM}" STREQUAL "arm")
    add_subdirectory(firmware)
    IF ("${PLATFORM}" STREQUAL "x86")
        add_subdirectory(scripts/code_generation/fakegen)
    ENDIF ()
ELSEIF ("${PLATFORM}" STREQUAL "dimos")
    add_subdirectory(host)
ENDIF ()