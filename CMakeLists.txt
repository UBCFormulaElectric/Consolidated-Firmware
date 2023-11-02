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

# ======== CPM ========
option(HANDLE_DEPS "Build build on the server" OFF)
option(NO_VENV "Disable invoking python scripts from virtual environment" OFF)
IF (HANDLE_DEPS)
    message("🏠 Handling the dependencies locally")
ELSE ()
    message("🏗️ Using your dependencies")
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

# ======== Host system specific settings ========
IF ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
    set(BINARY_SUFFIX ".exe")
ELSEIF (APPLE)
    set(BINARY_SUFFIX "")
ELSEIF (UNIX)
    set(BINARY_SUFFIX "")
ELSE ()
    message(FATAL_ERROR "❌ Unsupported host system: ${CMAKE_HOST_SYSTEM_NAME}")
ENDIF ()

message("⚙️ Configuring for ${PLATFORM}-${TARGET} build target")
IF ("${PLATFORM}" STREQUAL "firmware")
    # ======== Dependency Handling ========
    IF (HANDLE_DEPS)
        include(firmware/cmake/dependencies.cmake)
    ENDIF()

    # ======== Compiler ========
    IF("${TARGET}" STREQUAL "arm")
        # On Apple silicon, this line mysteriously causes loading CMake for unit tests to fail.
        # However, it is equally mysteriously required for building embedded binaries.
        # So, only run this if loading the embedded configuration.
        # TODO: Look into what is causing this.
        set(CMAKE_C_COMPILER_WORKS 1)
        message("${TOOLCHAIN_PREFIX}")
        set(TOOLCHAIN_PREFIX ${TOOLCHAIN_PREFIX}arm-none-eabi)
        set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc${BINARY_SUFFIX})
        set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++${BINARY_SUFFIX})
        set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc${BINARY_SUFFIX})
        set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy${BINARY_SUFFIX})
        set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump${BINARY_SUFFIX})
        set(SIZE ${TOOLCHAIN_PREFIX}-size${BINARY_SUFFIX})
        message("✅ ARM Toolchain Included")
        set(CMAKE_SYSTEM_NAME Generic)
        project("UBC Formula Electric Software" C ASM)
    ELSEIF ("${TARGET}" STREQUAL "x86")
        project("UBC Formula Electric Software" C CXX)
    ELSE()
        message(FATAL_ERROR "❌ Please set the TARGET variable to 'arm' or 'x86'. To do this, set the -DTARGET=<target>")
    ENDIF()
ELSEIF ("${PLATFORM}" STREQUAL "dimos")
    set(DIMOS_VERSION 0.1)
    IF("${TARGET}" STREQUAL "dev")
        project("Dimos Local Dev" VERSION ${DIMOS_VERSION} LANGUAGES CXX)
    ELSEIF ("${TARGET}" STREQUAL "deploy")
        project("Dimos Deploy" VERSION ${DIMOS_VERSION} LANGUAGES CXX)
    ELSEIF ("${TARGET}" STREQUAL "test")
        project("Dimos Test" VERSION ${DIMOS_VERSION} LANGUAGES CXX)
    ELSE()
        message(FATAL_ERROR "❌ Please set the TARGET variable to 'dev', 'deploy' or 'test'. To do this, set the -DTARGET=<target>")
    ENDIF()
ELSE ()
    message(FATAL_ERROR "❌ Please set the PLATFORM variable to 'firmware' or 'dimos'. To do this, set the -DPLATFORM=<platform>")
ENDIF()

# ======== Python Checks ========
IF (NO_VENV)
    IF ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
        set(PIP_COMMAND pip)
        set(PYTHON_COMMAND python)
    ELSE ()
        set(PIP_COMMAND pip3)
        set(PYTHON_COMMAND python3)
    ENDIF ()
ELSE ()
    IF ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
        set(PYTHON_COMMAND pipenv run python)
    ELSE ()
        set(PYTHON_COMMAND pipenv run python3)
    ENDIF ()
ENDIF ()

find_package(Python3 COMPONENTS Interpreter)
message("Python Command Set to ${PYTHON_COMMAND}")
find_program(PYTHON_HAS_COMMAND ${PYTHON_COMMAND})
IF (Python3_VERSION VERSION_LESS 3.8)
    message(FATAL_ERROR "❌ Python version 3.8 or greater is required")
ELSEIF (Python3_VERSION VERSION_GREATER_EQUAL 3.12)
    message(WARNING "⚠️ Python version 3.12 or greater is not supported as pyOCD does not support it")
ENDIF ()
message("🐍 Found Python ${Python3_VERSION}")

# ======== Python Checks ========

IF ("${PLATFORM}" STREQUAL "firmware")
    add_subdirectory(firmware)
    IF ("${TARGET}" STREQUAL "x86")
        add_subdirectory(scripts/code_generation/fakegen)
    ENDIF ()
ELSEIF ("${PLATFORM}" STREQUAL "dimos")
    add_subdirectory(software/dimos)
ENDIF ()

message(${CMAKE_PROJECT_NAME})