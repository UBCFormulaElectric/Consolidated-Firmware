project(FORMULA_E C ASM)
cmake_minimum_required(VERSION 3.2)

###################################################################################
##                                                                                #
##                        Append the list of module directories                   #
##       WARNING! Ensure module name in the string list matches directory!        #
##                                                                                #
###################################################################################

set(MODULES "PDM FSM")

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

if (TEST_BUILD)
set(TEST_DEFS
	"-DTEST"
)
endif()

##############################################
###                                        ###
###          GLOBAL LINKER OPTIONS         ###
###                                        ###
##############################################

# Remove the -rdynamic flag
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

##############################################
###                                        ###
###      SHARED_UTIL CODE COMPILATION      ###
###                                        ###
##############################################

file(GLOB_RECURSE SHARED_UTIL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/shared/*.c)

##############################################
###                                        ###
###      ITERATIVE MODULE COMPILATION      ###
###                                        ###
##############################################

foreach(MODULE_DIR ${MODULES})
	

	string(CONCAT exe_name ${MODULE_DIR} "_CODE")

	if(MODULE_DIR STREQUAL FSM)
		set(MCU -mcpu=cortex-m0)
		set(FPU )
		set(T32 -mthumb)
		set(FLOAT_ABI ) 
		set(BOARD -DSTM32F042x6)
		set(LINKER_SCRIPT STM32F042K6Tx_FLASH.ld)
		set(HAL_BOARD_DIR STM32F0xx)
	
	elseif(MODULE_DIR STREQUAL PDM)
		set(MCU -mcpu=cortex-m4)
		set(FPU -mfpu=fpv4-sp-d16)
		set(T32 -mthumb)
		set(FLOAT_ABI -mfloat-abi=soft) 
		set(BOARD -DSTM32F302x8)	
		set(LINKER_SCRIPT STM32F302R8Tx_FLASH.ld)
		set(HAL_BOARD_DIR STM32F3xx)
	
	else()
		continue()
	endif()

	set(TOTAL_TARGET_DEFS ${BOARD} ${TARGET_DEFS} ${TEST_DEFS})

	###  ASM FLAGS  ###
	set(MODULE_ASM_FLAGS ${MCU} ${FPU} ${T32} ${FLOAT_ABI})

	###  C FLAGS  ###
	set(MODULE_C_FLAGS ${TOTAL_TARGET_DEFS} ${MCU} ${FPU} ${T32} ${FLOAT_ABI} ${DEBUG_FLAGS} ${OPT_FLAGS} ${MISC_FLAGS})

	###  SOURCE FILES  ###
	# Get all the *.c files and *.s files
	file(GLOB_RECURSE SRC_C_FILES ${MODULE_DIR}/*.c)
	file(GLOB_RECURSE SRC_ASM_FILES ${MODULE_DIR}/*.s)


	# Create the project executable 
	add_executable(${exe_name} ${SRC_C_FILES} ${SRC_ASM_FILES} ${SHARED_UTIL_SRC})


	###  INCLUDE DIRECTORIES  ###
	target_include_directories(${exe_name} PRIVATE ${MODULE_DIR}/Inc)
	target_include_directories(${exe_name} PRIVATE ${MODULE_DIR}/Drivers/${HAL_BOARD_DIR}_HAL_Driver/Inc)
	target_include_directories(${exe_name} PRIVATE ${MODULE_DIR}/Drivers/${HAL_BOARD_DIR}_HAL_Driver/Inc/Legacy)
	target_include_directories(${exe_name} PRIVATE ${MODULE_DIR}/Drivers/CMSIS/Device/ST/${HAL_BOARD_DIR}/Include)
	target_include_directories(${exe_name} PRIVATE ${MODULE_DIR}/Drivers/CMSIS/Include)
	target_include_directories(${exe_name} PRIVATE shared/STM32-CAN-Code/Inc)

	### Add the compiler flags ###
	target_compile_options(${exe_name} PRIVATE $<$<COMPILE_LANGUAGE:C>:${MODULE_C_FLAGS}>)
	target_compile_options(${exe_name} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:${MODULE_ASM_FLAGS}>)
	
	#set_target_properties(${exe_name} PROPERTIES LINKER_LANGUAGE ASM)
	#target_compile_options(${exe_name} PRIVATE ${MODULE_ASM_FLAGS})
	#set_target_properties(${exe_name} PROPERTIES COMPILER_OPTIONS ${MODULE_C_FLAGS})
	#target_link_libraries(${exe_name} shared_lib)
	#set_source_files_properties(${SRC_ASM_FILES} PROPERTIES COMPILE_FLAGS ${MODULE_ASM_FLAGS})
	#set_source_files_properties(${SRC_C_FILES} PROPERTIES COMPILE_FLAGS ${MODULE_C_FLAGS})

	#SET(CMAKE_EXE_LINKER_FLAGS 
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

##############################################
###                                        ###
###           UNIT TEST FRAMEWORK          ###
###                                        ###
##############################################

if (TEST_BUILD)
	include(CTest)

	# Build the Unity Testing Framework
	file(GLOB UNITY_FILES Unity/src/*.c)
	add_library(unity_lib ${UNITY_FILES})
	target_include_directories(unity_lib PUBLIC Unity/src)

	# Grab the user-written unit tests
	file(GLOB TEST_SRC_FILES Tests/*.c)

	# Create an executable for each unit test 
	foreach(FILE ${TEST_SRC_FILES})	
		# Separate the file name from the path
		get_filename_component(file_name ${FILE} NAME)

		# Remove the .c extension from each file
		string(REPLACE ".c" "" test_name ${file_name})
		
		add_executable(${test_name} ${FILE})
		
		# Include the module source code 
		target_link_libraries(${test_name} module_lib unity_lib)

		# Define test linker options
		target_link_libraries(${test_name} 
		    "-Wl,--gc-sections \
		    -specs=nano.specs \
		    -specs=nosys.specs \
		    -mthumb \
		    ${MCU}"
		)
		
		# Create the test suite
		string(CONCAT test_suite ${test_name} "_suite")

		add_test(NAME ${test_suite} COMMAND ${test_name} WORKING_DIRECTORY Build) 
	endforeach()
endif()