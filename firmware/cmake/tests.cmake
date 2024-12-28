if(NOT "${SHARED_CMAKE_INCLUDED}" STREQUAL "TRUE")
    message(FATAL_ERROR "❌ shared.cmake must be included before tests.cmake")
endif()

message("")
message("🧪 Configuring tests")

set(FFF_DIR "${THIRD_PARTY_DIR}/fff")

message("  🔃 Registering function compile_gtest_executable")
function(compile_gtest_executable
    TEST_EXECUTABLE_NAME
    TEST_SRCS
    TEST_INCLUDE_DIRS
)
    add_executable(${TEST_EXECUTABLE_NAME}
        ${TEST_SRCS}
    )
    target_include_directories(${TEST_EXECUTABLE_NAME}
        PRIVATE
        ${TEST_INCLUDE_DIRS}
    )
    target_compile_options(${TEST_EXECUTABLE_NAME}
        PUBLIC
        -Wall
    )
    target_link_libraries(${TEST_EXECUTABLE_NAME} gtest_main)
    add_test(NAME ${TEST_EXECUTABLE_NAME}
        COMMAND ${TEST_EXECUTABLE_NAME})
endfunction()

message("  🔃 Registering function compile_fake_library")
function(create_fake_library
    LIB_NAME
    HDRS_TO_FAKE
)
    file(GLOB_RECURSE FAKEGEN_SRCS
        ${SCRIPTS_DIR}/code_generation/fakegen/src/*.py
        ${SCRIPTS_DIR}/code_generation/fakegen/src/*.j2
    )

    foreach(HDR_TO_FAKE ${HDRS_TO_FAKE})
        get_filename_component(HDR_MODULE_NAME ${HDR_TO_FAKE} NAME_WE)
        get_filename_component(HDR_DIR ${HDR_TO_FAKE} DIRECTORY)
        set(FAKE_HDR "${CMAKE_CURRENT_BINARY_DIR}/fake_${HDR_MODULE_NAME}.hpp")
        set(FAKE_SRC "${CMAKE_CURRENT_BINARY_DIR}/fake_${HDR_MODULE_NAME}.cpp")
        list(APPEND FAKE_HDRS ${FAKE_HDR})
        list(APPEND FAKE_SRCS ${FAKE_SRC})
        add_custom_command(
            OUTPUT ${FAKE_HDR} ${FAKE_SRC}
            COMMAND ${PYTHON_COMMAND}
            ${SCRIPTS_DIR}/code_generation/fakegen/src/generate_fake.py
            --header ${HDR_TO_FAKE}
            --output-header ${FAKE_HDR}
            --output-source ${FAKE_SRC}
            WORKING_DIRECTORY ${REPO_ROOT_DIR}
            DEPENDS ${FAKEGEN_SRCS} ${HDR_TO_FAKE}
        )
    endforeach()

    add_library(${LIB_NAME} STATIC ${FAKE_SRCS})
    target_compile_options(${LIB_NAME}
            PUBLIC
            -Wall
            -g3
    )
    target_include_directories(${LIB_NAME}
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${HDR_DIR}
        ${SHARED_APP_INCLUDE_DIR}
        ${SHARED_IO_INCLUDE_DIR}
    )
endfunction()