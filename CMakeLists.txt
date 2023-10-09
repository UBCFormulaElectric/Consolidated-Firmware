message("💡 Starting UBC Formula Electric Generate")

cmake_minimum_required(VERSION 3.11)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
# On Apple silicon, this line mysteriously causes loading CMake for unit tests to fail.
# However, it is equally mysteriously required for building embedded binaries.
# So, only run this if loading the embedded configuration.
# TODO: Look into what is causing this.
if("${PLATFORM}" STREQUAL "arm")
    set(CMAKE_SYSTEM_NAME Generic)
endif()

# Get CPM + CMake Dependency Configuration
cmake_policy(SET CMP0135 NEW)
file(
    DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
    EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# Python dependencies configuration
find_program(PIP_COMMAND pip)
IF(NOT PIP_COMMAND)
    message(FATAL_ERROR "❌ Could not find pip. Please install pip and try again.")
ENDIF()

# Globally accessible paths
set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)
set(SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/scripts)
set(ENV_BIN_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/environment/bin)

# Host system specific settings
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(PYTHON_COMMAND "python")
    set(ARM_BINARY_URL "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-win32.zip")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(PYTHON_COMMAND "python3")
    set(ARM_BINARY_URL "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    set(PYTHON_COMMAND "python3")
    set(ARM_BINARY_URL "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-mac.tar.bz2")
else()
    message(FATAL_ERROR "❌ Unsupported host system: ${CMAKE_HOST_SYSTEM_NAME}")
endif()

message("⚙️ Configuring for ${PLATFORM} build target")

# Target specific settings
if("${PLATFORM}" STREQUAL "arm")
    CPMAddPackage(${ARM_BINARY_URL})
    message("✅ Arm Compiler Installed")

    message("📦 Update Python Dependencies")
    # make command throw and error if it doesn't work
    execute_process(COMMAND pip install -r ${CMAKE_SOURCE_DIR}/environment/requirements.txt RESULT_VARIABLE PIP_INSTALL_STATUS OUTPUT_QUIET)
    if(NOT ${PIP_INSTALL_STATUS} EQUAL 0)
        message(FATAL_ERROR "❌ Could successfully install Python dependencies")
    else ()
        message("✅ Python dependencies installed")
    endif()

    set(CMAKE_C_COMPILER_WORKS 1)
    set(TOOLCHAIN_PREFIX ${gcc-arm-none-eabi-9_SOURCE_DIR}/bin/arm-none-eabi)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc.exe)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++.exe)
    set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc.exe)
    set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy.exe)
    set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump.exe)
    set(SIZE ${TOOLCHAIN_PREFIX}-size.exe)
    project("UBC Formula Electric Software" C ASM)
elseif("${PLATFORM}" STREQUAL "x86")
    # We assume this cmake is running on a x86 platform, so the
    # *default compiler* settings should be fine
    project("UBC Formula Electric Software" C CXX)

    # If compiling on x86, fetch GoogleTest for unit testing.
    include(FetchContent) # TODO migrate to CPM

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # For ctest to return a non-zero code when Valgrind fails, we must use
    # --error-exitcode. On the other hand, --leak-check=full ensures that any
    # memory leak issue will cause ctest to return a non-zero code.
    #
    # This must be set before include(CTest) is called, or else
    # MEMORYCHECK_COMMAND_OPTIONS won't be overwritten correctly!
    set(MEMORYCHECK_COMMAND_OPTIONS "--error-exitcode=1 --leak-check=full")
    include(CTest)
    enable_testing()
    message("✅ GTest Included")
else()
    message(FATAL_ERROR "❌ Please set the PLATFORM variable to 'x86' or 'arm'. To do this, set the -DPLATFORM=<platform>")
endif()

add_subdirectory(firmware)

if("${PLATFORM}" STREQUAL "x86")
    add_subdirectory(scripts/code_generation/fakegen)
endif()