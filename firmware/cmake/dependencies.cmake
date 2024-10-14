message("")
message("📦 [dependencies.cmake] Handling Dependencies")

# Git Submodules
execute_process(COMMAND "git submodule update --init --recursive" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
message("  ✅ Git Submodules Updated")

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
IF("${TARGET}" STREQUAL "binary")
    # STM32F4Cube firmware package: Contains STM32 HAL drivers and FreeRTOS with the CMSIS-RTOS v2 API.
    CPMAddPackage(
            NAME STM32CUBEF4
            GITHUB_REPOSITORY UBCFormulaElectric/STM32CubeF4
            GIT_TAG 3e907f83929363b6909ae1d9a3b7305866a67b82
            GIT_SHALLOW TRUE
    )

    # STM32H7Cube firmware package: Contains STM32 HAL drivers and FreeRTOS with the CMSIS-RTOS v2 API.
    CPMAddPackage(
            NAME STM32CUBEH7
            GITHUB_REPOSITORY STMicroelectronics/STM32CubeH7
            GIT_TAG ebe554a89fd3f958233f1b464757d1891ac59b6d
            GIT_SHALLOW TRUE
    )

    # SEGGER SystemView for RTT Logging and SystemView profiling
    CPMAddPackage(
            NAME SEGGER_SYSTEMVIEW
            GITHUB_REPOSITORY UBCFormulaElectric/SEGGER_SystemView_Src
            GIT_TAG 106cd0cff80eab1ce55a92e7c6ea955c6145b982
            GIT_SHALLOW TRUE
    )
ELSEIF("${TARGET}" STREQUAL "test")
    # Fetch GoogleTest for unit testing.
    CPMAddPackage(
            NAME googletest
            GITHUB_REPOSITORY "google/googletest"
            GIT_TAG "release-1.12.1"
            GIT_SHALLOW TRUE
    )
ENDIF()

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
