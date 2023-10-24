set(STM32F4_CUBE_ROOT ${CMAKE_SOURCE_DIR}/firmware/third_party/STM32CubeF4)
set(SYSTEMVIEW_ROOT ${CMAKE_SOURCE_DIR}/firmware/third_party/SEGGER_SystemView_Src)
set(NEWLIB_ROOT ${CMAKE_SOURCE_DIR}/firmware/third_party/newlib_freertos_patch)

if(NOT STM32CUBEMX_BIN_PATH)
    # Default STM32CubeMX path
    if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
        set(STM32CUBEMX_BIN_PATH "C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeMX/STM32CubeMX.exe")
    elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
        set(STM32CUBEMX_BIN_PATH "/Applications/STMicroelectronics/STM32CubeMX.app/Contents/MacOs/STM32CubeMX")
    else()
        set(STM32CUBEMX_BIN_PATH "/usr/local/STM32CubeMX/STM32CubeMX")
    endif()
endif()

set(FPU_FLAGS
    -mcpu=cortex-m4
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
)
set(COMPILER_FLAGS
    ${FPU_FLAGS}
    -mthumb
    -mthumb-interwork
    -ffunction-sections
    -fdata-sections
    -g3
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
set(COMPILER_DEFINES
    -D__weak=__attribute__\(\(weak\)\)
    -D__packed=__attribute__\(\(__packed__\)\)
    -DUSE_HAL_DRIVER
    -DARM_MATH_CM4
    -DARM_MATH_MATRIX_CHECK
    -DARM_MATH_ROUNDING
)

function(compile_embedded_lib
    LIB_NAME
    LIB_SRCS
    LIB_INCLUDE_DIRS
    STM32_MCU
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
    
    target_compile_definitions(${LIB_NAME}
        PRIVATE
        ${COMPILER_DEFINES}
        -D${STM32_MCU}
    )
    target_compile_options(${LIB_NAME}
        PRIVATE
        ${COMPILER_FLAGS}
    )
    target_link_options(${LIB_NAME}
        PRIVATE
        ${FPU_FLAGS}
        -Wl,-gc-sections,--print-memory-usage
        --specs=nano.specs
    )
endfunction()

function(compile_embedded_binary
    BIN_NAME
    BIN_SRCS
    BIN_INCLUDE_DIRS
    STM32_MCU
    LINKER_SCRIPT
)
    message("➕ Creating Arm Target for ${BIN_NAME}")
    add_executable(${BIN_NAME} ${BIN_SRCS})

    # this makes it so that you don't have to include each file with a full path
    target_include_directories(${BIN_NAME}
        PRIVATE
        ${BIN_INCLUDE_DIRS}
    )

    target_compile_definitions(${BIN_NAME}
        PRIVATE
        ${COMPILER_DEFINES}
        -D${STM32_MCU}
    )
    target_compile_options(${BIN_NAME}
        PUBLIC
        ${COMPILER_FLAGS}
    )
    target_link_options(${BIN_NAME}
        PUBLIC
        ${FPU_FLAGS}
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${BIN_NAME}.map
        -Wl,-gc-sections,--print-memory-usage
        -Wl,-T ${LINKER_SCRIPT}
        --specs=nano.specs
    )

    set(HEX_FILE "${BIN_NAME}.hex")
    set(BIN_FILE "${BIN_NAME}.bin")
    set(ASM_FILE "${BIN_NAME}.asm")

    # objcoopy is used to create a binary, hex, and assembly file from the elf
    add_custom_command(TARGET ${BIN_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY}
        -Oihex $<TARGET_FILE:${BIN_NAME}> $<TARGET_FILE_DIR:${BIN_NAME}>/${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY}
        -Obinary $<TARGET_FILE:${BIN_NAME}> $<TARGET_FILE_DIR:${BIN_NAME}>/${BIN_FILE}
        COMMAND ${CMAKE_OBJDUMP}
        -DS $<TARGET_FILE:${BIN_NAME}> > $<TARGET_FILE_DIR:${BIN_NAME}>/${ASM_FILE}
        COMMENT "
Building ${HEX_FILE}
Building ${BIN_FILE}
Building ${ASM_FILE}")
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

function(stm32f4_cube_library
    HAL_LIB_NAME
    HAL_CONF_DIR
    HAL_SRCS
    SYSCALLS
    IOC_CHECKSUM
    STM32_MCU
)
    set(DRIVERS_DIR "${STM32F4_CUBE_ROOT}/Drivers")
    set(FREERTOS_DIR "${STM32F4_CUBE_ROOT}/Middlewares/Third_Party/FreeRTOS/Source")

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
        "${SYSTEMVIEW_ROOT}/SEGGER"
        "${SYSTEMVIEW_ROOT}/Config"
        "${SYSTEMVIEW_ROOT}/Sample/FreeRTOSV10"
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
        "${SYSTEMVIEW_ROOT}/SEGGER/*.c" 
        "${SYSTEMVIEW_ROOT}/SEGGER/*.S"
    )
    # We use ARM's embedded GCC compiler, so append the GCC-specific SysCalls.
    list(APPEND SYSTEMVIEW_SRCS "${SYSTEMVIEW_ROOT}/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c")
    # Append the FreeRTOS patch to get SystemView to work with FreeRTOS. All of our STM32F412x boards use FreeRTOS V10. 
    # Note that the GSM currently has an STM32F302x MCU, which is limited to FreeRTOS V9, so SystemView is not supported
    # on the GSM.
    file(GLOB_RECURSE SYSTEMVIEW_FREERTOS_SRCS "${SYSTEMVIEW_ROOT}/Sample/FreeRTOSV10/*.c")
    list(APPEND SYSTEMVIEW_SRCS ${SYSTEMVIEW_FREERTOS_SRCS})

    # newlib_freertos_patch adds thread-safe malloc so we can use the heap and FreeRTOS.
    file(GLOB_RECURSE NEWLIB_SRCS "${NEWLIB_ROOT}/*.c")

    # Startup assembly script.
    set(STARTUP_SRC "${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f412rx.s")
    
    set(STM32CUBE_SRCS ${STM32_HAL_SRCS} ${RTOS_SRCS} ${SYSTEMVIEW_SRCS} ${SYSCALLS} ${NEWLIB_SRCS} ${IOC_CHECKSUM} ${STARTUP_SRC})
    compile_embedded_lib(
        "${HAL_LIB_NAME}"
        "${STM32CUBE_SRCS}"
        "${STM32CUBE_INCLUDE_DIRS}"
        "${STM32_MCU}"
        TRUE
    )
endfunction()
