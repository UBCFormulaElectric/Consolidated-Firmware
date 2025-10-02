message("💡 Starting UBC Formula Electric Software Build System")

cmake_minimum_required(VERSION 3.22.3)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
# use this for debugging
#set(CMAKE_VERBOSE_MAKEFILE on)
# Generates a comple_commands.json file which helps with VSCode intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_definitions("DEBUG=$<CONFIG:Debug>")
message("🛠️ Build Type: ${CMAKE_BUILD_TYPE}")

option(DEBUG_FETCHCONTENT "Enable verbose output for FetchContent" OFF)
IF (DEBUG_FETCHCONTENT)
    set(FETCHCONTENT_QUIET OFF)
ELSE ()
    set(FETCHCONTENT_QUIET ON)
ENDIF ()


# ======== Dependency Management Configs ========
message("")
include(get_cpm.cmake)
message("✅ Downloaded CPM.cmake")

# ======== Host system specific settings ========
message("")
message("🖥️ Host System: ${CMAKE_HOST_SYSTEM_NAME}")
IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    message("🪟 Building for Windows")
ELSEIF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    message("🍎 Building for Apples")
ELSEIF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    message("🐧 Building for Linux")
ELSE ()
    message(FATAL_ERROR "❌ Unsupported host system: ${CMAKE_HOST_SYSTEM_NAME}")
ENDIF ()

# ========== INCLUDES ==========
include("python.cmake")
include(scripts/code_generation/jsoncan/jsoncan.cmake)
include(scripts/code_generation/commit_info_gen/commit_info.cmake)

# ======== Clang-Format ========
set(FIX_FORMATTING_SCRIPT scripts/utilities/fix_formatting.py)
option(FIX_FORMATTING "Run clang-format on all boards" OFF)
IF (FIX_FORMATTING)
    message("⚙️ Run Clang-Formatting for all boards")
    execute_process(
            COMMAND ${PYTHON_COMMAND} ${FIX_FORMATTING_SCRIPT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
ENDIF ()

# ======== Code Time ========
option(PLATFORM "The platform to build for, either \"firmware\" or \"dimos\"" "firmware")
option(TARGET "The target to build for. Choose between \"binary\" and \"test\"." "binary")
message("")
message("⚙️ Configuring for \"${PLATFORM}\" platform and \"${TARGET}\" build target")
IF ("${PLATFORM}" STREQUAL "firmware")
    # Override AppleClang with Homebrew GCC for gcov support on macOS.
    IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin" AND ${TARGET} STREQUAL "test")
        message("📌 Searching for latest Homebrew GCC for host-based unit tests")

        file(GLOB HOMEBREW_GCC_CANDIDATES
                "/opt/homebrew/bin/gcc-[0-9]*"
                "/usr/local/bin/gcc-[0-9]*"
        )

        IF (HOMEBREW_GCC_CANDIDATES)
            list(SORT HOMEBREW_GCC_CANDIDATES COMPARE NATURAL ORDER DESCENDING)
            list(GET HOMEBREW_GCC_CANDIDATES 0 HOMEBREW_GCC)
            string(REPLACE "gcc" "g++" HOMEBREW_GXX ${HOMEBREW_GCC})

            set(CMAKE_C_COMPILER ${HOMEBREW_GCC} CACHE STRING "" FORCE)
            set(CMAKE_CXX_COMPILER ${HOMEBREW_GXX} CACHE STRING "" FORCE)

        ELSE ()
            message(FATAL_ERROR "❌ No Homebrew GCC installation found. Please install with 'brew install gcc'")
        ENDIF ()
    ENDIF ()

    include(firmware/cmake/shared.cmake)

    # ======== Compiler ========
    IF ("${TARGET}" STREQUAL "binary")
        project("UBC Formula Electric Software" C CXX ASM)
    ELSEIF ("${TARGET}" STREQUAL "test")
        project("UBC Formula Electric Software" C CXX)
    ELSE ()
        message(FATAL_ERROR "❌ Please set the TARGET variable to 'deploy' or 'test'. To do this, set the -DTARGET=<target>")
    ENDIF ()

    add_subdirectory(firmware)
    IF ("${TARGET}" STREQUAL "test")
        add_subdirectory(scripts/code_generation/fakegen)
        add_subdirectory(scripts/code_generation/jsoncan/test)
    ENDIF ()
ELSEIF ("${PLATFORM}" STREQUAL "dimos")
    project("UBC Formula Electric Software" VERSION 0.1 LANGUAGES C CXX)
    add_subdirectory(software/dimos)
ELSE ()
    message(FATAL_ERROR "❌ Please set the PLATFORM variable to 'firmware' or 'dimos'. To do this, set the -DPLATFORM=<platform>")
ENDIF ()
