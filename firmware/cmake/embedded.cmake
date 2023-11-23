include("${CMAKE_SOURCE_DIR}/firmware/cmake/shared.cmake")

# SEGGER SystemView library, enables CPU profiling with a J-Link dongle.
set(SYSTEMVIEW_DIR "${THIRD_PARTY_DIR}/SEGGER_SystemView_Src")

# FreeRTOS patch to enable thread-safe malloc (so we can use the heap with FreeRTOS).
set(NEWLIB_DIR "${THIRD_PARTY_DIR}/newlib_freertos_patch")

if(NOT STM32CUBEMX_BIN_PATH)
    if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
        set(STM32CUBEMX_BIN_PATH "C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeMX/STM32CubeMX.exe")
    elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
        set(STM32CUBEMX_BIN_PATH "/Applications/STMicroelectronics/STM32CubeMX.app/Contents/MacOs/STM32CubeMX")
    else()
        set(STM32CUBEMX_BIN_PATH "/usr/local/STM32CubeMX/STM32CubeMX")
    endif()
endif()

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

function(embedded_library
    LIB_NAME
    LIB_SRCS
    LIB_INCLUDE_DIRS
    ARM_CORE
    THIRD_PARTY
)
    add_library(${LIB_NAME} STATIC ${LIB_SRCS})

    if(THIRD_PARTY)
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
    elseif()
        target_include_directories(${LIB_NAME}
            PUBLIC
            ${LIB_INCLUDE_DIRS}
        )
    endif()

    set(COMPILER_DEFINES ${SHARED_COMPILER_DEFINES})
    set(COMPILER_FLAGS ${SHARED_COMPILER_FLAGS})
    set(LINKER_FLAGS ${SHARED_LINKER_FLAGS})

    if("${ARM_CORE}" STREQUAL "cm4")
        list(APPEND COMPILER_DEFINES ${CM4_DEFINES})
        list(APPEND COMPILER_FLAGS ${CM4_FPU_FLAGS})
        list(APPEND LINKER_FLAGS ${CM4_FPU_FLAGS})
    elseif("${ARM_CORE}" STREQUAL "cm7")
        list(APPEND COMPILER_DEFINES ${CM7_DEFINES})
        list(APPEND COMPILER_FLAGS ${CM7_FPU_FLAGS})
        list(APPEND LINKER_FLAGS ${CM7_FPU_FLAGS})
    endif()

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

function(embedded_binary
    BIN_NAME
    BIN_SRCS
    BIN_INCLUDE_DIRS
    LINKER_SCRIPT
    ARM_CORE
)
    message("➕ Creating Embedded Target for ${BIN_NAME}")
    set(ELF_NAME "${BIN_NAME}.elf")
    add_executable(${ELF_NAME} ${BIN_SRCS})

    target_include_directories(${ELF_NAME}
        PRIVATE
        ${BIN_INCLUDE_DIRS}
    )

    set(COMPILER_DEFINES ${SHARED_COMPILER_DEFINES})
    set(COMPILER_FLAGS ${SHARED_COMPILER_FLAGS})
    set(LINKER_FLAGS ${SHARED_LINKER_FLAGS})

    if("${ARM_CORE}" STREQUAL "cm4")
        list(APPEND COMPILER_DEFINES ${CM4_DEFINES})
        list(APPEND COMPILER_FLAGS ${CM4_FPU_FLAGS})
        list(APPEND LINKER_FLAGS ${CM4_FPU_FLAGS})
    elseif("${ARM_CORE}" STREQUAL "cm7")
        list(APPEND COMPILER_DEFINES ${CM7_DEFINES})
        list(APPEND COMPILER_FLAGS ${CM7_FPU_FLAGS})
        list(APPEND LINKER_FLAGS ${CM7_FPU_FLAGS})
    endif()

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
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${BIN_NAME}.map
        -Wl,-gc-sections,--print-memory-usage
        -Wl,-T ${LINKER_SCRIPT}
        --specs=nano.specs
    )

    set(HEX_FILE "${BIN_NAME}.hex")
    set(ASM_FILE "${BIN_NAME}.asm")
    set(HEX_PATH "${CMAKE_CURRENT_BINARY_DIR}/${HEX_FILE}")
    set(ASM_PATH "${CMAKE_CURRENT_BINARY_DIR}/${ASM_FILE}")
    add_custom_target(${HEX_FILE} ALL DEPENDS ${HEX_PATH})
    add_custom_target(${ASM_FILE} ALL DEPENDS ${ASM_PATH})

    add_custom_command(TARGET ${BIN_NAME} POST_BUILD
    COMMAND ${PYTHON_COMMAND} ${SCRIPTS_DIR}/clang_format/fix_formatting.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Formatting Source Files")

    # objcoopy is used to create a hex, and assembly file from the elf.
    add_custom_command(
        OUTPUT ${HEX_PATH}
        COMMAND ${CMAKE_OBJCOPY}
        -Oihex ${CMAKE_CURRENT_BINARY_DIR}/${ELF_NAME} ${HEX_PATH}
        DEPENDS ${ELF_NAME}
        COMMENT "Building ${HEX_FILE}"
    )
    add_custom_command(
        OUTPUT ${ASM_PATH}
        COMMAND ${CMAKE_OBJDUMP}
        -DS ${CMAKE_CURRENT_BINARY_DIR}/${ELF_NAME} > ${ASM_PATH}
        DEPENDS ${ELF_NAME}
        COMMENT "Building ${ASM_FILE}"
    )
endfunction()

# Generate firmware image package (merged app + bootloader).
function(embedded_image
    IMAGE_NAME
    APP_HEX_TARGET
    APP_HEX_PATH
    BOOT_HEX_TARGET
    BOOT_HEX_PATH
)
    message("🖼️ Creating Embedded Image for ${IMAGE_NAME}")

    set(APP_METADATA_HEX "${IMAGE_NAME}_app_metadata.hex")
    set(IMAGE_HEX "${IMAGE_NAME}.hex")
    set(APP_METADATA_HEX_PATH "${CMAKE_CURRENT_BINARY_DIR}/${APP_METADATA_HEX}")
    set(IMAGE_HEX_PATH "${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_HEX}")

    add_custom_target(${APP_METADATA_HEX} ALL
        DEPENDS ${APP_METADATA_HEX_PATH}
    )
    add_custom_target(${IMAGE_HEX} ALL
        DEPENDS ${IMAGE_HEX_PATH} 
    )

    set(GENERATE_IMAGE_SCRIPT "${SCRIPTS_DIR}/utilities/generate_image.py")
    add_custom_command(
        OUTPUT ${APP_METADATA_HEX_PATH} ${IMAGE_HEX_PATH}
        COMMAND ${PYTHON_COMMAND} ${GENERATE_IMAGE_SCRIPT}
        --app-hex ${APP_HEX_PATH}
        --boot-hex ${BOOT_HEX_PATH}
        --app-metadata-hex-out ${APP_METADATA_HEX_PATH}
        --image-hex-out ${IMAGE_HEX_PATH}
        DEPENDS ${GENERATE_IMAGE_SCRIPT} ${APP_HEX_PATH} ${BOOT_HEX_PATH}
        WORKING_DIRECTORY ${REPO_ROOT_DIR}
    )
    add_dependencies(${APP_METADATA_HEX} ${APP_HEX_TARGET} ${BOOT_HEX_TARGET})
    add_dependencies(${IMAGE_HEX} ${APP_HEX_TARGET} ${BOOT_HEX_TARGET})
endfunction()

# Generate STM32CubeMX driver code for TARGET_NAME using the given IOC_PATH in
# the directory where this function is called from.
function(generate_stm32cube_code
    TARGET_NAME
    IOC_PATH
)
    set(GENERATE_CUBE_CODE_SCRIPT_PY
        ${SCRIPTS_DIR}/utilities/generate_cube_code.py)
    set(FIX_FORMATTING_SCRIPT_PY
        ${SCRIPTS_DIR}/clang_format/fix_formatting.py)
    set(LOG4J_PROPERTIES "$ENV{HOME}/.stm32cubemx/log4j.properties")
    get_filename_component(IOC_DIR ${IOC_PATH} DIRECTORY)

    add_custom_command(
        OUTPUT ${IOC_PATH}.md5
        ${LOG4J_PROPERTIES}
        COMMENT "Generating drivers for ${TARGET_NAME}"
        COMMAND ${PYTHON_COMMAND}
        ${GENERATE_CUBE_CODE_SCRIPT_PY}
        --board ${TARGET_NAME}
        --log4j_properties_output ${LOG4J_PROPERTIES}
        --ioc ${IOC_PATH}
        --codegen_output_dir ${IOC_DIR}
        --cube_bin ${STM32CUBEMX_BIN_PATH}

        # Create a MD5 hash of IOC_PATH for other build targets to depend on, so
        # this custom command is only executed when IOC_PATH.md5 changes.
        COMMAND ${PYTHON_COMMAND}
        ${SCRIPTS_DIR}/utilities/generate_md5_checksum.py
        ${IOC_PATH}
        ${IOC_PATH}.md5
        DEPENDS ${IOC_PATH}

        # Run clang-format because the STM32CubeMX-generated code is not
        # compliant to our clang-format rules.
        COMMAND ${PYTHON_COMMAND}
        ${FIX_FORMATTING_SCRIPT_PY}
        WORKING_DIRECTORY ${REPO_ROOT_DIR}
    )
endfunction()

function(stm32f412rx_cube_library
    HAL_LIB_NAME
    HAL_CONF_DIR
    HAL_SRCS
    SYSCALLS
    IOC_CHECKSUM
)
    set(DRIVERS_DIR "${STM32CUBEF4_SOURCE_DIR}/Drivers")
    set(FREERTOS_DIR "${STM32CUBEF4_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source")

    # Set include directories for STM32Cube library.
    set(STM32CUBE_INCLUDE_DIRS
        "${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc"
        "${DRIVERS_DIR}/CMSIS/Include"
        "${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy"
        "${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Include"
        "${FREERTOS_DIR}/include"
        "${FREERTOS_DIR}/CMSIS_RTOS_V2"
        "${FREERTOS_DIR}/portable/GCC/ARM_CM4F"
        "${HAL_CONF_DIR}"
        "${SYSTEMVIEW_DIR}/SEGGER"
        "${SYSTEMVIEW_DIR}/Config"
        "${SYSTEMVIEW_DIR}/Sample/FreeRTOSV10"
    )

    # HAL sources.
    set(STM32_HAL_SRCS)

    foreach(HAL_SRC ${HAL_SRCS})
        list(APPEND STM32_HAL_SRCS "${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/${HAL_SRC}")
    endforeach()

    # FreeRTOS sources.
    file(GLOB RTOS_SRCS
        "${FREERTOS_DIR}/*.c"
        "${FREERTOS_DIR}/CMSIS_RTOS_V2/*.c"
        "${FREERTOS_DIR}/portable/GCC/ARM_CM4F/*.c"
    )

    # SEGGER SystemView sources.
    file(GLOB SYSTEMVIEW_SRCS
        "${SYSTEMVIEW_DIR}/SEGGER/*.c"
        "${SYSTEMVIEW_DIR}/SEGGER/*.S"
    )

    # We use ARM's embedded GCC compiler, so append the GCC-specific SysCalls.
    list(APPEND SYSTEMVIEW_SRCS "${SYSTEMVIEW_DIR}/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c")

    # Append the FreeRTOS patch to get SystemView to work with FreeRTOS. All of our boards use FreeRTOS 10.3.1.
    file(GLOB_RECURSE SYSTEMVIEW_FREERTOS_SRCS "${SYSTEMVIEW_DIR}/Sample/FreeRTOSV10/*.c")
    list(APPEND SYSTEMVIEW_SRCS ${SYSTEMVIEW_FREERTOS_SRCS})

    # newlib_freertos_patch adds thread-safe malloc so we can use the heap and FreeRTOS.
    file(GLOB_RECURSE NEWLIB_SRCS "${NEWLIB_DIR}/*.c")

    # Startup assembly script.
    set(STARTUP_SRC "${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f412rx.s")

    set(STM32CUBE_SRCS ${STM32_HAL_SRCS} ${RTOS_SRCS} ${SYSTEMVIEW_SRCS} ${SYSCALLS} ${NEWLIB_SRCS} ${IOC_CHECKSUM} ${STARTUP_SRC})
    embedded_library(
        "${HAL_LIB_NAME}"
        "${STM32CUBE_SRCS}"
        "${STM32CUBE_INCLUDE_DIRS}"
        "cm4"
        TRUE
    )
    target_compile_definitions(${HAL_LIB_NAME}
        PUBLIC
        -DSTM32F412Rx
    )
endfunction()

function(stm32h733xx_cube_library
    HAL_LIB_NAME
    HAL_CONF_DIR
    HAL_SRCS
    SYSCALLS
    IOC_CHECKSUM
)
    set(DRIVERS_DIR "${STM32CUBEH7_SOURCE_DIR}/Drivers")
    set(FREERTOS_DIR "${STM32CUBEH7_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source")

    # Set include directories for STM32Cube library.
    set(STM32CUBE_INCLUDE_DIRS
        "${DRIVERS_DIR}/STM32H7xx_HAL_Driver/Inc"
        "${DRIVERS_DIR}/CMSIS/Include"
        "${DRIVERS_DIR}/STM32H7xx_HAL_Driver/Inc/Legacy"
        "${DRIVERS_DIR}/CMSIS/Device/ST/STM32H7xx/Include"
        "${FREERTOS_DIR}/include"
        "${FREERTOS_DIR}/CMSIS_RTOS_V2"
        "${FREERTOS_DIR}/portable/GCC/ARM_CM4F"
        "${HAL_CONF_DIR}"
        "${SYSTEMVIEW_DIR}/SEGGER"
        "${SYSTEMVIEW_DIR}/Config"
        "${SYSTEMVIEW_DIR}/Sample/FreeRTOSV10"
    )
    # HAL sources.
    set(STM32_HAL_SRCS)
    foreach(HAL_SRC ${HAL_SRCS})
        list(APPEND STM32_HAL_SRCS "${DRIVERS_DIR}/STM32H7xx_HAL_Driver/Src/${HAL_SRC}")
    endforeach()

    # FreeRTOS sources.
    file(GLOB RTOS_SRCS
        "${FREERTOS_DIR}/*.c"
        "${FREERTOS_DIR}/CMSIS_RTOS_V2/*.c"
        "${FREERTOS_DIR}/portable/GCC/ARM_CM4F/*.c"
    )
    
    # SEGGER SystemView sources.
    file(GLOB SYSTEMVIEW_SRCS 
        "${SYSTEMVIEW_DIR}/SEGGER/*.c" 
        "${SYSTEMVIEW_DIR}/SEGGER/*.S"
    )
    # We use ARM's embedded GCC compiler, so append the GCC-specific SysCalls.
    list(APPEND SYSTEMVIEW_SRCS "${SYSTEMVIEW_DIR}/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c")
    # Append the FreeRTOS patch to get SystemView to work with FreeRTOS. All of our boards use FreeRTOS 10.3.1. 
    file(GLOB_RECURSE SYSTEMVIEW_FREERTOS_SRCS "${SYSTEMVIEW_DIR}/Sample/FreeRTOSV10/*.c")
    list(APPEND SYSTEMVIEW_SRCS ${SYSTEMVIEW_FREERTOS_SRCS})

    # newlib_freertos_patch adds thread-safe malloc so we can use the heap and FreeRTOS.
    file(GLOB_RECURSE NEWLIB_SRCS "${NEWLIB_DIR}/*.c")

    # Startup assembly script.
    set(STARTUP_SRC "${DRIVERS_DIR}/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/startup_stm32h733xx.s")
    
    set(STM32CUBE_SRCS ${STM32_HAL_SRCS} ${RTOS_SRCS} ${SYSTEMVIEW_SRCS} ${SYSCALLS} ${NEWLIB_SRCS} ${IOC_CHECKSUM} ${STARTUP_SRC})
    embedded_library(
        "${HAL_LIB_NAME}"
        "${STM32CUBE_SRCS}"
        "${STM32CUBE_INCLUDE_DIRS}"
        "cm7"
        TRUE
    )
    target_compile_definitions(${HAL_LIB_NAME}
        PUBLIC
        -DSTM32H733xx
    )
endfunction()

