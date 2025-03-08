list(APPEND JOURNAL_TEST_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/journal_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/journal.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/disk.c"
)

compile_gtest_executable(
    logfs2_journal_test
    "${JOURNAL_TEST_SRCS}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
