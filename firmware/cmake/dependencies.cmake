# Host system specific settings
IF (WIN32)
    set(ARM_COMPILER "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-win32.zip")
ELSEIF (APPLE)
    set(ARM_COMPILER "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-mac.tar.bz2")
ELSEIF (UNIX)
    set(ARM_COMPILER "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2")
ENDIF ()

# Git Submodules
execute_process(COMMAND "git submodule update --init --recursive" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
message("✅ Git Submodules Updated")

# setup pip
IF (WIN32)
    set(PIP_COMMAND pip)
ELSE ()
    set(PIP_COMMAND pip3)
ENDIF ()
find_program(HAS_PIP_COMMAND pip)
IF (NOT HAS_PIP_COMMAND)
    message(FATAL_ERROR "❌ Could not find pip. Please install pip and try again.")
ELSE ()
    message("📥 Found pip")
ENDIF ()
# python pip dependencies
IF (NO_VENV)
    message("📦 Update Python Dependencies")
    execute_process(COMMAND ${PIP_COMMAND} install -r ${CMAKE_SOURCE_DIR}/environment/requirements.txt
            RESULT_VARIABLE PIP_INSTALL_STATUS)
    IF (NOT ${PIP_INSTALL_STATUS} EQUAL 0)
        message(FATAL_ERROR "❌ Could not successfully install Python dependencies")
    ELSE ()
        message("✅ Python dependencies installed")
    ENDIF ()
ELSE ()
    execute_process(COMMAND ${PIP_COMMAND} install pipenv OUTPUT_QUIET)
    message("📥 Installed pipenv")
    execute_process(COMMAND pipenv install RESULT_VARIABLE PIPENV_STATUS OUTPUT_QUIET)
    IF (NOT ${PIPENV_STATUS} EQUAL 0)
        message(FATAL_ERROR "❌ Error setting up pipenv. Please install/fix pipenv and try again.")
    ENDIF()
    message("📦 Pipenv Dependencies Updated")
ENDIF ()

# arm toolchain
IF ("${TARGET}" STREQUAL "deploy")
    # arm binary
    CPMAddPackage(URL ${ARM_COMPILER} NAME gcc-arm-none-eabi-9 DOWNLOAD_ONLY)
    message("🔖 ARM Compiler Installed At ${gcc-arm-none-eabi-9_SOURCE_DIR}")
    set(TOOLCHAIN_PREFIX ${gcc-arm-none-eabi-9_SOURCE_DIR}/bin/)
    IF (WIN32)
        # openocd
        CPMAddPackage(URL "https://github.com/openocd-org/openocd/releases/download/v0.12.0/openocd-v0.12.0-i686-w64-mingw32.tar.gz" NAME openocd DOWNLOAD_ONLY)
        message("🔖 OpenOCD Installed At ${openocd_SOURCE_DIR}/bin/openocd.exe")
        # zadig
        IF (NOT EXISTS ${CMAKE_SOURCE_DIR}/zadig.exe)
            message(DEBUG "Zadig does not exist, installing now...")
            file(
                    DOWNLOAD
                    https://github.com/pbatard/libwdi/releases/download/v1.5.0/zadig-2.8.exe
                    ${CMAKE_SOURCE_DIR}/zadig.exe
            )
        ENDIF ()
        message("🔖 Zadig has been installed in the root directory")
    ENDIF ()
ENDIF ()
