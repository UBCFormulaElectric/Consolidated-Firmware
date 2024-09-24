# Checks for
# - Python
# - Pip
# - Pipenv if using virtual environment (option below)
# OUTPUTS
# PYTHON_COMMAND: The command to run python scripts
# PIP_COMMAND: The command to run pip

option(NO_VENV "Disable invoking python scripts from virtual environment" OFF)

message("")
message("🐍 Python Configuration")

# ====== Generate PYTHON_COMMAND ======
IF (NO_VENV)
    find_package(Python3 3.10.0...3.11.12 COMPONENTS Interpreter REQUIRED)
    message("  ℹ️ Found Python ${Python3_VERSION}")
    set(PYTHON_COMMAND ${Python3_EXECUTABLE})
ELSE ()
    IF (WIN32)
        set(PYTHON_RAW_COMMAND python)
    ELSE ()
        set(PYTHON_RAW_COMMAND python3)
    ENDIF ()
    set(PYTHON_COMMAND pipenv run ${PYTHON_RAW_COMMAND})
ENDIF ()
message("  📟 Using Python Command: \"${PYTHON_COMMAND}\"")

# ============== PIP PACKAGES and PIPENV ==============
IF (NO_VENV)
    message("  📦 Update Python Dependencies")
    find_program(PIP_COMMAND pip REQUIRED)
    IF (NOT PIP_COMMAND)
        message(FATAL_ERROR "❌ Could not find pip. Please install pip and try again.")
    ENDIF ()
    message("  🔎 Found pip at \"${PIP_COMMAND}\"")

    execute_process(COMMAND ${PIP_COMMAND} install -r ${CMAKE_SOURCE_DIR}/environment/requirements.txt
            RESULT_VARIABLE PIP_INSTALL_STATUS)
    IF (NOT ${PIP_INSTALL_STATUS} EQUAL 0)
        message(FATAL_ERROR "❌ Could not successfully install Python dependencies")
    ELSE ()
        message("✅ Python dependencies installed")
    ENDIF ()
ELSE ()
    # check that pipenv exists
    find_program(HAS_PIPENV_COMMAND pipenv)
    IF (NOT HAS_PIPENV_COMMAND)
        message(FATAL_ERROR "❌ pipenv not found. Please install it with \"pip install pipenv\"")
    ENDIF ()
    execute_process(COMMAND pipenv --venv
            OUTPUT_VARIABLE PIPENV_OUTPUT
            RESULT_VARIABLE PIPENV_RESULT
            ERROR_VARIABLE PIPENV_ERROR
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE)
    IF(${PIPENV_RESULT})
        message(FATAL_ERROR "Pipenv path report error: ${PIPENV_RESULT} ${PIPENV_ERROR}
        Make sure that you have ran \"pipenv install\" in the root directory")
    ELSE()
        message("  🛣 Pipenv path: ${PIPENV_OUTPUT}")
    ENDIF()

    # check that dependencies are installed with pipenv
    execute_process(COMMAND pipenv install RESULT_VARIABLE PIPENV_STATUS OUTPUT_QUIET)
    IF (NOT ${PIPENV_STATUS} EQUAL 0)
        message(FATAL_ERROR "❌ Error setting up pipenv. Please install/fix pipenv and try again.")
    ENDIF ()
    message("  📦 Pipenv Dependencies Updated")
ENDIF ()
