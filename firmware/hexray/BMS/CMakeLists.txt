message("")
message("⚙️ Configuring BMS (Hexray)")

file(GLOB CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

set(JOBS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs.cpp")
set(TASKS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.cpp")
set(SYSTEM_SRCS ${JOBS_SRC} ${TASKS_SRC})
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE APP_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.cpp")
list(APPEND APP_SRCS
        # "${SHARED_APP_INCLUDE_DIR}/app_jsoncan.c"
)
set(APP_INCLUDE_DIRS
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
        "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_DIR_CPP}"
)

file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_time.cpp"
)
set(IO_INCLUDE_DIRS
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_DIR_CPP}"
)

file(GLOB_RECURSE HW_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.cpp")
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_error.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_gpio.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_hardFaultHandler.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_usb.cpp"
)
set(HW_INCLUDE_DIRS
               "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_DIR_CPP}"
)

# chimera sources
set(CHIMERA_SRCS ${HW_SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/src/chimera.cpp")
set(CHIMERA_INCLUDE_DIRS ${HW_INCLUDE_DIRS})

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm7")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app_only.ld")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

    embedded_binary(
            "hexray_BMS_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    set(STM32_HAL_SRCS
        "stm32h7xx_hal.c"
        "stm32h7xx_hal_rcc.c"
        "stm32h7xx_hal_rcc_ex.c"
        "stm32h7xx_hal_pwr_ex.c"
        "stm32h7xx_hal_cortex.c"
        "stm32h7xx_hal_exti.c"
        "stm32h7xx_hal_tim.c"
        "stm32h7xx_hal_gpio.c"
        "stm32h7xx_hal_adc.c"
        "stm32h7xx_hal_dma.c"         
        "stm32h7xx_hal_iwdg.c"

        "stm32h7xx_hal_adc_ex.c"       
        "stm32h7xx_hal_tim_ex.c"       
        "stm32h7xx_hal_dma_ex.c" 
        
        "stm32h7xx_hal_crc.c"        
        "stm32h7xx_hal_crc_ex.c"       
        "stm32h7xx_hal_fdcan.c"       
        "stm32h7xx_hal_spi.c"        
        "stm32h7xx_hal_spi_ex.c"

        "stm32h7xx_hal_sd.c"       
        "stm32h7xx_hal_sd_ex.c"
        "stm32h7xx_ll_sdmmc.c"        

        "stm32h7xx_hal_pcd.c"     
        "stm32h7xx_hal_pcd_ex.c"
        "stm32h7xx_ll_usb.c"       
        
        
    )
    set(CUBEMX_INCLUDES ${CUBEMX_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    stm32h733xx_cube_library(
            "hexray_BMS_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/BMS.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDES}"
            TRUE
            "${ARM_CORE}"
            TRUE
    )

    # JSONCAN
    jsoncan_embedded_library(BMS hexray "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "hexray_BMS_app.elf"
            "hexray_BMS_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("hexray_BMS_app.elf" PRIVATE "hexray_BMS_stm32" "hexray_BMS_jsoncan" "hexray_BMS_commit_info" "m")
    target_compile_definitions("hexray_BMS_app.elf" PRIVATE BMS)

    add_chimera_stm32h7("hexray_BMS_chimera" "${CHIMERA_SRCS}" "${CHIMERA_INCLUDE_DIRS}" "hexray_chimera_v2_proto_cm7")
    target_link_libraries("hexray_BMS_chimera.elf" PRIVATE "hexray_BMS_stm32")
elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    set(SRCS ${APP_SRCS} ${JOBS_SRC} ${TEST_SRCS})
    set(INCLUDE_DIRS
            # "${CMAKE_CURRENT_SOURCE_DIR}/test"
            ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR_CPP}
    )
    compile_gtest_executable(
            "hexray_BMS_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    add_compile_definitions("hexray_BMS_test" PRIVATE STM32h763Rx)

    # JSONCAN
    jsoncan_library(BMS hexray "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
            "hexray_BMS_test"
            "hexray_BMS_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    # FakeIO
    set(HEADERS_TO_FAKE
    )

    file(GLOB TEST_IO_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.h")
    list(APPEND HEADERS_TO_FAKE ${TEST_IO_HDRS})
    create_fake_library(
            "hexray_BMS_fakes"
            "${HEADERS_TO_FAKE}"
    )

    target_link_libraries("hexray_BMS_test" PRIVATE "hexray_BMS_fakes" "hexray_BMS_commit_info")
endif ()
