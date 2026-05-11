# OUTPUTS
# - REQUIRED_LIBRARIES -> list of libraries to link

# libgpiod
IF("${TARGET}" STREQUAL "deploy")
    find_library(GPIOD_LIBRARY NAMES libgpiod gpiod)
    IF(NOT GPIOD_LIBRARY)
        message("${GPIOD_LIBRARY}")
        message(FATAL_ERROR "❌ gpiod library not found. Install sudo apt install libgpiod-dev")
    ENDIF()
    list(APPEND REQUIRED_LIBRARIES "gpiod")
    list(APPEND REQUIRED_LIBRARIES "gpiodcxx")
    message("📚✔️ Added gpiod library")
ENDIF()

# jsoncan
jsoncan_sources(dimos "${CMAKE_CURRENT_BINARY_DIR}/jsoncan" TRUE "quadruna")
add_library(dimos_can STATIC ${CAN_SRCS})
target_include_directories(dimos_can PUBLIC ${CAN_INCLUDE_DIRS})
target_compile_definitions(dimos_can PRIVATE -DTHREAD_SAFE_CAN_PACKING)
list(APPEND REQUIRED_LIBRARIES "dimos_can")
message("📚✔️ Added JsonCAN library")

# commit_info
commit_info_generate_sources(${EXECUTABLE_NAME} "${CMAKE_CURRENT_BINARY_DIR}/commit_info")
target_include_directories(dimos_commit_info PUBLIC ${COMMIT_INFO_INCLUDE_DIR})
list(APPEND REQUIRED_LIBRARIES "dimos_commit_info")
message("📚✔️ Added commit_info library")