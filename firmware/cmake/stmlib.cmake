message("")
message("🎚️ [stmlib.cmake] Configuring STM32CubeMX functions")
set(STM32LIB_CMAKE_INCLUDED TRUE)

# ==== generate log4j properties for stm32cubemx codegen ====
IF (${CMAKE_HOST_WIN32}) # this is slightly more reliable than WIN32
    set(LOG4J_PROPERTIES "$ENV{UserProfile}/.stm32cubemx/log4j.properties")
ELSE ()
    set(LOG4J_PROPERTIES "$ENV{HOME}/.stm32cubemx/log4j.properties")
ENDIF ()
message("  📝 Generating log4j.properties at ${LOG4J_PROPERTIES}")
execute_process(
        COMMAND ${PYTHON_COMMAND}
        ${SCRIPTS_DIR}/utilities/generate_log4j_properties.py
        --log4j_properties_output ${LOG4J_PROPERTIES}
)

file(GLOB_RECURSE NEWLIB_SRCS "${THIRD_PARTY_DIR}/newlib_freertos_patch/*.c")

# ==== STM32CubeMX functions ====

message("  🔃 Registered generate_stm32cube_code() function")
# Generate STM32CubeMX driver code for TARGET_NAME using the given IOC_PATH in
# the directory where this function is called from.
function(generate_stm32cube_code
        TARGET_NAME
        IOC_PATH
)
    set(GENERATE_CUBE_CODE_SCRIPT_PY ${SCRIPTS_DIR}/utilities/generate_cube_code.py)
    get_filename_component(IOC_DIR ${IOC_PATH} DIRECTORY)
    get_filename_component(IOC_FILE_NAME ${IOC_PATH} NAME)
    set(TRACKED_MD5_LOCATION "${IOC_PATH}.md5")
    set(OUTPUT_MD5_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/${IOC_FILE_NAME}.md5")
    set(MD5_LOCATION ${OUTPUT_MD5_LOCATION} PARENT_SCOPE)

    add_custom_command(
            OUTPUT ${OUTPUT_MD5_LOCATION}
            COMMENT "Generating drivers for ${TARGET_NAME}"
            COMMAND ${PYTHON_COMMAND} ${GENERATE_CUBE_CODE_SCRIPT_PY}
            --board ${TARGET_NAME}
            --ioc ${IOC_PATH}
            --codegen_output_dir ${IOC_DIR}
            --cube_bin ${STM32CUBEMX_BIN_PATH}
            --md5 ${TRACKED_MD5_LOCATION}
            WORKING_DIRECTORY ${REPO_ROOT_DIR}

            COMMAND ${CMAKE_COMMAND} -E copy ${TRACKED_MD5_LOCATION} ${OUTPUT_MD5_LOCATION}

            DEPENDS ${IOC_PATH}
    )
endfunction()

message("  🔃 Registered stm32f412rx_cube_library() function")
# HAL_CONF_DIR: src/cubemx/Inc
# HAL_SRCS: the ones that we want, stripped prefixes
# SYSCALLS: most of the functions defined inside are weak references, only used to make sure it builds without error.
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
            "${HAL_CONF_DIR}"
            "${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc"
            "${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy"
            "${FREERTOS_DIR}/include"
            "${FREERTOS_DIR}/CMSIS_RTOS_V2"
            "${FREERTOS_DIR}/portable/GCC/ARM_CM4F"
            "${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Include"
            "${DRIVERS_DIR}/CMSIS/Include"
            # SEGGER SystemView includes.
            "${THIRD_PARTY_DIR}/sysview"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Config"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Sample/FreeRTOSV10"
    )

    # HAL sources.
    set(STM32_HAL_SRCS)

    foreach (HAL_SRC ${HAL_SRCS})
        list(APPEND STM32_HAL_SRCS "${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/${HAL_SRC}")
    endforeach ()

    # FreeRTOS sources.
    file(GLOB RTOS_SRCS
            "${FREERTOS_DIR}/*.c"
            "${FREERTOS_DIR}/CMSIS_RTOS_V2/cmsis_os2.c"
            "${FREERTOS_DIR}/portable/GCC/ARM_CM4F/port.c"
    )

    # SEGGER SystemView sources.
    file(GLOB SYSTEMVIEW_SRCS
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/*.c"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/*.S"
    )
    # We use ARM's embedded GCC compiler, so append the GCC-specific SysCalls.
    list(APPEND SYSTEMVIEW_SRCS "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c")
    # Append the FreeRTOS patch to get SystemView to work with FreeRTOS. All of our boards use FreeRTOS 10.3.1.
    file(GLOB_RECURSE SYSTEMVIEW_FREERTOS_SRCS "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Sample/FreeRTOSV10/SEGGER_SYSVIEW_FreeRTOS.c")
    list(APPEND SYSTEMVIEW_SRCS ${SYSTEMVIEW_FREERTOS_SRCS})
    # NOTE: the configs for the systemview should be provided in the hardware layer of the respective boards
    list(APPEND SYSTEMVIEW_SRCS "${THIRD_PARTY_DIR}/sysview/sysviewConfig.c")

    # Startup assembly script.
    set(STARTUP_SRC "${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f412rx.s")

    set(STM32CUBE_SRCS ${STM32_HAL_SRCS} ${RTOS_SRCS} ${SYSTEMVIEW_SRCS} ${SYSCALLS} ${IOC_CHECKSUM} ${STARTUP_SRC} ${NEWLIB_SRCS})
    embedded_library(
            "${HAL_LIB_NAME}"
            "${STM32CUBE_SRCS}"
            "${STM32CUBE_INCLUDE_DIRS}"
            "cm4"
            TRUE
    )
    target_compile_definitions(${HAL_LIB_NAME}
            PUBLIC
            USE_HAL_DRIVER
            STM32F412Rx
    )
endfunction()

message("  🔃 Registered stm32h733xx_cube_library() function")
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
            "${HAL_CONF_DIR}"
            "${DRIVERS_DIR}/STM32H7xx_HAL_Driver/Inc"
            "${DRIVERS_DIR}/STM32H7xx_HAL_Driver/Inc/Legacy"
            "${FREERTOS_DIR}/include"
            "${FREERTOS_DIR}/CMSIS_RTOS_V2"
            "${FREERTOS_DIR}/portable/GCC/ARM_CM4F"
            "${DRIVERS_DIR}/CMSIS/Device/ST/STM32H7xx/Include"
            "${DRIVERS_DIR}/CMSIS/Include"
            # SEGGER SystemView includes.
            "${THIRD_PARTY_DIR}/sysview"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Config"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Sample/FreeRTOSV10"
    )
    # HAL sources.
    set(STM32_HAL_SRCS)
    foreach (HAL_SRC ${HAL_SRCS})
        list(APPEND STM32_HAL_SRCS "${DRIVERS_DIR}/STM32H7xx_HAL_Driver/Src/${HAL_SRC}")
    endforeach ()

    # FreeRTOS sources.
    file(GLOB RTOS_SRCS
            "${FREERTOS_DIR}/*.c"
            "${FREERTOS_DIR}/CMSIS_RTOS_V2/cmsis_os2.c"
            "${FREERTOS_DIR}/portable/MemMang/heap_4.c"
            "${FREERTOS_DIR}/portable/GCC/ARM_CM4F/port.c"
    )

    # SEGGER SystemView sources.
    file(GLOB SYSTEMVIEW_SRCS
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/*.c"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/*.S"
    )
    # We use ARM's embedded GCC compiler, so append the GCC-specific SysCalls.
    list(APPEND SYSTEMVIEW_SRCS "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c")
    # Append the FreeRTOS patch to get SystemView to work with FreeRTOS. All of our boards use FreeRTOS 10.3.1.
    file(GLOB_RECURSE SYSTEMVIEW_FREERTOS_SRCS "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Sample/FreeRTOSV10/SEGGER_SYSVIEW_FreeRTOS.c")
    list(APPEND SYSTEMVIEW_SRCS ${SYSTEMVIEW_FREERTOS_SRCS})
    # NOTE: the configs for the systemview should be provided in the hardware layer of the respective boards
    list(APPEND SYSTEMVIEW_SRCS "${THIRD_PARTY_DIR}/sysview/sysviewConfig.c")

    # Startup assembly script.
    set(STARTUP_SRC "${DRIVERS_DIR}/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/startup_stm32h733xx.s")

    set(STM32CUBE_SRCS ${STM32_HAL_SRCS} ${RTOS_SRCS} ${SYSTEMVIEW_SRCS} ${SYSCALLS} ${IOC_CHECKSUM} ${STARTUP_SRC})
    embedded_library(
            "${HAL_LIB_NAME}"
            "${STM32CUBE_SRCS}"
            "${STM32CUBE_INCLUDE_DIRS}"
            "cm7"
            TRUE
    )
    target_compile_definitions(${HAL_LIB_NAME}
            PUBLIC
            USE_HAL_DRIVER
            STM32H733xx
            CANFD
    )
endfunction()