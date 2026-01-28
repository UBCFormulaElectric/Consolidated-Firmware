message("")
message("📦 [dependencies.cmake] Handling Dependencies")

# Git Submodules
execute_process(COMMAND git submodule update --init --recursive WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
message("  ✅ Git Submodules Updated at ${CMAKE_SOURCE_DIR}")

# Protobuf Compiler - protoc
find_program(HAS_PROTOBUF_COMPILER protoc)
IF (NOT HAS_PROTOBUF_COMPILER)
    IF (${CMAKE_HOST_WIN32})
        message(FATAL_ERROR "Could not find protoc. Please install it from https://github.com/protocolbuffers/protobuf/releases")
    ELSEIF (${CMAKE_HOST_LINUX})
        set(PROTOBUF_COMPILER "\"apt install -y protobuf-compiler\"")
    ELSEIF (${CMAKE_HOST_APPLE})
        set(PROTOBUF_COMPILER "\"brew install protobuf\"")
    ENDIF ()
    message(FATAL_ERROR "Could not find protoc. Please run ${PROTOBUF_COMPILER} to install it.")
ENDIF ()
message("  ✅ Found protoc at ${HAS_PROTOBUF_COMPILER}")

message("  🚚 Fetching Sources")
# Source Dependencies
IF ("${TARGET}" STREQUAL "binary")
    # STM32F4Cube firmware package: Contains STM32 HAL drivers and FreeRTOS with the CMSIS-RTOS v2 API.
    CPMAddPackage(
            NAME STM32CUBEF4
            GITHUB_REPOSITORY UBCFormulaElectric/STM32CubeF4
            GIT_TAG 623397fd34f0788ef7b6aea1d2deaa22bbc4091d
            GIT_SHALLOW TRUE
            DOWNLOAD_ONLY TRUE
    )

    # STM32H7Cube firmware package: Contains STM32 HAL drivers and FreeRTOS with the CMSIS-RTOS v2 API.
    CPMAddPackage(
            NAME STM32CUBEH7
            GITHUB_REPOSITORY UBCFormulaElectric/STM32CubeH7
            GIT_TAG c709838f91f1356cd3461eb2c5981c481dd0884a
            GIT_SHALLOW TRUE
            DOWNLOAD_ONLY TRUE
    )

    # STM32H5Cube firmware package: Contains STM32 HAL drivers and FreeRTOS with the CMSIS-RTOS v2 API.
    CPMAddPackage(
            NAME STM32CUBEH5
            GITHUB_REPOSITORY UBCFormulaElectric/STM32CubeH5
            GIT_TAG d02f23d1a88c733635d28842c4e4b5549a7ad333
            GIT_SHALLOW TRUE
            DOWNLOAD_ONLY TRUE
    )

    # SEGGER SystemView for RTT Logging and SystemView profiling
    CPMAddPackage(
            NAME SEGGER_SYSTEMVIEW
            URL https://www.segger.com/downloads/systemview/SystemView_Src_V360d.zip
            DOWNLOAD_ONLY TRUE
    )

    # SBG Systems library: Contains the SBG Systems API for interfacing with the Inertial Measurement Unit (IMU).
    CPMAddPackage(
            NAME SBG_ECOM
            GITHUB_REPOSITORY SBG-Systems/sbgECom
            GIT_TAG 05c19cdd7c021a8474b2bffc8ca4ec274f6a9155
            GIT_SHALLOW TRUE
            DOWNLOAD_ONLY TRUE
    )
    CPMAddPackage(
        NAME AUTO_DIFF
        GITHUB_REPOSITORY autodiff/autodiff
        GIT_TAG 2e2f3a2b16afcd9c04e76c8a689e9fd23ff78679
        GIT_SHALLOW TRUE
        DOWNLOAD_ONLY TRUE
    )
ELSEIF ("${TARGET}" STREQUAL "test")
    # Fetch GoogleTest for unit testing.
    CPMAddPackage(
            NAME googletest
            GITHUB_REPOSITORY "google/googletest"
            GIT_TAG "release-1.12.1"
            GIT_SHALLOW TRUE
    )
ENDIF ()

# protobufs
CPMAddPackage(
        NAME NANOPB_REPO
        GITHUB_REPOSITORY nanopb/nanopb
        GIT_TAG 0.4.8
        GIT_SHALLOW TRUE
        DOWNLOAD_ONLY TRUE
)
list(APPEND CMAKE_MODULE_PATH ${NANOPB_REPO_SOURCE_DIR}/extra)
find_package(Nanopb REQUIRED)
message("  ✅ Sources Fetched")
