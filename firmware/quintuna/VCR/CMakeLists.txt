message("")
message("⚙️ Configuring VCR (Quintuna)")

set(ARM_CORE "cm7")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app_only.ld")

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

set(JOBS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs.c")
set(TASKS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.c")
set(SYSTEM_SRCS ${TASKS_SRC} ${JOBS_SRC})
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/app_jsoncan.c")
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_QUINTUNA_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c")

set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_QUINTUNA_DIR}")

file(GLOB_RECURSE HW_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c")
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_ubsan.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_can_h7.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_utils.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_resetReason.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_freeRtosConfigs.c"
)
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SHARED_HW_INCLUDE_DIR}")

if ("${TARGET}" STREQUAL "binary")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})
    embedded_binary("quintuna_VCR_app" "${SRCS}" "${INCLUDE_DIRS}" "${LINKER_SCRIPT}" "${ARM_CORE}")

    # Bootloader files.
    #    file(GLOB BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    #    file(GLOB BOOT_CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
    #    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")

    # Build bootloader.
    #    stm32h7_boot_binary(
    #            "quintuna_VCR_boot"
    #            "${BOOT_SRCS}"
    #            "${BOOT_CUBEMX_SRCS}"
    #            "${BOOT_INCLUDE_DIRS}"
    #            "quintuna_VCR"
    #            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/VCR_boot.ioc"
    #    )

    # STM HAL Library
    set(STM32_HAL_SRCS
            "stm32h7xx_hal_cortex.c"
            "stm32h7xx_hal_fdcan.c"
            "stm32h7xx_hal_gpio.c"
            "stm32h7xx_hal_iwdg.c"
            "stm32h7xx_hal_pwr_ex.c"
            "stm32h7xx_hal_rcc_ex.c"
            "stm32h7xx_hal_rcc.c"
            "stm32h7xx_hal_tim_ex.c"
            "stm32h7xx_hal_tim.c"
            "stm32h7xx_hal.c"
    )
    stm32h733xx_cube_library(
            "quintuna_VCR_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/VCR.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDE_DIRS}"
            FALSE
            "${ARM_CORE}"

    )

    jsoncan_embedded_library(VCR quintuna "${CMAKE_CURRENT_BINARY_DIR}")

    #CommitInfo
    commit_info_library(
            "quintuna_VCR_app.elf"
            "quintuna_VCR_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )
    target_compile_definitions("quintuna_VCR_app.elf" PRIVATE VCR)
    target_link_libraries("quintuna_VCR_app.elf" PRIVATE
            "quintuna_VCR_stm32"
            "quintuna_VCR_jsoncan"
            "quintuna_VCR_commit_info")
    target_compile_definitions("quintuna_VCR_app.elf" PRIVATE NO_BOOTLOADER)

    # embedded_image(
    #         "quintuna_VCR"
    #         "quintuna_VCR_app.hex"
    #         "${CMAKE_CURRENT_BINARY_DIR}/quintuna_VCR_app.hex")
elseif ("${TARGET}" STREQUAL "test")
endif ()
