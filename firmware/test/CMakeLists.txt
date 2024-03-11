add_library(api1 SHARED 
    "${CMAKE_CURRENT_SOURCE_DIR}/api1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/impl.c"
)
target_compile_options(api1 PRIVATE -fvisibility=hidden)

add_library(api2 SHARED 
    "${CMAKE_CURRENT_SOURCE_DIR}/api2.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/impl.c"
)
target_compile_options(api2 PRIVATE -fvisibility=hidden)

add_executable(main "${CMAKE_CURRENT_SOURCE_DIR}/main.c")
target_link_libraries(main api1 api2)