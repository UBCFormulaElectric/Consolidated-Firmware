# in particular, this is shared between embedded and tests
message("")
message("⚙️ [shared.cmake] Configuring shared variables and functions")
set(SHARED_CMAKE_INCLUDED TRUE)

set(FIRMWARE_DIR "${CMAKE_SOURCE_DIR}/firmware")
set(LINKER_DIR "${FIRMWARE_DIR}/linker")
set(BOOT_DIR "${FIRMWARE_DIR}/boot")
set(SHARED_DIR "${FIRMWARE_DIR}/shared")
set(THIRD_PARTY_DIR "${FIRMWARE_DIR}/third_party")
set(TEST_DIR "${FIRMWARE_DIR}/test")
set(SCRIPTS_DIR "${CMAKE_SOURCE_DIR}/scripts")

# C shared code
set(SHARED_EMBEDDED_DIR "${SHARED_DIR}/src")
set(SHARED_APP_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/app")
set(SHARED_IO_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/io")
set(SHARED_HW_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/hw")
set(SHARED_FAKES_DIR "${SHARED_DIR}/src_fake")
# C++ shared code
set(SHARED_EMBEDDED_DIR_CPP "${SHARED_DIR}/srcpp")
set(SHARED_APP_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/app")
set(SHARED_IO_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/io")
set(SHARED_HW_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/hw")
# code sources
file(GLOB_RECURSE SHARED_APP_SRCS "${SHARED_APP_INCLUDE_DIR}/*.c")
file(GLOB_RECURSE SHARED_IO_SRCS "${SHARED_IO_INCLUDE_DIR}/*.c")
file(GLOB_RECURSE SHARED_HW_SRCS "${SHARED_HW_INCLUDE_DIR}/*.c")

# Test Utils
set(SHARED_TEST_UTILS_INCLUDE_DIRS "${SHARED_DIR}/test")

set(SHARED_GNU_COMPILER_CHECKS
        -Wall
        -Werror
        -Wextra
        -pedantic
        -Wdouble-promotion
        -Wshadow
        -Wundef
        -Wconversion
        -Wno-unused-variable
        -Wno-unused-parameter
        -pedantic-errors
        -Wfloat-equal
        -Wformat
        -Wformat=2
        -Winline
        -Wmissing-braces
        -Wmissing-format-attribute
        #        -Wmissing-include-dirs # disable for now?
        -Wmissing-noreturn
        -Wredundant-decls
        -Wswitch-default
        -Wswitch-enum
        # perhaps we want to do reinterpret casts sometimes??
        #        -Wcast-align
        #        -Wcast-qual
        # requires instrumentation
        # -Wstack-protector
        # -Wstrict-aliasing
        # -Wstrict-aliasing=2
)