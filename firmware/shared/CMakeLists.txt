message("")
message("⚙️ Configuring Shared")

IF("${TARGET}" STREQUAL "test")
    # shared modules tests
    file(GLOB_RECURSE SHARED_TEST_SRCS
            "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
    )
    set(SRCS ${SHARED_TEST_SRCS} ${SHARED_APP_SRCS})
    set(INCLUDE_DIRS ${SHARED_APP_INCLUDE_DIR} ${FFF_DIR} ${SHARED_TEST_UTILS_INCLUDE_DIRS})
    compile_gtest_executable("shared_test" "${SRCS}" "${INCLUDE_DIRS}")

    # jsoncan lib
    jsoncan_library("shared" "quadruna" "${CMAKE_CURRENT_BINARY_DIR}")

    target_link_libraries("shared_test" PRIVATE "quadruna_shared_jsoncan" "shared_fakes")
ENDIF()