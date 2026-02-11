if (NOT "${SHARED_CMAKE_INCLUDED}" STREQUAL "TRUE")
    message(FATAL_ERROR "❌ shared.cmake must be included before tests.cmake")
endif ()

message("")
message("🧪 Configuring tests")

set(FFF_DIR "${THIRD_PARTY_DIR}/fff")

message("  🔃 Registering function create_fake_library")
function(create_fake_library
        FAKE_LIB_NAME
        HDRS_TO_FAKE
)
    file(GLOB_RECURSE FAKEGEN_SCRIPT_SRCS
            ${SCRIPTS_DIR}/code_generation/fakegen/src/*.py
            ${SCRIPTS_DIR}/code_generation/fakegen/src/*.j2
    )
    set(FAKEGEN_DIR "${CMAKE_CURRENT_BINARY_DIR}")

    unset(FAKE_HDRS CACHE)
    unset(FAKE_SRCS CACHE)
    unset(FAKE_HDR_DIRS CACHE)
    foreach (HDR_TO_FAKE ${HDRS_TO_FAKE})
        get_filename_component(HDR_MODULE_NAME ${HDR_TO_FAKE} NAME_WE)
        get_filename_component(HDR_DIR ${HDR_TO_FAKE} DIRECTORY)
        set(FAKE_HDR "${FAKEGEN_DIR}/fake_${HDR_MODULE_NAME}.hpp")
        set(FAKE_SRC "${FAKEGEN_DIR}/fake_${HDR_MODULE_NAME}.cpp")
        list(APPEND FAKE_HDRS ${FAKE_HDR})
        list(APPEND FAKE_SRCS ${FAKE_SRC})

        add_custom_command(
                OUTPUT ${FAKE_SRC} ${FAKE_HDR}
                COMMAND ${PYTHON_COMMAND}
                ${SCRIPTS_DIR}/code_generation/fakegen/src/generate_fake.py
                --header ${HDR_TO_FAKE}
                --output-header ${FAKE_HDR}
                --output-source ${FAKE_SRC}
                COMMENT "Generating fake for ${HDR_TO_FAKE}"
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                DEPENDS ${FAKEGEN_SCRIPT_SRCS} ${HDR_TO_FAKE}
        )
        list(APPEND FAKE_HDR_DIRS ${HDR_DIR})
        list(REMOVE_DUPLICATES FAKE_HDR_DIRS)
    endforeach ()

    # build the library
    # all the library needs to do is let the parent know about the sources that it provides
    # and the location of the headers
    message("  🔃 Creating fake library ${FAKE_LIB_NAME} with sources ${FAKE_SRCS}")
    add_library(${FAKE_LIB_NAME} INTERFACE)
    target_sources(${FAKE_LIB_NAME} INTERFACE ${FAKE_SRCS})
    no_checks("${FAKE_SRCS}")
    target_include_directories(${FAKE_LIB_NAME} INTERFACE ${FAKEGEN_DIR} ${FAKE_HDR_DIRS})
endfunction()

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
            ${TEST_DIR}
    )
    if (MSVC)
        target_compile_options(${TEST_EXECUTABLE_NAME}
                PRIVATE
                /W3
                /WX
        )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${TEST_EXECUTABLE_NAME}
                PRIVATE
                ${SHARED_GNU_COMPILER_CHECKS_STRICT}
        )
        target_compile_options(${TEST_EXECUTABLE_NAME} PRIVATE --coverage)
        target_link_libraries(${TEST_EXECUTABLE_NAME} PRIVATE gcov)
    else ()
        message(WARNING "  ⚠️ Unsupported compiler for test executable ${TEST_EXECUTABLE_NAME}")
    endif ()
    target_link_libraries(${TEST_EXECUTABLE_NAME} PRIVATE gtest_main)
    add_test(NAME ${TEST_EXECUTABLE_NAME}
            COMMAND ${TEST_EXECUTABLE_NAME})
endfunction()