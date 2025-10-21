message("")
message("Configuring jsoncan_test")
jsoncan_sources(
        "ECU1"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
        "${CMAKE_CURRENT_BINARY_DIR}/jsoncan_test.dbc"
        "${CMAKE_CURRENT_SOURCE_DIR}/../json_configs/valid_json1"
)
add_library("jsoncan_test_lib" INTERFACE)
target_sources("jsoncan_test_lib" INTERFACE ${CAN_SRCS})
no_checks("${CAN_SRCS}")
target_include_directories("jsoncan_test_lib" INTERFACE "${CAN_INCLUDE_DIRS}")
target_compile_definitions("jsoncan_test_lib" INTERFACE THREAD_SAFE_CAN_PACKING)

file(GLOB SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")
compile_gtest_executable(
        "jsoncan_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
)
target_link_libraries("jsoncan_test" PRIVATE "jsoncan_test_lib")