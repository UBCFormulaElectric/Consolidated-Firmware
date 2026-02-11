message("")
message("⚙️ Configuring Shared")

IF ("${TARGET}" STREQUAL "test")
    set(INCLUDE_DIRS 
        ${SHARED_APP_INCLUDE_DIR} 
        ${SHARED_APP_INCLUDE_DIR_CPP} 
        ${FFF_DIR} 
        ${SHARED_FAKES_DIR}
        ${SHARED_TEST_UTILS_INCLUDE_DIRS} 
        ${SHARED_UTIL_INCLUDE_DIR_CPP} 
        ${SHARED_UTIL_INCLUDE_DIR} 
        ${SHARED_IO_INCLUDE_DIR_CPP}
    )

    set(HDRS_TO_FAKE
        "${SHARED_IO_INCLUDE_DIR}/io_time.h"
        "${SHARED_IO_INCLUDE_DIR}/io_led.h"
        "${SHARED_IO_INCLUDE_DIR}/io_switch.h"
        "${SHARED_IO_INCLUDE_DIR}/io_mechanicalLoad.h"
    )
    set(HDRS_TO_FAKE_CPP
        "${SHARED_IO_INCLUDE_DIR}/io_led.h"
        "${SHARED_IO_INCLUDE_DIR}/io_switch.h"
        "${SHARED_IO_INCLUDE_DIR}/io_mechanicalLoad.h"
    )

    file(GLOB_RECURSE APP_SRCS_C CONFIGURE_DEPENDS "${SHARED_APP_INCLUDE_DIR}/*.c")
    file(GLOB_RECURSE APP_SRCS_CPP CONFIGURE_DEPENDS "${SHARED_APP_INCLUDE_DIR_CPP}/*.cpp")
    file(GLOB_RECURSE TEST_SRCS_C CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c")
    file(GLOB_RECURSE TEST_SRCS_CPP CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

    set(CAN_SUPPORT_SRCS
        "app/app_canTx.c"
        "app/app_canRx.c"
        "app/app_canUtils.c"
        "app/app_canAlerts.c"
        "app/app_canDataCapture.c"
    )

    jsoncan_library("Debug" "quintuna" "${CMAKE_CURRENT_BINARY_DIR}")

    create_fake_library("shared_fakes_c" "${HDRS_TO_FAKE}")
    set(ALL_SRCS_C ${TEST_SRCS_C} ${APP_SRCS_C} ${CAN_SUPPORT_SRCS})
    compile_gtest_executable("shared_test_c" "${ALL_SRCS_C}" "${INCLUDE_DIRS}")
    target_compile_definitions("shared_test_c" PRIVATE STM32H733xx)
    target_link_libraries("shared_test_c" PRIVATE "quintuna_Debug_jsoncan" "shared_fakes_c")

    create_fake_library("shared_fakes_cpp" "${HDRS_TO_FAKE_CPP}")
    set(ALL_SRCS_CPP ${TEST_SRCS_CPP} ${APP_SRCS_CPP} ${APP_SRCS_C} ${CAN_SUPPORT_SRCS} "${SHARED_FAKES_DIR}/io_timeFake.cpp")
    compile_gtest_executable("shared_test_cpp" "${ALL_SRCS_CPP}" "${INCLUDE_DIRS}")
    target_compile_definitions("shared_test_cpp" PRIVATE STM32H733xx)
    target_link_libraries("shared_test_cpp" PRIVATE "quintuna_Debug_jsoncan" "shared_fakes_cpp")

    add_custom_target(shared_test DEPENDS shared_test_c shared_test_cpp)
ENDIF ()