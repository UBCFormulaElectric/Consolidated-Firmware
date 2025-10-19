message("")
message("🎚️ [stmlib.cmake] Configuring STM32CubeMX functions")
set(STM32LIB_CMAKE_INCLUDED TRUE)

# ==== generate log4j properties for stm32cubemx codegen ====
IF (${CMAKE_HOST_WIN32}) # this is slightly more reliable than WIN32
    set(LOG4J_PROPERTIES "$ENV{UserProfile}/.stm32cubemx/log4j.properties")
ELSE ()
    set(LOG4J_PROPERTIES "$ENV{HOME}/.stm32cubemx/log4j.properties")
ENDIF ()
if (NOT EXISTS ${LOG4J_PROPERTIES})
    execute_process(
            COMMAND ${PYTHON_COMMAND}
            ${SCRIPTS_DIR}/utilities/generate_log4j_properties.py
            --log4j_properties_output ${LOG4J_PROPERTIES}
    )
endif ()
message("  📝 log4j.properties generated at ${LOG4J_PROPERTIES}")

file(GLOB_RECURSE NEWLIB_SRCS "${THIRD_PARTY_DIR}/newlib_freertos_patch/*.c")

# ==== STM32CubeMX functions ====

message("  🔃 Registered generate_stm32cube_code() function")
# Adds a custom command which generates new code when the IOC file changes.
# MD5_LOCATION is included into the GENERATED_SRCS list to ensure that dependent targets
# are rebuilt when the IOC file changes.
# TRACKED_MD5_LOCATION is stored on the github for CI reasons, to ensure coherency between generated code and the IOC file.
function(generate_stm32cube_code IOC_PATH)
    get_filename_component(IOC_DIR ${IOC_PATH} DIRECTORY)
    get_filename_component(IOC_FILE_NAME ${IOC_PATH} NAME)
    set(TRACKED_MD5_LOCATION "${IOC_PATH}.md5")
    set(OUTPUT_MD5_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/${IOC_FILE_NAME}.md5")
    set(MD5_LOCATION ${OUTPUT_MD5_LOCATION} PARENT_SCOPE)

    add_custom_command(
            OUTPUT ${OUTPUT_MD5_LOCATION}
            COMMENT "Generating drivers for ${LIB_NAME}"
            COMMAND ${PYTHON_COMMAND} ${SCRIPTS_DIR}/utilities/generate_cube_code.py
            --board ${LIB_NAME}
            --ioc ${IOC_PATH}
            --codegen_output_dir ${IOC_DIR}
            --cube_bin ${STM32CUBEMX_BIN_PATH}
            --md5 ${TRACKED_MD5_LOCATION}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}

            COMMAND ${CMAKE_COMMAND} -E copy ${TRACKED_MD5_LOCATION} ${OUTPUT_MD5_LOCATION}

            DEPENDS ${IOC_PATH}
    )
endfunction()

message("  🔃 Registered stm32f412rx_cube_library() function")
# HAL_SRCS: the ones that we want, stripped prefixes
# USB_ENABLED: flags if usb middleware should be included.
function(stm32f412rx_cube_library
        HAL_LIB_NAME
        HAL_SRCS
        IOC_PATH
        CUBEMX_SRCS
        CUBEMX_INCLUDE_DIR
        USB_ENABLED
        ARM_CORE
)
    set(DRIVERS_DIR "${STM32CUBEF4_SOURCE_DIR}/Drivers")
    set(FREERTOS_DIR "${STM32CUBEF4_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source")

    # Set include directories for STM32Cube library.
    set(STM32CUBE_INCLUDE_DIRS
            "${CUBEMX_INCLUDE_DIR}"
            "${SHARED_HW_INCLUDE_DIR}"
            "${SHARED_IO_INCLUDE_DIR}"
            "${SHARED_UTIL_INCLUDE_DIR}"

            "${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc"
            "${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy"
            "${FREERTOS_DIR}/include"
            "${FREERTOS_DIR}/CMSIS_RTOS_V2"
            "${FREERTOS_DIR}/portable/GCC/ARM_CM4F"
            "${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Include"
            "${DRIVERS_DIR}/CMSIS/Include"
            "${THIRD_PARTY_DIR}/freertos"

            # SEGGER SystemView includes.
            "${THIRD_PARTY_DIR}/sysview"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Config"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Sample/FreeRTOSV10"
    )

    # HAL sources.
    set(STM32_HAL_SRCS ${HAL_SRCS})
    list(TRANSFORM STM32_HAL_SRCS PREPEND "${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/")

    # FreeRTOS sources.
    file(GLOB RTOS_SRCS
            "${FREERTOS_DIR}/*.c"
            "${FREERTOS_DIR}/CMSIS_RTOS_V2/cmsis_os2.c"
            "${FREERTOS_DIR}/portable/GCC/ARM_CM4F/port.c"
            "${FREERTOS_DIR}/portable/MemMang/heap_4.c"
    )

    # SEGGER SystemView sources.
    file(GLOB SYSTEMVIEW_SRCS
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/*.c"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/*.S"
            # We use ARM's embedded GCC compiler, so append the GCC-specific SysCalls.
            # "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c"
            # Append the FreeRTOS patch to get SystemView to work with FreeRTOS. All of our boards use FreeRTOS 10.3.1.
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Sample/FreeRTOSV10/SEGGER_SYSVIEW_FreeRTOS.c"
            # NOTE: the configs for the systemview should be provided in the hardware layer of the respective boards
            "${THIRD_PARTY_DIR}/sysview/sysviewConfig.c"
    )

    # Startup assembly script.
    set(STARTUP_SRC "${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f412rx.s")

    set(STM32CUBE_SRCS ${STM32_HAL_SRCS} ${RTOS_SRCS} ${SYSTEMVIEW_SRCS} ${IOC_CHECKSUM} ${STARTUP_SRC} ${CUBEMX_SRCS})
    if (USB_ENABLED)
        set(USB_MIDDLEWARE_DIR "${STM32CUBEF4_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library")

        list(APPEND STM32CUBE_SRCS
                "${USB_MIDDLEWARE_DIR}/Class/CDC/Src/usbd_cdc.c"
                "${USB_MIDDLEWARE_DIR}/Core/Src/usbd_core.c"
                "${USB_MIDDLEWARE_DIR}/Core/Src/usbd_ctlreq.c"
                "${USB_MIDDLEWARE_DIR}/Core/Src/usbd_ioreq.c"
        )

        list(APPEND STM32CUBE_INCLUDE_DIRS
                "${USB_MIDDLEWARE_DIR}/Core/Inc"
                "${USB_MIDDLEWARE_DIR}/Class/CDC/Inc"
        )
    endif ()

    generate_stm32cube_code("${IOC_PATH}")
    embedded_object_library(
            "${HAL_LIB_NAME}"
            "${STM32CUBE_SRCS}"
            "${STM32CUBE_INCLUDE_DIRS}"
            TRUE
            "${ARM_CORE}"
    )

    if ("${WATCHDOG}" STREQUAL "OFF")
        target_compile_definitions(${HAL_LIB_NAME} PUBLIC WATCHDOG_DISABLED)
    endif ()
    target_compile_definitions(${HAL_LIB_NAME}
            PUBLIC
            USE_HAL_DRIVER
            STM32F412Rx
    )
endfunction()

message("  🔃 Registered stm32h733xx_cube_library() function")
function(stm32h733xx_cube_library
        HAL_LIB_NAME
        HAL_SRCS
        IOC_PATH
        CUBEMX_SRCS
        CUBEMX_INCLUDE_DIR
        USB_ENABLED
        ARM_CORE
)
    set(DRIVERS_DIR "${STM32CUBEH7_SOURCE_DIR}/Drivers")
    set(FREERTOS_DIR "${STM32CUBEH7_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source")

    # Set include directories for STM32Cube library.
    set(STM32CUBE_INCLUDE_DIRS
            "${CUBEMX_INCLUDE_DIR}"
            "${SHARED_HW_INCLUDE_DIR}"
            "${SHARED_IO_INCLUDE_DIR}"
            "${SHARED_UTIL_INCLUDE_DIR}"

            "${DRIVERS_DIR}/STM32H7xx_HAL_Driver/Inc"
            "${DRIVERS_DIR}/STM32H7xx_HAL_Driver/Inc/Legacy"
            "${FREERTOS_DIR}/include"
            "${FREERTOS_DIR}/CMSIS_RTOS_V2"
            "${FREERTOS_DIR}/portable/GCC/ARM_CM7/r0p1"
            "${DRIVERS_DIR}/CMSIS/Device/ST/STM32H7xx/Include"
            "${DRIVERS_DIR}/CMSIS/Include"
            "${THIRD_PARTY_DIR}/freertos"

            # SEGGER SystemView includes.
            "${THIRD_PARTY_DIR}/sysview"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Config"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Sample/FreeRTOSV10"
    )

    # HAL sources.
    set(STM32_HAL_SRCS ${HAL_SRCS})
    list(TRANSFORM STM32_HAL_SRCS PREPEND "${DRIVERS_DIR}/STM32H7xx_HAL_Driver/Src/")

    # FreeRTOS sources.
    file(GLOB RTOS_SRCS
            "${FREERTOS_DIR}/*.c"
            "${FREERTOS_DIR}/CMSIS_RTOS_V2/cmsis_os2.c"
            "${FREERTOS_DIR}/portable/GCC/ARM_CM7/r0p1/port.c"
            "${FREERTOS_DIR}/portable/MemMang/heap_4.c"
    )

    # SEGGER SystemView sources.
    file(GLOB SYSTEMVIEW_SRCS
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/*.c"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/*.S"
            # We use ARM's embedded GCC compiler, so append the GCC-specific SysCalls.
            # "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c"
            # Append the FreeRTOS patch to get SystemView to work with FreeRTOS. All of our boards use FreeRTOS 10.3.1.
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Sample/FreeRTOSV10/SEGGER_SYSVIEW_FreeRTOS.c"
            # NOTE: the configs for the systemview should be provided in the hardware layer of the respective boards
            "${THIRD_PARTY_DIR}/sysview/sysviewConfig.c"
    )

    # Startup assembly script.
    set(STARTUP_SRC "${DRIVERS_DIR}/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/startup_stm32h733xx.s")

    set(STM32CUBE_SRCS ${STM32_HAL_SRCS} ${RTOS_SRCS} ${SYSTEMVIEW_SRCS} ${IOC_CHECKSUM} ${STARTUP_SRC} ${CUBEMX_SRCS})

    # Handle usb srcs and include directories.
    # Currently, all our USB devices are of the Communications Device Class (CDC).
    # If we want to ever support different device classes,
    # you will need to add a argument to this function with the requested class.
    if (USB_ENABLED)
        set(USB_MIDDLEWARE_DIR "${STM32CUBEH7_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library")
        list(APPEND STM32CUBE_SRCS
                "${USB_MIDDLEWARE_DIR}/Class/CDC/Src/usbd_cdc.c"
                "${USB_MIDDLEWARE_DIR}/Core/Src/usbd_core.c"
                "${USB_MIDDLEWARE_DIR}/Core/Src/usbd_ctlreq.c"
                "${USB_MIDDLEWARE_DIR}/Core/Src/usbd_ioreq.c"
        )
        list(APPEND STM32CUBE_INCLUDE_DIRS
                "${USB_MIDDLEWARE_DIR}/Core/Inc"
                "${USB_MIDDLEWARE_DIR}/Class/CDC/Inc"
        )
    endif ()

    generate_stm32cube_code("${IOC_PATH}")
    embedded_object_library(
            "${HAL_LIB_NAME}"
            "${STM32CUBE_SRCS}"
            "${STM32CUBE_INCLUDE_DIRS}"
            TRUE
            "${ARM_CORE}"
    )

    if ("${WATCHDOG}" STREQUAL "OFF")
        target_compile_definitions(${HAL_LIB_NAME} PUBLIC WATCHDOG_DISABLED)
    endif ()
    target_compile_definitions(${HAL_LIB_NAME}
            PUBLIC
            USE_HAL_DRIVER
            STM32H733xx
    )
endfunction()

message(" 🔃 Registered stm32h562xx_cube_library () function")
function(stm32h562xx_cube_library
        HAL_LIB_NAME
        HAL_SRCS
        IOC_PATH
        CUBEMX_SRCS
        CUBEMX_INCLUDE_DIR
        USB_ENABLED
        ARM_CORE
)
    set(DRIVERS_DIR "${STM32CUBEH5_SOURCE_DIR}/Drivers")
    set(FREERTOS_DIR "${STM32CUBEH5_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source")

    # Set include directories for STM32Cube library.
    set(STM32CUBE_INCLUDE_DIRS
            "${CUBEMX_INCLUDE_DIR}"
            "${SHARED_HW_INCLUDE_DIR}"
            "${SHARED_IO_INCLUDE_DIR}"
            "${SHARED_UTIL_INCLUDE_DIR}"

            "${DRIVERS_DIR}/STM32H5xx_HAL_Driver/Inc"
            "${DRIVERS_DIR}/STM32H5xx_HAL_Driver/Inc/Legacy"
            "${FREERTOS_DIR}/include"
            "${FREERTOS_DIR}/CMSIS_RTOS_V2"
            "${FREERTOS_DIR}/portable/GCC/ARM_CM33/non_secure"
            "${DRIVERS_DIR}/CMSIS/Device/ST/STM32H5xx/Include"
            "${DRIVERS_DIR}/CMSIS/Include"
            "${THIRD_PARTY_DIR}/freertos"

            # SEGGER SystemView includes.
            "${THIRD_PARTY_DIR}/sysview"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Config"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Sample/FreeRTOSV10"
    )

    # HAL sources.
    set(STM32_HAL_SRCS ${HAL_SRCS})
    list(TRANSFORM STM32_HAL_SRCS PREPEND "${DRIVERS_DIR}/STM32H5xx_HAL_Driver/Src/")

    # FreeRTOS sources.
    file(GLOB RTOS_SRCS
            "${FREERTOS_DIR}/*.c"
            "${FREERTOS_DIR}/CMSIS_RTOS_V2/cmsis_os2.c"
            "${FREERTOS_DIR}/portable/GCC/ARM_CM33_NTZ/port.c"
            "${FREERTOS_DIR}/portable/MemMang/heap_4.c"
    )

    # SEGGER SystemView sources.
    file(GLOB SYSTEMVIEW_SRCS
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/*.c"
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/*.S"
            # We use ARM's embedded GCC compiler, so append the GCC-specific SysCalls.
            # "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c"
            # Append the FreeRTOS patch to get SystemView to work with FreeRTOS. All of our boards use FreeRTOS 10.3.1.
            "${SEGGER_SYSTEMVIEW_SOURCE_DIR}/Sample/FreeRTOSV10/SEGGER_SYSVIEW_FreeRTOS.c"
            # NOTE: the configs for the systemview should be provided in the hardware layer of the respective boards
            "${THIRD_PARTY_DIR}/sysview/sysviewConfig.c"
    )

    # Startup assembly script.
    set(STARTUP_SRC "${DRIVERS_DIR}/CMSIS/Device/ST/STM32H5xx/Source/Templates/gcc/startup_stm32h562xx.s")

    set(STM32CUBE_SRCS ${STM32_HAL_SRCS} ${RTOS_SRCS} ${SYSTEMVIEW_SRCS} ${IOC_CHECKSUM} ${STARTUP_SRC} ${CUBEMX_SRCS})

    # Handle usb srcs and include directories.
    # Currently, all our USB devices are of the Communications Device Class (CDC).
    # If we want to ever support different device classes,
    # you will need to add a argument to this function with the requested class.
    if (USB_ENABLED)
        set(USB_MIDDLEWARE_DIR "${STM32CUBEH5_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library")
        list(APPEND STM32CUBE_SRCS
                "${USB_MIDDLEWARE_DIR}/Class/CDC/Src/usbd_cdc.c"
                "${USB_MIDDLEWARE_DIR}/Core/Src/usbd_core.c"
                "${USB_MIDDLEWARE_DIR}/Core/Src/usbd_ctlreq.c"
                "${USB_MIDDLEWARE_DIR}/Core/Src/usbd_ioreq.c"
        )
        list(APPEND STM32CUBE_INCLUDE_DIRS
                "${USB_MIDDLEWARE_DIR}/Core/Inc"
                "${USB_MIDDLEWARE_DIR}/Class/CDC/Inc"
        )
    endif ()

    generate_stm32cube_code("${IOC_PATH}")
    embedded_object_library(
            "${HAL_LIB_NAME}"
            "${STM32CUBE_SRCS}"
            "${STM32CUBE_INCLUDE_DIRS}"
            TRUE
            "${ARM_CORE}"
    )

    if ("${WATCHDOG}" STREQUAL "OFF")
        target_compile_definitions(${HAL_LIB_NAME} PUBLIC WATCHDOG_DISABLED)
    endif ()
    target_compile_definitions(${HAL_LIB_NAME}
            PUBLIC
            USE_HAL_DRIVER
            STM32H562xx
    )
endfunction()
