IF (NOT "${SHARED_CMAKE_INCLUDED}" STREQUAL "TRUE")
    message(FATAL_ERROR "❌ shared.cmake must be included before embedded.cmake")
ENDIF ()
IF (NOT "${STM32LIB_CMAKE_INCLUDED}" STREQUAL "TRUE")
    message(FATAL_ERROR "❌ stm32lib.cmake must be included before embedded.cmake")
ENDIF ()
message("")
message("💽 [embedded.cmake] Configuring Embedded Build")
set(EMBEDDED_CMAKE_INCLUDED TRUE)

# ===== OPTIONS =====
option(BUILD_ASM "Build the assembly files" OFF)

# STM32CUBEMX Binary Path
IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(STM32CUBEMX_BIN_PATH "C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeMX/STM32CubeMX.exe")
ELSEIF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    set(STM32CUBEMX_BIN_PATH "/Applications/STMicroelectronics/STM32CubeMX.app/Contents/MacOs/STM32CubeMX")
ELSEIF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(STM32CUBEMX_BIN_PATH "/usr/local/STM32CubeMX/STM32CubeMX")
ELSE ()
    message(FATAL_ERROR "❌ Unsupported host system: ${CMAKE_HOST_SYSTEM_NAME}")
ENDIF ()

set(SHARED_COMPILER_DEFINES
        -D__weak=__attribute__\(\(weak\)\)
        -D__packed=__attribute__\(\(__packed__\)\)
        -DUSE_HAL_DRIVER
        -DARM_MATH_MATRIX_CHECK
        -DARM_MATH_ROUNDING
)
set(SHARED_COMPILER_FLAGS
        -mthumb
        -mthumb-interwork
        -ffunction-sections
        -fdata-sections
        -g3
        -O0
        -fno-common
        -fmessage-length=0
        -Wall
        -Werror
        -Wextra
        -pedantic
        -specs=nosys.specs
        -specs=nano.specs
        -Wdouble-promotion
        -Wshadow
        -Wundef
        -fstack-usage
        -Wconversion
        -Wno-unused-variable
        -Wno-unused-parameter
)
set(SHARED_LINKER_FLAGS
        -Wl,-gc-sections,--print-memory-usage
        -L${FIRMWARE_DIR}/linker
        --specs=nano.specs
)

set(CM4_DEFINES
        -DARM_MATH_CM4
)
set(CM4_FPU_FLAGS
        -mcpu=cortex-m4
        -mfloat-abi=hard
        -mfpu=fpv4-sp-d16
)

set(CM7_DEFINES
        -DARM_MATH_CM7
)
set(CM7_FPU_FLAGS
        -mcpu=cortex-m4
        -mfloat-abi=hard
        -mfpu=fpv5-d16
)

message("  🔃 Registered embedded_library() function")
function(embedded_library
        LIB_NAME
        LIB_SRCS
        LIB_INCLUDE_DIRS
        ARM_CORE
        THIRD_PARTY
)
    add_library(${LIB_NAME} STATIC ${LIB_SRCS})

    IF (THIRD_PARTY)
        # Suppress header file warnings for third-party code by marking them as system includes.
        target_include_directories(${LIB_NAME} SYSTEM
                PUBLIC
                ${LIB_INCLUDE_DIRS}
        )

        # Suppress source file warnings for third-party code.
        set_source_files_properties(
                ${LIB_SRCS}
                PROPERTIES COMPILE_FLAGS "-w"
        )
    ELSEIF ()
        target_include_directories(${LIB_NAME}
                PUBLIC
                ${LIB_INCLUDE_DIRS}
        )
    ENDIF ()

    set(COMPILER_DEFINES ${SHARED_COMPILER_DEFINES})
    set(COMPILER_FLAGS ${SHARED_COMPILER_FLAGS})
    set(LINKER_FLAGS ${SHARED_LINKER_FLAGS})

    IF ("${ARM_CORE}" STREQUAL "cm4")
        list(APPEND COMPILER_DEFINES ${CM4_DEFINES})
        list(APPEND COMPILER_FLAGS ${CM4_FPU_FLAGS})
        list(APPEND LINKER_FLAGS ${CM4_FPU_FLAGS})
    ELSEIF ("${ARM_CORE}" STREQUAL "cm7")
        list(APPEND COMPILER_DEFINES ${CM7_DEFINES})
        list(APPEND COMPILER_FLAGS ${CM7_FPU_FLAGS})
        list(APPEND LINKER_FLAGS ${CM7_FPU_FLAGS})
    ENDIF ()

    target_compile_definitions(${LIB_NAME}
            PRIVATE
            ${COMPILER_DEFINES}
    )
    target_compile_options(${LIB_NAME}
            PRIVATE
            ${COMPILER_FLAGS}
    )
    target_link_options(${LIB_NAME}
            PRIVATE
            ${LINKER_FLAGS}
    )
endfunction()

message("  🔃 Registered embedded_binary() function")
# Generate an embedded binary target, a hex file, and an optional assembly file.
function(embedded_binary
        BIN_NAME
        BIN_SRCS
        BIN_INCLUDE_DIRS
        LINKER_SCRIPT
        ARM_CORE
)
    message("  ➕ [embedded.cmake, embedded_binary()] Creating Embedded Target for ${BIN_NAME}")
    set(ELF_NAME "${BIN_NAME}.elf")
    add_executable(${ELF_NAME} ${BIN_SRCS})

    target_include_directories(${ELF_NAME}
            PRIVATE
            ${BIN_INCLUDE_DIRS}
    )

    set(COMPILER_DEFINES ${SHARED_COMPILER_DEFINES})
    set(COMPILER_FLAGS ${SHARED_COMPILER_FLAGS})
    set(LINKER_FLAGS ${SHARED_LINKER_FLAGS})

    IF ("${ARM_CORE}" STREQUAL "cm4")
        list(APPEND COMPILER_DEFINES ${CM4_DEFINES})
        list(APPEND COMPILER_FLAGS ${CM4_FPU_FLAGS})
        list(APPEND LINKER_FLAGS ${CM4_FPU_FLAGS})
    ELSEIF ("${ARM_CORE}" STREQUAL "cm7")
        list(APPEND COMPILER_DEFINES ${CM7_DEFINES})
        list(APPEND COMPILER_FLAGS ${CM7_FPU_FLAGS})
        list(APPEND LINKER_FLAGS ${CM7_FPU_FLAGS})
    ENDIF ()

    target_compile_definitions(${ELF_NAME}
            PRIVATE
            ${COMPILER_DEFINES}
    )
    target_compile_options(${ELF_NAME}
            PRIVATE
            ${COMPILER_FLAGS}
    )
    target_link_options(${ELF_NAME}
            PRIVATE
            ${LINKER_FLAGS}
            # binary specific linker flags
            -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${BIN_NAME}.map
            -Wl,-gc-sections,--print-memory-usage
            -Wl,-T ${LINKER_SCRIPT}
            --specs=nano.specs
    )

    # 2) Hex file generation
    set(HEX_FILE "${BIN_NAME}.hex")
    set(HEX_PATH "${CMAKE_CURRENT_BINARY_DIR}/${HEX_FILE}")
    # objcopy is used to create a hex, and assembly file from the elf.
    add_custom_target(${HEX_FILE} ALL
            COMMENT "[Binary] Building ${HEX_FILE}"
            COMMAND ${CMAKE_OBJCOPY} -Oihex ${CMAKE_CURRENT_BINARY_DIR}/${ELF_NAME} ${HEX_PATH}
            DEPENDS ${ELF_NAME}
    )

    # 3?) ASM File Generation
    IF (${BUILD_ASM})
        set(ASM_FILE "${BIN_NAME}.asm")
        set(ASM_PATH "${CMAKE_CURRENT_BINARY_DIR}/${ASM_FILE}")
        add_custom_target(${ASM_FILE} ALL
                COMMENT " [Binary] Building ${ASM_FILE}"
                COMMAND ${CMAKE_OBJDUMP} -DS ${CMAKE_CURRENT_BINARY_DIR}/${ELF_NAME} > ${ASM_PATH}
                DEPENDS ${ELF_NAME}
        )
    ENDIF ()
endfunction()

message("  🔃 Registered embedded_image() function")
# Generate firmware image package (merged app + bootloader).
function(embedded_image
        IMAGE_NAME
        APP_HEX_TARGET
        APP_HEX_PATH
        BOOT_HEX_TARGET
        BOOT_HEX_PATH
)
    message("  🖼️ [embedded.cmake, embedded_image()] Creating Embedded Image for ${IMAGE_NAME}")

    set(APP_METADATA_HEX "${IMAGE_NAME}_app_metadata.hex")
    set(IMAGE_HEX "${IMAGE_NAME}.hex")
    set(APP_METADATA_HEX_PATH "${CMAKE_CURRENT_BINARY_DIR}/${APP_METADATA_HEX}")
    set(IMAGE_HEX_PATH "${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_HEX}")

    set(GENERATE_IMAGE_SCRIPT "${SCRIPTS_DIR}/utilities/generate_image.py")
    add_custom_target(${IMAGE_HEX} ALL
            COMMENT "[Image] Building ${IMAGE_HEX} and ${APP_METADATA_HEX}"
            COMMAND ${PYTHON_COMMAND} ${GENERATE_IMAGE_SCRIPT}
            --app-hex ${APP_HEX_PATH}
            --boot-hex ${BOOT_HEX_PATH}
            --app-metadata-hex-out ${APP_METADATA_HEX_PATH}
            --image-hex-out ${IMAGE_HEX_PATH}
            WORKING_DIRECTORY ${REPO_ROOT_DIR}
            DEPENDS ${GENERATE_IMAGE_SCRIPT} ${APP_HEX_PATH} ${BOOT_HEX_PATH}
    )
    add_dependencies(${IMAGE_HEX} ${APP_HEX_TARGET} ${BOOT_HEX_TARGET})
endfunction()

message("  🔃 Registered stm32f4_boot_binary() function")
function(stm32f4_boot_binary
        BOOT_NAME
        SRCS
        INCLUDE_DIRS
        CONFIG_DEFINE
        SYSCALLS
        IOC_PATH
)
    generate_stm32cube_code(
            "${BOOT_NAME}_cubegen"
            "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
            "stm32f4xx_hal_can.c"
            "stm32f4xx_hal_crc.c"
            "stm32f4xx_hal_cortex.c"
            "stm32f4xx_hal_exti.c"
            "stm32f4xx_hal_flash.c"
            "stm32f4xx_hal_flash_ex.c"
            "stm32f4xx_hal_gpio.c"
            "stm32f4xx_hal_rcc_ex.c"
            "stm32f4xx_hal_rcc.c"
            "stm32f4xx_hal_tim_ex.c"
            "stm32f4xx_hal_tim.c"
            "stm32f4xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    stm32f412rx_cube_library(
            "${BOOT_NAME}_stm32cube"
            "${INCLUDE_DIRS}"
            "${STM32_HAL_SRCS}"
            "${SYSCALLS}"
            "${MD5_LOCATION}"
    )

    # Add bootloader-specific files.
    list(APPEND SRCS "${BOOT_DIR}/bootloader.c" "${BOOT_DIR}/bootloader_f4.c")
    list(APPEND INCLUDE_DIRS "${BOOT_DIR}")

    # Add shared files.
    list(APPEND SRCS
            "${SHARED_IO_INCLUDE_DIR}/io_can.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_flash.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_can.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
    )
    list(APPEND INCLUDE_DIRS
            "${SHARED_APP_INCLUDE_DIR}"
            "${SHARED_IO_INCLUDE_DIR}"
            "${SHARED_HW_INCLUDE_DIR}"
    )

    set(ARM_CORE "cm4")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_boot.ld")
    embedded_binary(
            "${BOOT_NAME}"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )
    target_link_libraries("${BOOT_NAME}.elf" "${BOOT_NAME}_stm32cube")
    target_compile_definitions("${BOOT_NAME}.elf" PRIVATE "${CONFIG_DEFINE}")
endfunction()

message("  🔃 Registered stm32h7_boot_binary() function")
function(stm32h7_boot_binary
        BOOT_NAME
        SRCS
        INCLUDE_DIRS
        CONFIG_DEFINE
        SYSCALLS
        IOC_PATH
)
    generate_stm32cube_code(
            "${BOOT_NAME}_cubegen"
            "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
            "stm32h7xx_hal_crc.c"
            "stm32h7xx_hal_crc_ex.c"
            "stm32h7xx_hal_cortex.c"
            "stm32h7xx_hal_dma_ex.c"
            "stm32h7xx_hal_dma.c"
            "stm32h7xx_hal_exti.c"
            "stm32h7xx_hal_fdcan.c"
            "stm32h7xx_hal_flash.c"
            "stm32h7xx_hal_flash_ex.c"
            "stm32h7xx_hal_gpio.c"
            "stm32h7xx_hal_iwdg.c"
            "stm32h7xx_hal_pwr_ex.c"
            "stm32h7xx_hal_rcc_ex.c"
            "stm32h7xx_hal_rcc.c"
            "stm32h7xx_hal_tim_ex.c"
            "stm32h7xx_hal_tim.c"
            "stm32h7xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    stm32h733xx_cube_library(
            "${BOOT_NAME}_stm32cube"
            "${INCLUDE_DIRS}"
            "${STM32_HAL_SRCS}"
            "${SYSCALLS}"
            "${MD5_LOCATION}"
    )

    # Add bootloader-specific files.
    list(APPEND SRCS "${BOOT_DIR}/bootloader.c" "${BOOT_DIR}/bootloader_h7.c")
    list(APPEND INCLUDE_DIRS "${BOOT_DIR}")

    # Add shared files.
    list(APPEND SRCS
            "${SHARED_IO_INCLUDE_DIR}/io_can.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_flash.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_fdcan.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
    )
    list(APPEND INCLUDE_DIRS
            "${SHARED_APP_INCLUDE_DIR}"
            "${SHARED_IO_INCLUDE_DIR}"
            "${SHARED_HW_INCLUDE_DIR}"
    )

    set(ARM_CORE "cm7")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_boot.ld")
    embedded_binary(
            "${BOOT_NAME}"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )
    target_link_libraries("${BOOT_NAME}.elf" "${BOOT_NAME}_stm32cube")
    target_compile_definitions("${BOOT_NAME}.elf" PRIVATE "${CONFIG_DEFINE}")
endfunction()