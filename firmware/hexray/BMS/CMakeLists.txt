message("")
message("⚙️ Configuring BMS (Hexray)")

file(GLOB CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

set(JOBS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs.cpp")
set(TASKS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.cpp")
set(SYSTEM_SRCS ${JOBS_SRC} ${TASKS_SRC})
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        #        "${CMAKE_CURRENT_SOURCE_DIR}/boot"
)

file(GLOB_RECURSE APP_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        # "${SHARED_APP_INCLUDE_DIR}/app_jsoncan.c"
)
set(APP_INCLUDE_DIRS
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
        "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_DIR_CPP}"
)

file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_time.cpp"
)
set(IO_INCLUDE_DIRS
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_DIR_CPP}"
)

file(GLOB_RECURSE HW_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c")
list(APPEND HW_SRCS)
set(HW_INCLUDE_DIRS
        #        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_DIR_CPP}"
)

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

            "stm32h7xx_hal_adc_ex.c"       # Fixes: HAL_ADCEx_MultiModeConfigChannel
        "stm32h7xx_hal_tim_ex.c"       # Fixes: HAL_TIMEx_MasterConfigSynchronization
        "stm32h7xx_hal_dma.c"          # Fixes: HAL_DMA_IRQHandler
        
        # --- Missing Peripherals (From your log) ---
        "stm32h7xx_hal_crc.c"          # Fixes: HAL_CRC_Init
        "stm32h7xx_hal_crc_ex.c"       
        "stm32h7xx_hal_fdcan.c"        # Fixes: HAL_FDCAN_Init
        "stm32h7xx_hal_spi.c"          # Fixes: HAL_SPI_Init
        "stm32h7xx_hal_spi_ex.c"

        # --- Missing SD Card (Must include LL driver) ---
        "stm32h7xx_hal_sd.c"           # Fixes: HAL_SD_Init
        "stm32h7xx_hal_sd_ex.c"
        "stm32h7xx_ll_sdmmc.c"         # REQUIRED: HAL_SD relies on this

        # --- Missing USB (Must include LL driver) ---
        "stm32h7xx_hal_pcd.c"          # Fixes: HAL_PCD_Init
        "stm32h7xx_hal_pcd_ex.c"
        "stm32h7xx_ll_usb.c"           # REQUIRED: HAL_PCD relies on this
    )
    stm32h733xx_cube_library(
            "hexray_BMS_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/BMS.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDE_DIRS}"
            FALSE
            "${ARM_CORE}"
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
