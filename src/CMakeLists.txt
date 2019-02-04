project(FORMULA_E C ASM)
cmake_minimum_required(VERSION 3.2)

##############################################
###                                        ###
###           Utility Functions            ###
###                                        ###
##############################################

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

################################################################################
##                                                                             #
##                         CODE COMPILATION BEGINS HERE                        #
##                                                                             #
##                      Append the list of module directories                  #
##    WARNING! Ensure module name in the string list matches directory name!   #
##                                                                             #
################################################################################

set(MODULES "PDM FSM DCM BMS")

#Create string list
string(REPLACE " " ";" MODULES ${MODULES})

##############################################
###                                        ###
###          Compiler Tool Setup           ###
###        * PROCEED WITH CAUTION *        ###
###                                        ###
##############################################

###  Compiler Tools  ###
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

if(MINGW OR CYGWIN OR WIN32)
    set(UTIL_SEARCH_CMD where)
elseif(UNIX OR APPLE)
    set(UTIL_SEARCH_CMD which)
endif()

set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(TOOLCHAIN_TRIPLE arm-none-eabi)

find_program(BINUTILS_PATH ${TOOLCHAIN_PREFIX}gcc)

if(BINUTILS_PATH)
    message(STATUS "Found " ${TOOLCHAIN_PREFIX} "gcc in " ${BINUTILS_PATH})
else()
    message(FATAL_ERROR "Could not find " ${TOOLCHAIN_PREFIX} "gcc")
endif(BINUTILS_PATH)

get_filename_component(ARM_TOOLCHAIN_DIR ${BINUTILS_PATH} DIRECTORY)
set(triple ${TOOLCHAIN_TRIPLE})
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}as)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH ${ARM_TOOLCHAIN_DIR})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build)

##############################################
###                                        ###
###        GLOBAL COMPILATION FLAGS        ###
###  * CHANGE THESE PARAMETERS AS NEEDED * ###
###                                        ###
##############################################

###  DEBUG FLAGS  ###
set(DEBUG_FLAGS -gdwarf-3)

###  OPTIMIZATION FLAGS  ###
set(OPT_FLAGS -O0)

###  WARNING FLAGS  ###
set(WARNING_FLAGS
    "-Wno-packed \
    -Wno-missing-variable-declarations \
    -Wno-missing-prototypes \
    -Wno-missing-noreturn \
    -Wno-sign-conversion \
    -Wno-nonportable-include-path \
    -Wno-reserved-id-macro \
    -Wno-unused-macros \
    -Wno-documentation-unknown-command \
    -Wno-documentation \
    -Wno-license-management \
    -Wno-parentheses-equality"
)

set(MISC_FLAGS -funsigned-char -ffunction-sections -fdata-sections -Wall -MD)

# Cube-generated flags
set(TARGET_DEFS -DUSE_STM32F3XX_NUCLEO -DUSE_HAL_DRIVER -std=c11)

##############################################
###                                        ###
###          GLOBAL LINKER OPTIONS         ###
###                                        ###
##############################################

# Remove the -rdynamic flag
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

##############################################
###                                        ###
###      ITERATIVE MODULE COMPILATION      ###
###                                        ###
##############################################

foreach(MODULE_DIR ${MODULES})
    
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_DIR})
        message(STATUS "Found " ${MODULE_DIR})
    else()
        message("WARNING! No directory found for " ${MODULE_DIR} ". Skipping!")
        continue()
    endif()
    
    #Name each executable target ${MODULE_DIR}_CODE
    string(CONCAT exe_name ${MODULE_DIR} "_CODE")

    # BMS-specific flags
    if(MODULE_DIR STREQUAL BMS)
        set(MCU -mcpu=cortex-m4)
        set(FPU -mfpu=fpv4-sp-d16)
        set(T32 -mthumb)
        set(FLOAT_ABI -mfloat-abi=soft) 
        set(BOARD -DSTM32F302x8)  
        set(MATH_LIB -DARM_MATH_CM4)  
        set(LINKER_SCRIPT STM32F302R8Tx_FLASH.ld)
        set(HAL_BOARD_DIR STM32F3xx)

    # FSM-specific flags
    elseif(MODULE_DIR STREQUAL FSM)
        set(MCU -mcpu=cortex-m0)
        set(FPU )
        set(T32 -mthumb)
        set(FLOAT_ABI ) 
        set(BOARD -DSTM32F042x6)
        set(MATH_LIB -DARM_MATH_CM0)
        set(LINKER_SCRIPT STM32F042K6Tx_FLASH.ld)
        set(HAL_BOARD_DIR STM32F0xx)
    
    # PDM-specific flags
    elseif(MODULE_DIR STREQUAL PDM)
        set(MCU -mcpu=cortex-m4)
        set(FPU -mfpu=fpv4-sp-d16)
        set(T32 -mthumb)
        set(FLOAT_ABI -mfloat-abi=soft) 
        set(BOARD -DSTM32F302x8) 
        set(MATH_LIB -DARM_MATH_CM4)   
        set(LINKER_SCRIPT STM32F302R8Tx_FLASH.ld)
        set(HAL_BOARD_DIR STM32F3xx)

    # DCM-specific flags
    elseif(MODULE_DIR STREQUAL DCM)
        set(MCU -mcpu=cortex-m4)
        set(FPU -mfpu=fpv4-sp-d16)
        set(T32 -mthumb)
        set(FLOAT_ABI -mfloat-abi=soft) 
        set(BOARD -DSTM32F302x8)
        set(MATH_LIB -DARM_MATH_CM4)   
        set(LINKER_SCRIPT STM32F302R8Tx_FLASH.ld)
        set(HAL_BOARD_DIR STM32F3xx)
    
    else()
        message("WARNING! No compilation flags for " ${MODULE_DIR} ". Skipping!")
        continue()
    endif()

    # Custom flag detailing the name of the module
    set(BOARD_DEFS -D${MODULE_DIR})

    # Target-specific definitions
    set(TOTAL_TARGET_DEFS ${BOARD} ${TARGET_DEFS} ${BOARD_DEFS} ${MATH_LIB} ${TEST_DEFS})

    ###  ASM FLAGS  ###
    set(MODULE_ASM_FLAGS ${MCU} ${FPU} ${T32} ${FLOAT_ABI})

    ###  C FLAGS  ###
    set(MODULE_C_FLAGS ${TOTAL_TARGET_DEFS} ${MCU} ${FPU} ${T32} ${FLOAT_ABI} ${DEBUG_FLAGS} ${OPT_FLAGS} ${MISC_FLAGS})

    ###  SOURCE FILES  ###
    # Build the path to the project file
    string(CONCAT PROJECT_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR} "/" ${MODULE_DIR} "/MDK-ARM/" ${MODULE_DIR} ".uvprojx")

    # Generate the cmake file containing the "INCLUDE_DIRS" and 
    # "SRC_C_FILES". Also prefix the generated paths appropriately so we can
    # reference them from here (by default they are referenced relative to the
    # project files from which they were parsed)
    string(CONCAT GENERATED_CMAKE_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR} "/CMakeFiles/" ${MODULE_DIR} ".cmake")
    execute_process(
        COMMAND python "${CMAKE_CURRENT_LIST_DIR}/cmake/uvprojx_to_cmake.py" 
        "${GENERATED_CMAKE_FILE_PATH}" 
        "${PROJECT_FILE_PATH}" 
        --file-path-prefix "${CMAKE_SOURCE_DIR}/${MODULE_DIR}/MDK-ARM/"
        RESULT_VARIABLE MODULE_CMAKE_GENERATION_SUCCESS
        )

    # Make sure generation went OK
    if(NOT ${MODULE_CMAKE_GENERATION_SUCCESS} MATCHES "0")
        message(FATAL_ERROR "CMake Generation Failed For ${MODULE_DIR}")
    endif(NOT ${MODULE_CMAKE_GENERATION_SUCCESS} MATCHES "0")

    # TODO: Delete me
    message(STATUS ${INCLUDE_DIRS})

    # include the cmake file we just generated, and in doing so "import" the
    # "${MODULE_NAME_SRC_C_FILES}" and "${MODULE_NAME_INCLUDE_DIRS}" variables for this module
    include("${GENERATED_CMAKE_FILE_PATH}")

    # Grab the bootloader file. There is only one assembly file required: startup_stm32fxxxxxx.s 
    # Keil-generated startup assembly files are written in ARM assembly syntax instead of the GCC syntax, 
    # which CMake relies on. GCC syntax files, generated from STMCube, have been appended with '_gcc' and
    # added to the source file lists.
    file(GLOB_RECURSE SRC_ASM_FILES ${MODULE_DIR}/*_gcc.s)

    ### PROJECT EXECUTABLE ###
    add_executable(${exe_name} ${SRC_C_FILES} ${SRC_ASM_FILES})

    ###  INCLUDE DIRECTORIES  ###
    target_include_directories(${exe_name} PRIVATE "$<BUILD_INTERFACE:${INCLUDE_DIRS}>")

    ### COMPILER FLAGS ###
    # Add the compiler flags
    target_compile_options(${exe_name} PRIVATE $<$<COMPILE_LANGUAGE:C>:${MODULE_C_FLAGS}>)
    target_compile_options(${exe_name} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:${MODULE_ASM_FLAGS}>)
    
    ### TARGET LINKER OPTION ###
    # Add the command-line linker options for the target
    target_link_libraries(${exe_name} 
        "-Wl,--gc-sections \
        -specs=nano.specs \
        -specs=nosys.specs \
        -mthumb \
        ${MCU} \
        -T${MODULE_DIR}/${LINKER_SCRIPT} \
        -Wl,-Map=Build/${exe_name}.map,--cref"
    )
endforeach()
