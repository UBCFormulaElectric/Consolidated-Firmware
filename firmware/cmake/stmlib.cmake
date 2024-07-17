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

# ==== important variables ====
# SEGGER SystemView library, enables CPU profiling with a J-Link dongle.
set(SYSTEMVIEW_DIR "${THIRD_PARTY_DIR}/SEGGER_SystemView_Src")

# FreeRTOS patch to enable thread-safe malloc (so we can use the heap with FreeRTOS).
set(NEWLIB_DIR "${THIRD_PARTY_DIR}/newlib_freertos_patch")


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

    foreach (HAL_SRC ${HAL_SRCS})
        list(APPEND STM32_HAL_SRCS "${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/${HAL_SRC}")
    endforeach ()

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
    foreach (HAL_SRC ${HAL_SRCS})
        list(APPEND STM32_HAL_SRCS "${DRIVERS_DIR}/STM32H7xx_HAL_Driver/Src/${HAL_SRC}")
    endforeach ()

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
            -DCANFD
    )
endfunction()