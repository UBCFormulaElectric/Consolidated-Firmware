# OUTPUTS
# - REQUIRED_LIBRARIES -> list of libraries to link

#gpiod
if (${LINUX})
    find_library(GPIOD_LIBRARY NAMES libgpiod gpiod)
    if (NOT GPIOD_LIBRARY)
        message("${GPIOD_LIBRARY}")
        message(FATAL_ERROR "❌ gpiod library not found. Install sudo apt install libgpiod-dev")
    endif ()
    list(APPEND REQUIRED_LIBRARIES "gpiod")
    list(APPEND REQUIRED_LIBRARIES "gpiodcxx")
    message("📚✔️ Added gpiod library")
endif ()

# jsoncan
jsoncan_sources(dimos "${CMAKE_CURRENT_BINARY_DIR}/jsoncan" TRUE "quadruna")
add_library(dimos_can STATIC ${CAN_SRCS})
target_include_directories(dimos_can PUBLIC ${CAN_INCLUDE_DIRS})
target_compile_definitions(dimos_can PRIVATE -DTHREAD_SAFE_CAN_PACKING)
list(APPEND REQUIRED_LIBRARIES "dimos_can")
message("📚✔️ Added JsonCAN library")

# commit_info
commit_info_generate_sources(${EXECUTABLE_NAME} "${CMAKE_CURRENT_BINARY_DIR}/commit_info")
add_library(dimos_commit_info STATIC ${COMMIT_INFO_SRC})
target_include_directories(dimos_commit_info PUBLIC ${COMMIT_INFO_INCLUDE_DIR})
list(APPEND REQUIRED_LIBRARIES "dimos_commit_info")
message("📚✔️ Added commit_info library")