# Checks for
# - Python
# - Pip
# - Pipenv if using virtual environment (option below)
# OUTPUTS
# PYTHON_COMMAND: The command to run python scripts
# PIP_COMMAND: The command to run pip

message("")
message("🐍 Python Configuration")

# ====== Generate PYTHON_COMMAND ======
find_package(Python3 3.10.0...3.14.5 COMPONENTS Interpreter REQUIRED)
message("  ℹ️ Found Python ${Python3_VERSION}")
set(PYTHON_COMMAND ${Python3_EXECUTABLE})

find_program(PIPENV_COMMAND pipenv)
IF (PIPENV_COMMAND)
    # check that pipenv exists
    execute_process(COMMAND ${PIPENV_COMMAND} --venv
            OUTPUT_VARIABLE FIND_PIPENV_OUTPUT
            RESULT_VARIABLE PIPENV_VALID_CMD_RESULT
            ERROR_VARIABLE FIND_PIPENV_ERROR
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE)
ELSE ()
    set(PIPENV_VALID_CMD_RESULT 1)
ENDIF ()

IF (${PIPENV_VALID_CMD_RESULT} EQUAL 0)
    message("  ℹ️ Found pipenv: ${PIPENV_COMMAND}")
    message("  📟 Using pipenv command: ${PIPENV_COMMAND}")

    # Determine the python command to use within pipenv
    IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
        set(PYTHON_BASE_COMMAND python)
    ELSEIF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
        set(PYTHON_BASE_COMMAND python3)
    ELSEIF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
        set(PYTHON_BASE_COMMAND python3)
    ELSE ()
        message(FATAL_ERROR "❌ Unsupported host system: ${CMAKE_HOST_SYSTEM_NAME}")
    ENDIF ()
    set(PYTHON_COMMAND ${PIPENV_COMMAND} run ${PYTHON_BASE_COMMAND})

    message("  🛣 Pipenv path: ${FIND_PIPENV_OUTPUT}")

    # check that dependencies are installed with pipenv
    message("  📦 Updating Pipenv dependencies...")
    execute_process(COMMAND ${PIPENV_COMMAND} sync RESULT_VARIABLE PIPENV_STATUS OUTPUT_QUIET)
    IF (NOT ${PIPENV_STATUS} EQUAL 0)
        message(FATAL_ERROR "❌ Error setting up pipenv. Please install/fix pipenv and try again.")
    ENDIF ()
    message("  📦 Pipenv Dependencies Updated")
ELSE ()
    message("   ❌ pipenv not found. If you would like to use a virtual environment, install it with \"pip install pipenv\"")
    message("  🔍 Finding pip...")
    set(PIP_COMMAND ${PYTHON_COMMAND} -m pip)
    execute_process(COMMAND ${PIP_COMMAND} --version RESULT_VARIABLE PIP_CHECK_STATUS)
    IF (NOT ${PIP_CHECK_STATUS} EQUAL 0)
        message(FATAL_ERROR "❌ Could not find pip. Please install pip and try again.")
    ENDIF ()
    message("  📟 Using pip with \"${PIP_COMMAND}\"")

    execute_process(COMMAND ${PIP_COMMAND} install -r ${CMAKE_SOURCE_DIR}/environment/requirements.txt
            RESULT_VARIABLE PIP_INSTALL_STATUS
            COMMAND_ERROR_IS_FATAL ANY
    )
    IF (NOT ${PIP_INSTALL_STATUS} EQUAL 0)
        message(FATAL_ERROR "❌ Could not successfully install Python dependencies")
    ENDIF ()
    message("  📦 Pip packages collected")
ENDIF ()
message("  📟 Using Python Command: \"${PYTHON_COMMAND}\"")
message("  ✅ Python dependencies installed")
