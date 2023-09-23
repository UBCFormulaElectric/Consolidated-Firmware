function(create_fake_library
    LIBRARY_NAME
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
            COMMAND ${PYTHON_CMD}
            ${SCRIPTS_DIR}/code_generation/fakegen/src/generate_fake.py
            --header ${HDR_TO_FAKE}
            --output-header ${FAKE_HDR}
            --output-source ${FAKE_SRC}
            WORKING_DIRECTORY ${PIPENV_PROJECT_DIR}
            DEPENDS ${FAKEGEN_SRCS} ${HDR_TO_FAKE}
        )
    endforeach()

    add_library(${LIBRARY_NAME} STATIC ${FAKE_SRCS})
    target_compile_options(${LIBRARY_NAME}
        PUBLIC
        -Wall
        -Werror
        -g3
    )
    target_include_directories(${LIBRARY_NAME}
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${HDR_DIR}
    )
endfunction()

set(TEST_HDRS_TO_FAKE
    "${CMAKE_CURRENT_SOURCE_DIR}/test/test_header.h"
)
set(TEST_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/test/test_fakegen.cpp"
)
set(TEST_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
create_fake_library(
    "fakegen_test_fakes"
    "${TEST_HDRS_TO_FAKE}"
)
compile_gtest_executable(
    "fakegen_test"
    "${TEST_SRCS}"
    "${TEST_INCLUDE_DIRS}"
)
target_link_libraries("fakegen_test" "fakegen_test_fakes")