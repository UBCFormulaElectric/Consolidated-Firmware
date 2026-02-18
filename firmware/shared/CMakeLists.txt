message("")
message("⚙️ Configuring Shared")

IF ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE SHARED_APP_SRCS CONFIGURE_DEPENDS "${SHARED_APP_INCLUDE_DIR}/*.c")
    file(GLOB_RECURSE SHARED_APP_SRCS_CPP CONFIGURE_DEPENDS "${SHARED_APP_INCLUDE_DIR_CPP}/*.cpp")
    # shared modules tests
    file(GLOB_RECURSE SHARED_TEST_SRCS CONFIGURE_DEPENDS
            "${SHARED_TEST_UTILS_INCLUDE_DIRS}/*.cpp"
    )
    file(GLOB_RECURSE SHARED_TEST_SRCS_CPP CONFIGURE_DEPENDS
            "${SHARED_TEST_UTILS_INCLUDE_DIRS_CPP}/*.cpp"
            "${SHARED_FAKES_DIR_CPP}/io_time.cpp"
    )

    set(SRCS ${SHARED_TEST_SRCS} ${SHARED_APP_SRCS})
    set(SRCS_CPP ${SHARED_TEST_SRCS_CPP} ${SHARED_APP_SRCS_CPP})
    set(INCLUDE_DIRS ${SHARED_APP_INCLUDE_DIR} ${FFF_DIR} ${SHARED_TEST_UTILS_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR})
    set(INCLUDE_DIRS_CPP ${SHARED_APP_INCLUDE_DIR_CPP} ${SHARED_IO_INCLUDE_DIR_CPP} ${FFF_DIR} ${SHARED_TEST_UTILS_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR_CPP})
    compile_gtest_executable("shared_test" "${SRCS}" "${INCLUDE_DIRS}")
    compile_gtest_executable("shared_test_cpp" "${SRCS_CPP}" "${INCLUDE_DIRS_CPP}")

    # jsoncan lib
    jsoncan_library("Debug" "quintuna" "${CMAKE_CURRENT_BINARY_DIR}")

    # io time :)
    set(HDRS_TO_FAKE
            "${SHARED_IO_INCLUDE_DIR}/io_time.h"
            "${SHARED_IO_INCLUDE_DIR}/io_led.h"
            "${SHARED_IO_INCLUDE_DIR}/io_switch.h"
            "${SHARED_IO_INCLUDE_DIR}/io_mechanicalLoad.h"
    )
    create_fake_library("shared_test_fakes" "${HDRS_TO_FAKE}")

    target_compile_definitions("shared_test" PRIVATE STM32H733xx)
    target_compile_definitions("shared_test_cpp" PRIVATE STM32H733xx)
    target_link_libraries("shared_test" PRIVATE "quintuna_Debug_jsoncan" "shared_test_fakes")
    target_link_libraries("shared_test_cpp" PRIVATE "shared_test_fakes" "eigen_interface" "autodiff_interface")
ENDIF ()