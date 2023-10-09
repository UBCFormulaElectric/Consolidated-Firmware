message("💡 Starting UBC Formula Electric Generate")

cmake_minimum_required(VERSION 3.11)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
# On Apple silicon, this line mysteriously causes loading CMake for unit tests to fail.
# However, it is equally mysteriously required for building embedded binaries.
# So, only run this if loading the embedded configuration.
# TODO: Look into what is causing this.
IF("${PLATFORM}" STREQUAL "arm")
    set(CMAKE_SYSTEM_NAME Generic)
ENDIF()

# Globally accessible paths
set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)
set(SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/scripts)
set(ENV_BIN_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/environment/bin)
cmake_policy(SET CMP0135 NEW)

option(BUILD_SERVER "Build build on the server" OFF)
option(NO_VENV "Disable invoking python scripts from virtual environment" OFF)
IF(BUILD_SERVER)
    message("🏗️ Building on the server")
    set(NO_VENV ON)
ELSE()
    message("🏠 Building locally")
ENDIF()
IF(NO_VENV)
    IF(WIN32)
        set(PIP_COMMAND pip)
    ELSE()
        set(PIP_COMMAND pip3)
    ENDIF ()
ELSE()
    IF(WIN32)
        set(PYTHON_COMMAND pipenv run python)
    ELSE()
        set(PYTHON_COMMAND pipenv run python3)
    ENDIF ()
ENDIF ()

# Host system specific settings
IF(WIN32)
    set(BINARY_SUFFIX ".exe")
    set(ARM_BINARY_URL "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-win32.zip")
ELSEIF(LINUX)
    set(BINARY_SUFFIX "")
    set(ARM_BINARY_URL "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2")
ELSEIF(APPLE)
    set(BINARY_SUFFIX "")
    set(ARM_BINARY_URL "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-mac.tar.bz2")
ELSE()
    message(FATAL_ERROR "❌ Unsupported host system: ${CMAKE_HOST_SYSTEM_NAME}")
ENDIF()

# Find all required pre-installed binaries
find_package(Python3 COMPONENTS Interpreter)
find_program(PYTHON_HAS_COMMAND ${PYTHON_COMMAND})
IF(Python3_VERSION VERSION_LESS 3.9)
    message(FATAL_ERROR "❌ Python version 3.9 or greater is required")
ELSEIF(Python3_VERSION VERSION_GREATER_EQUAL 3.12)
    message(WARNING "⚠️ Python version 3.12 or greater is not supported as pyOCD does not support it")
ENDIF()
message("🐍 Found Python ${Python3_VERSION}")


# Dependency Management
IF (NOT BUILD_SERVER)
    # Git Submodules
    execute_process(COMMAND "git submodule update --init --recursive" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    message("✅ Git Submodules Updated")

    # CPM + CMake Dependency Configuration
    file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
    )
    message("✅ Download CPM.cmake")
    include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

    # check pip is ok
    find_program(PIP_HAS_COMMAND pip)
    IF(NOT PIP_HAS_COMMAND)
        message(FATAL_ERROR "❌ Could not find pip. Please install pip and try again.")
    ELSE ()
        message("📥 Found pip")
    ENDIF()
    # python pip dependencies
    IF(NO_VENV)
        message("📦 Update Python Dependencies")
        execute_process(COMMAND "${PIP_COMMAND} -r ${CMAKE_SOURCE_DIR}/environment/requirements.txt"
                RESULT_VARIABLE PIP_INSTALL_STATUS)
        IF(NOT ${PIP_INSTALL_STATUS} EQUAL 0)
            message(FATAL_ERROR "❌ Could successfully install Python dependencies")
        ELSE ()
            message("✅ Python dependencies installed")
        ENDIF()
    ELSE()
        execute_process(COMMAND "pip install pipenv")
        execute_process(COMMAND pipenv install RESULT_VARIABLE PIPENV_STATUS OUTPUT_QUIET)
        IF(NOT ${PIPENV_STATUS} EQUAL 0)
            message(FATAL_ERROR "❌ Error setting up pipenv. Please install/fix pipenv and try again.")
        ELSE ()
            message("📥 Found pipenv")
        ENDIF ()
    ENDIF ()

    # arm toolchain
    IF("${PLATFORM}" STREQUAL "arm")
        # arm binary
        CPMAddPackage(${ARM_BINARY_URL})
        set(TOOLCHAIN_PREFIX ${gcc-arm-none-eabi-9_SOURCE_DIR}/bin/)
        IF(WIN32)
            # openocd
            CPMAddPackage("https://github.com/openocd-org/openocd/releases/download/v0.12.0/openocd-v0.12.0-i686-w64-mingw32.tar.gz")
            message("🔖 OpenOCD Installed At ${openocd_SOURCE_DIR}/bin/openocd.exe")
            # zadig
            IF(NOT EXISTS ${CMAKE_SOURCE_DIR}/zadig.exe)
                message(DEBUG "Zadig does not exist, installing now...")
                file(
                    DOWNLOAD
                    https://github.com/pbatard/libwdi/releases/download/v1.5.0/zadig-2.8.exe
                    ${CMAKE_SOURCE_DIR}/zadig.exe
                )
            ENDIF()
            message("🔖 Zadig has been installed in the root directory")
        ENDIF ()
    ENDIF()
ENDIF ()

message("⚙️ Configuring for ${PLATFORM} build target")

# Target specific settings
IF("${PLATFORM}" STREQUAL "arm")
    set(CMAKE_C_COMPILER_WORKS 1)
    set(TOOLCHAIN_PREFIX ${TOOLCHAIN_PREFIX}arm-none-eabi)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc${BINARY_SUFFIX})
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++${BINARY_SUFFIX})
    set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc${BINARY_SUFFIX})
    set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy${BINARY_SUFFIX})
    set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump${BINARY_SUFFIX})
    set(SIZE ${TOOLCHAIN_PREFIX}-size${BINARY_SUFFIX})
    project("UBC Formula Electric Software" C ASM)
ELSEIF("${PLATFORM}" STREQUAL "x86")
    # We assume this cmake is running on a x86 platform, so the
    # *default compiler* settings should be fine
    project("UBC Formula Electric Software" C CXX)

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
ELSE()
    message(FATAL_ERROR "❌ Please set the PLATFORM variable to 'x86' or 'arm'. To do this, set the -DPLATFORM=<platform>")
ENDIF()

add_subdirectory(firmware)

IF("${PLATFORM}" STREQUAL "x86")
    add_subdirectory(scripts/code_generation/fakegen)
ENDIF()