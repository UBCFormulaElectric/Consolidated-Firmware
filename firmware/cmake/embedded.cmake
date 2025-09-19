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
    # check if you have the STM32CubeMX_PATH environment variable set
    if (NOT "$ENV{STM32CubeMX_PATH}" STREQUAL "")
        set(STM32CUBEMX_BIN_PATH "$ENV{STM32CubeMX_PATH}/STM32CubeMX.exe")
    else ()
        # if not, guess the you have it here
        set(STM32CUBEMX_BIN_PATH "C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeMX/STM32CubeMX.exe")
        # check if the file exists
        if (NOT EXISTS ${STM32CUBEMX_BIN_PATH})
            message(FATAL_ERROR "❌ STM32CubeMX not found at ${STM32CUBEMX_BIN_PATH}")
        endif ()
    endif ()
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
        --specs=nosys.specs
        -mthumb
        -mthumb-interwork
        -ffunction-sections
        -fdata-sections
        -fno-common
        -fmessage-length=0
        -Wall
        -Werror
        -Wextra
        -pedantic
        -Wdouble-promotion
        -Wshadow
        -Wundef
        -fstack-usage
        -Wconversion
        -Wno-unused-variable
        -Wno-unused-parameter
)
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    list(APPEND SHARED_COMPILER_FLAGS
            -O0 # previously O0, idk why this breaks bootloader??
            -g3
    )
else ()
    list(APPEND SHARED_COMPILER_FLAGS
            -Os
            -g0
    )
endif ()
set(SHARED_LINKER_FLAGS
        -Wl,-gc-sections,--print-memory-usage
        -L${FIRMWARE_DIR}/linker
        -static
        --specs=nano.specs
        --specs=nosys.specs
)

set(CM33_DEFINES
        -DARM_MATH_CM33
)
# FPU flags are compiler and linker flags
set(CM33_FPU_FLAGS
        -mcpu=cortex-m33
        -mfloat-abi=hard
        -mfpu=fpv4-sp-d16
)

set(CM4_DEFINES
        -DARM_MATH_CM4
)
# FPU flags are compiler and linker flags
set(CM4_FPU_FLAGS
        -mcpu=cortex-m4
        -mfloat-abi=hard
        -mfpu=fpv4-sp-d16
)

set(CM7_DEFINES
        -DARM_MATH_CM7
)
# FPU flags are compiler and linker flags
set(CM7_FPU_FLAGS
        -mcpu=cortex-m7
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

    IF (${THIRD_PARTY})
        # Suppress header file warnings for third-party code by marking them as system includes.
        target_include_directories(${LIB_NAME} SYSTEM
                PUBLIC
                ${LIB_INCLUDE_DIRS}
        )

        # Suppress source file warnings for third-party code.
        #        list(APPEND COMPILER_FLAGS -w)
        embedded_no_checks("${LIB_SRCS}")
    ELSE ()
        target_include_directories(${LIB_NAME} PUBLIC ${LIB_INCLUDE_DIRS})
        #        list(APPEND COMPILER_FLAGS ${WARNING_COMPILER_FLAGS})
    ENDIF ()

    set(COMPILER_DEFINES ${SHARED_COMPILER_DEFINES})
    set(COMPILER_FLAGS ${SHARED_COMPILER_FLAGS})
    set(LINKER_FLAGS ${SHARED_LINKER_FLAGS})

    IF ("${ARM_CORE}" STREQUAL "cm4")
        list(APPEND COMPILER_DEFINES ${CM4_DEFINES})
        list(APPEND COMPILER_FLAGS ${CM4_FPU_FLAGS})
        list(APPEND LINKER_FLAGS ${CM4_FPU_FLAGS})
    ELSEIF ("${ARM_CORE}" STREQUAL "cm33")
        list(APPEND COMPILER_DEFINES ${CM33_DEFINES})
        list(APPEND COMPILER_FLAGS ${CM33_FPU_FLAGS})
        list(APPEND LINKER_FLAGS ${CM33_FPU_FLAGS})
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

function(embedded_interface_library
        LIB_NAME
        LIB_SRCS
        LIB_INCLUDE_DIRS
        THIRD_PARTY
)
    add_library(${LIB_NAME} INTERFACE)
    target_sources(${LIB_NAME} INTERFACE ${LIB_SRCS})

    IF (${THIRD_PARTY})
        # Suppress header file warnings for third-party code by marking them as system includes
        target_include_directories(${LIB_NAME} SYSTEM
                INTERFACE
                ${LIB_INCLUDE_DIRS}
        )
        set_source_files_properties(
                ${LIB_SRCS}
                PROPERTIES COMPILE_FLAGS "-w"
        )
    ELSE ()
        target_include_directories(${LIB_NAME} INTERFACE ${LIB_INCLUDE_DIRS})
    ENDIF ()
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
    set_source_files_properties(${BIN_SRCS} PROPERTIES COMPILE_FLAGS "-fsanitize=undefined")
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
    ELSEIF ("${ARM_CORE}" STREQUAL "cm33")
        list(APPEND COMPILER_DEFINES ${CM33_DEFINES})
        list(APPEND COMPILER_FLAGS ${CM33_FPU_FLAGS})
        list(APPEND LINKER_FLAGS ${CM33_FPU_FLAGS})
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
    add_custom_target(${IMAGE_HEX}
            COMMENT "[Image] Building ${IMAGE_HEX} and ${APP_METADATA_HEX}"
            COMMAND ${PYTHON_COMMAND} ${GENERATE_IMAGE_SCRIPT}
            --app-hex ${APP_HEX_PATH}
            --boot-hex ${BOOT_HEX_PATH}
            --app-metadata-hex-out ${APP_METADATA_HEX_PATH}
            --image-hex-out ${IMAGE_HEX_PATH}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS ${GENERATE_IMAGE_SCRIPT} ${APP_HEX_PATH} ${BOOT_HEX_PATH}
    )
    add_dependencies(${IMAGE_HEX} ${APP_HEX_TARGET} ${BOOT_HEX_TARGET})
endfunction()

function(embedded_no_checks SRCS)
    set(unique_dirs "")  # Initialize empty list

    foreach (file_path IN LISTS SRCS)
        # Get absolute path to file
        get_filename_component(abs_file_path "${file_path}" ABSOLUTE)

        # Get directory of the file
        get_filename_component(dir_path "${abs_file_path}" DIRECTORY)

        # Get path relative to the CMake root
        file(RELATIVE_PATH rel_path "${CMAKE_SOURCE_DIR}" "${dir_path}")

        # Avoid duplicates
        list(FIND unique_dirs "${rel_path}" found_index)
        if (found_index EQUAL -1)
            list(APPEND unique_dirs "${rel_path}")
        endif ()
    endforeach ()

    list(LENGTH SRCS SRCS_LENGTH)
    message("  🚫 [embedded.cmake, embedded_no_checks()] Disabling Warnings for ${SRCS_LENGTH} files under ${unique_dirs}")
    set_source_files_properties(
            ${SRCS}
            PROPERTIES COMPILE_FLAGS "-w"
    )
endfunction()
