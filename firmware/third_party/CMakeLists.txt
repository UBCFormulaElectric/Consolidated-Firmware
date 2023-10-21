include(../cmake/embedded.cmake)

# Import include directories and source files for Cortex M4 profiler
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/profiler-cortex-m4)
list(APPEND SHARED_IO_INCLUDE_DIRS ${PROFILER_INCLUDE_DIRS})
file(GLOB_RECURSE PROFILER_SRCS ${PROFILER_BASE_DIR}/*.c)
list(APPEND SHARED_IO_SRCS ${PROFILER_SRCS})

# Import sources files for SBG Systems library
set(SBG_ECOM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sbgECom)
list(APPEND SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_INCLUDE_DIRS ${SBG_ECOM_DIR}/common ${SBG_ECOM_DIR}/src)
file(GLOB_RECURSE SBG_ECOM_SRCS
    ${SBG_ECOM_DIR}/src/*.c
    ${SBG_ECOM_DIR}/common/crc/*.c
    ${SBG_ECOM_DIR}/common/platform/*.c
    ${SBG_ECOM_DIR}/common/interfaces/sbgInterface.c
)
list(APPEND SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_SRCS ${SBG_ECOM_SRCS})

# Expose the following variables to the parent scope (i.e. The scope of any
# other CMakeLists.txt that uses add_subdirectory() on this CMakeLists.txt).
set(SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_SRCS
    ${SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_SRCS}
    PARENT_SCOPE)
set(SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_INCLUDE_DIRS
    ${SHARED_ARM_BINARY_X86_INCOMPATIBLE_THIRD_PARTY_INCLUDE_DIRS}
    PARENT_SCOPE)
set(SHARED_GOOGLETEST_TEST_THIRD_PARTY_INCLUDE_DIRS
    ${SHARED_GOOGLETEST_TEST_THIRD_PARTY_INCLUDE_DIRS}
    PARENT_SCOPE)
