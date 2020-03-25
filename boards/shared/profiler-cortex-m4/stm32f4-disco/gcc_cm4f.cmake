#-- System ---------------------------------------------------------------------
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)

#-- Toolchain ------------------------------------------------------------------
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
IF(WIN32)
    SET(TOOL_EXECUTABLE_SUFFIX ".exe")
ELSE()
    SET(TOOL_EXECUTABLE_SUFFIX "")
ENDIF()
SET(CMAKE_C_COMPILER "arm-none-eabi-gcc${TOOL_EXECUTABLE_SUFFIX}")
SET(CMAKE_CXX_COMPILER "arm-none-eabi-g++${TOOL_EXECUTABLE_SUFFIX}")
SET(CMAKE_ASM_COMPILER "arm-none-eabi-gcc${TOOL_EXECUTABLE_SUFFIX}")
SET(CMAKE_OBJCOPY "arm-none-eabi-objcopy${TOOL_EXECUTABLE_SUFFIX}" CACHE STRING "objcopy tool")
SET(CMAKE_OBJDUMP "arm-none-eabi-objdump${TOOL_EXECUTABLE_SUFFIX}" CACHE STRING "objdump tool")
SET(CMAKE_SIZE "arm-none-eabi-size${TOOL_EXECUTABLE_SUFFIX}" CACHE STRING "size tool")

#-- Common flags ---------------------------------------------------------------
SET(COMPILER_COMMON_FLAGS "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wall -Wextra -ffunction-sections -fdata-sections -mlong-calls")
SET(CMAKE_C_FLAGS "${COMPILER_COMMON_FLAGS} -std=gnu99" CACHE STRING "c compiler flags")
SET(CMAKE_CXX_FLAGS "${COMPILER_COMMON_FLAGS} -std=c++11" CACHE STRING "c++ compiler flags")
SET(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m4" CACHE STRING "assembler compiler flags")
SET(CMAKE_EXE_LINKER_FLAGS "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlong-calls -Wl,--gc-sections  -specs=nosys.specs -specs=nano.specs -lgcc -lc" CACHE STRING "executable linker flags")
SET(CMAKE_MODULE_LINKER_FLAGS "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16" CACHE STRING "module linker flags")
SET(CMAKE_SHARED_LINKER_FLAGS "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16" CACHE STRING "shared linker flags")

#-- Debug flags ----------------------------------------------------------------
SET(CMAKE_C_FLAGS_DEBUG "-Og -g -DDEBUG" CACHE STRING "c compiler flags debug")
SET(CMAKE_CXX_FLAGS_DEBUG "-Og -g -DDEBUG" CACHE STRING "c++ compiler flags debug")
SET(CMAKE_ASM_FLAGS_DEBUG "-g" CACHE STRING "assembler compiler flags debug")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "" CACHE STRING "linker flags debug")

#-- Release flags ----------------------------------------------------------------
SET(CMAKE_C_FLAGS_RELEASE "-Os -flto -ffat-lto-objects" CACHE STRING "c compiler flags release")
SET(CMAKE_CXX_FLAGS_RELEASE "-Os -flto -ffat-lto-objects" CACHE STRING "c++ compiler flags release")
SET(CMAKE_ASM_FLAGS_RELEASE "" CACHE STRING "assembler compiler flags release")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto" CACHE STRING "linker flags release")
