message("💡 Starting UBC Formula Electric Generate")

# On Apple silicon, this line mysteriously causes loading CMake for unit tests to fail.
# However, it is equally mysteriously required for building embedded binaries.
# So, only run this if loading the embedded configuration.
# TODO: Look into what is causing this.
if("${PLATFORM}" STREQUAL "arm")
    set(CMAKE_SYSTEM_NAME Generic)
endif()
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0135 NEW)

# Globally accessible paths
set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/firmware)
set(SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/scripts)
set(ENV_BIN_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/environment/bin)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(PYTHON_COMMAND "python")
    set(ENVBIN_DIR ${ENV_BIN_PREFIX}/win)
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(PYTHON_COMMAND "python3")
    set(ENVBIN_DIR ${ENV_BIN_PREFIX}/lin)
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    set(PYTHON_COMMAND "python3")
    set(ENVBIN_DIR ${ENV_BIN_PREFIX}/mac)
else()
    message(FATAL_ERROR "❌ Unsupported host system: ${CMAKE_HOST_SYSTEM_NAME}")
endif()


find_program(PIP_COMMAND pip)
IF(NOT PIP_COMMAND)
    message(FATAL_ERROR "❌ Could not find pip. Please install pip and try again.")
ENDIF()
message("✅ Update Python Dependencies")
execute_process(COMMAND pip install -r ${CMAKE_SOURCE_DIR}/environment/requirements.txt
                OUTPUT_QUIET)

message("✅ Configuring for ${PLATFORM} build target")

# Target-platform specific settings
if("${PLATFORM}" STREQUAL "arm")
    set(CMAKE_C_COMPILER_WORKS 1)
    set(TOOLCHAIN_PREFIX ${ENVBIN_DIR}/arm/bin/arm-none-eabi)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc.exe)
    set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc.exe)
    set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy.exe)
    set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump.exe)
    set(SIZE ${TOOLCHAIN_PREFIX}-size)
    project("UBC Formula Electric Software" C ASM)
elseif("${PLATFORM}" STREQUAL "x86")
    # We assume this cmake is running on a x86 platform, so the
    # default compiler settings should be fine
    project("UBC Formula Electric Software" C CXX)
else()
    message(FATAL_ERROR "❌ Please set the PLATFORM variable to 'x86' or 'arm'. To do this, set the -DPLATFORM=<platform>")
endif()

# If compiling on x86, fetch GoogleTest for unit testing.
if("${PLATFORM}" STREQUAL "x86")
    include(FetchContent)
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
endif()

add_subdirectory(firmware)

if("${PLATFORM}" STREQUAL "x86")
    add_subdirectory(scripts/code_generation/fakegen)
endif()