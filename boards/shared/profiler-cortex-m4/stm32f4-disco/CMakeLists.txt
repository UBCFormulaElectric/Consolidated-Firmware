#-- Service --------------------------------------------------------------------
SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/gcc_cm4f.cmake)
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.0)
ENABLE_LANGUAGE(ASM)
IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    MESSAGE(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
ENDIF()
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#-- Project name ---------------------------------------------------------------
PROJECT(profiler)

#-- Defines --------------------------------------------------------------------
#ADD_DEFINITIONS(-DUSE_FULL_ASSERT=1)
ADD_DEFINITIONS(-DUSE_RETARGET) # or USE_RETARGET_ITM
ADD_DEFINITIONS(-DHSE_VALUE=8000000)
ADD_DEFINITIONS(-DUSE_FULL_LL_DRIVER)
ADD_DEFINITIONS(-DSTM32F407xx)

#-- Project paths --------------------------------------------------------------
SET(APP_PATH                app)
SET(LIBS_PATH               lib)
SET(CMSIS_DEVICE_SRC_PATH   ${LIBS_PATH}/CMSIS/Device/ST/STM32F4xx/Source/Template/GCC)
SET(CMSIS_DEVICE_INC_PATH   ${LIBS_PATH}/CMSIS/Device/ST/STM32F4xx/Include)
SET(CMSIS_CORE_INC_PATH     ${LIBS_PATH}/CMSIS/Core/Include)
SET(HAL_SRC_PATH            ${LIBS_PATH}/STM32F4xx_HAL_Driver_V1.7.4/Src)
SET(HAL_INC_PATH            ${LIBS_PATH}/STM32F4xx_HAL_Driver_V1.7.4/Inc)
SET(RETARGET_SRC_PATH       ${LIBS_PATH}/retarget)
SET(RETARGET_INC_PATH       ${LIBS_PATH}/retarget)
SET(PROFILER_SRC_PATH       ../profiler)
SET(PROFILER_INC_PATH       ../profiler)

#-- Include dirs ---------------------------------------------------------------
INCLUDE_DIRECTORIES(${APP_PATH})
INCLUDE_DIRECTORIES(${HAL_INC_PATH})
INCLUDE_DIRECTORIES(${CMSIS_CORE_INC_PATH})
INCLUDE_DIRECTORIES(${CMSIS_DEVICE_INC_PATH})
INCLUDE_DIRECTORIES(${RETARGET_INC_PATH})
INCLUDE_DIRECTORIES(${PROFILER_INC_PATH})

#-- Sources list ---------------------------------------------------------------
FILE(GLOB_RECURSE APP_SRC ${APP_PATH}/*.c ${RETARGET_SRC_PATH}/*.c ${PROFILER_SRC_PATH}/*.c)
LIST(APPEND APP_SRC ${CMSIS_DEVICE_SRC_PATH}/startup_stm32f407xx.s)

#-- Libs -----------------------------------------------------------------------
FILE(GLOB HAL_SRC ${HAL_SRC_PATH}/*ll_*.c)
ADD_LIBRARY(
    HAL
    ${HAL_SRC}
)

#-- Options --------------------------------------------------------------------
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u_printf_float")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u_scanf_float")

#-- Linker script --------------------------------------------------------------
SET(LDSCRIPT ${CMAKE_SOURCE_DIR}/${CMSIS_DEVICE_SRC_PATH}/STM32F407VGTx_FLASH.ld)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LDSCRIPT} -Wl,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map -Wl,--print-memory-usage")

#-- Project linking ------------------------------------------------------------
ADD_EXECUTABLE(${PROJECT_NAME}.elf ${APP_SRC})
TARGET_LINK_LIBRARIES(${PROJECT_NAME}.elf HAL m)

#-- Custom commands ------------------------------------------------------------
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} "-Oihex" ${PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} "-Obinary" ${PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
        COMMAND ${CMAKE_OBJDUMP} "-DS" ${PROJECT_NAME}.elf > ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dasm
        COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf)
