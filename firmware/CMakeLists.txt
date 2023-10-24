include(cmake/embedded.cmake)
include(cmake/tests.cmake)
include(cmake/shared.cmake)

if("${PLATFORM}" STREQUAL "arm")
    add_compile_definitions(TARGET_EMBEDDED)
elseif("${PLATFORM}" STREQUAL "x86")
    add_compile_definitions(TARGET_TEST)
endif()

add_subdirectory(third_party)
add_subdirectory(shared)
add_subdirectory(thruna)