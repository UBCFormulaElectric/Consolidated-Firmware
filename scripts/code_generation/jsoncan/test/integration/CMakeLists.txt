message(STATUS "Configuring jsoncan_test")
jsoncan_sources(
        "ECU1"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
        "${CMAKE_CURRENT_BINARY_DIR}/jsoncan_test.dbc"
        "${CMAKE_CURRENT_SOURCE_DIR}/../json_configs/valid_json1"
)
add_library("jsoncan_test_lib" ${CAN_SRCS})
# target_sources("jsoncan_test_lib" PRIVATE ${CAN_SRCS})
target_include_directories("jsoncan_test_lib" PUBLIC "${CAN_INCLUDE_DIRS}")
target_compile_definitions("jsoncan_test_lib" PRIVATE THREAD_SAFE_CAN_PACKING)

file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")
compile_gtest_executable(
        "jsoncan_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
)
target_link_libraries("jsoncan_test" PRIVATE "jsoncan_test_lib")