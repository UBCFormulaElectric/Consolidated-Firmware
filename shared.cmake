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
set(CAN_DIR ${CMAKE_SOURCE_DIR}/can_bus)

# C shared code
set(SHARED_EMBEDDED_DIR "${SHARED_DIR}/src")
set(SHARED_APP_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/app")
set(SHARED_IO_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/io")
set(SHARED_HW_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/hw")
set(SHARED_UTIL_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/util")
set(SHARED_FAKES_DIR "${SHARED_DIR}/src_fake")
# C++ shared code
set(SHARED_EMBEDDED_DIR_CPP "${SHARED_DIR}/srcpp")
set(SHARED_APP_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/app")
set(SHARED_IO_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/io")
set(SHARED_HW_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/hw")
set(SHARED_UTIL_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/util")
# code sources

# Test Utils
set(SHARED_TEST_UTILS_INCLUDE_DIRS "${SHARED_DIR}/test")

set(SHARED_GNU_COMPILER_CHECKS
        -Wall
        -Wextra
        -pedantic
        -Wdouble-promotion
        -Wshadow
        -Wundef
        -Wconversion
        -Wunused-variable
        -Wunused-parameter
        -pedantic-errors
        -Wfloat-equal
        -Wformat
        -Wformat=2
        -Wmissing-braces
        -Wmissing-format-attribute
        -Wmissing-include-dirs # disable for now?
        -Wmissing-noreturn
        -Wredundant-decls
        -Wswitch-default
        -Wswitch-enum
)
set(SHARED_GNU_COMPILER_CHECKS_STRICT
        -pedantic
        -pedantic-errors
        -Werror=all
        -Werror=extra
        -Werror=double-promotion
        -Werror=shadow
        -Werror=undef
        -Werror=conversion
        -Wno-error=unused-variable
        -Wunused-variable
        -Wno-error=unused-const-variable
        -Wunused-const-variable
        -Wno-error=unused-parameter
        -Wunused-parameter
        -Werror=float-equal
        -Werror=format
        -Werror=format=2
        -Werror=inline
        -Werror=missing-braces
        -Werror=missing-format-attribute
        -Werror=missing-include-dirs # disable for now?
        -Werror=missing-noreturn
        -Werror=redundant-decls
        -Werror=switch-default
        -Werror=switch-enum
        # perhaps we want to do reinterpret casts sometimes??
        # -Wcast-align
        # -Wcast-qual
        # requires instrumentation
        # -Wstack-protector
        # -Wstrict-aliasing
        # -Wstrict-aliasing=2
)

function(no_checks SRCS)
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
    message("  🚫 [shared.cmake, no_checks()] Disabling Warnings for ${SRCS_LENGTH} files under ${unique_dirs}")
    set_source_files_properties(
            ${SRCS}
            PROPERTIES COMPILE_FLAGS "-w"
    )
endfunction()
