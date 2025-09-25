set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)

# On Apple silicon, this line mysteriously causes loading CMake for unit tests to fail.
# However, it is equally mysteriously required for building embedded binaries.
# So, only run this if loading the embedded configuration.
# TODO: Look into what is causing this.
set(CMAKE_C_COMPILER_WORKS 1)

IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(TOOLCHAIN_POSTFIX ".exe")
ELSEIF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    set(TOOLCHAIN_POSTFIX "")
ELSEIF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(TOOLCHAIN_POSTFIX "")
ELSE ()
    message(FATAL_ERROR "❌ Unsupported host system: ${CMAKE_HOST_SYSTEM_NAME}")
ENDIF ()

set(TOOLCHAIN_PREFIX arm-none-eabi)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc${TOOLCHAIN_POSTFIX})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++${TOOLCHAIN_POSTFIX})
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc${TOOLCHAIN_POSTFIX})
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy${TOOLCHAIN_POSTFIX})
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump${TOOLCHAIN_POSTFIX})
set(SIZE ${TOOLCHAIN_PREFIX}-size${TOOLCHAIN_POSTFIX})

# TODO consider bringing these in? Would change the name of many targets.
#set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
#set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
#set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

# WARNING: ONLY ENABLE THESE AS LAST RESORT FOR "Compiler can't compile a simple program" ERROR
#set(CMAKE_C_COMPILER_WORKS 1)
#set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Turn this on to debug makefile
#set(CMAKE_VERBOSE_MAKEFILE ON)

message("")
message("✅ ARM Toolchain Included, using ${CMAKE_C_COMPILER}")
