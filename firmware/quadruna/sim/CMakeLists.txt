compile_gtest_executable(
    sim_test
    "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(sim_test quadruna_VC_sim)