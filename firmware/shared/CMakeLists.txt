if("${PLATFORM}" STREQUAL "x86")
    list(APPEND HDRS_TO_FAKE 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_time.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_led.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_rgbLed.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_switch.h"
    )
    set(SHARED_FAKES_INCLUDE_DIRS "${SHARED_APP_INCLUDE_DIRS}")
    create_fake_library(
        "shared_fakes"
        "${HDRS_TO_FAKE}"
        "${SHARED_FAKES_INCLUDE_DIRS}"
    )
endif()