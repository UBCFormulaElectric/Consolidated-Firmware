set(THRUNA_DIR "${FIRMWARE_DIR}/thruna")
set(SHARED_THRUNA_DIR "${THRUNA_DIR}/shared")
set(SHARED_EMBEDDED_THRUNA_DIR "${SHARED_THRUNA_DIR}/src")

set(SHARED_APP_INCLUDE_THRUNA_DIR "${SHARED_EMBEDDED_THRUNA_DIR}/app")
set(SHARED_IO_INCLUDE_THRUNA_DIR "${SHARED_EMBEDDED_THRUNA_DIR}/io")
set(SHARED_HW_INCLUDE_THRUNA_DIR "${SHARED_EMBEDDED_THRUNA_DIR}/hw")

add_subdirectory(shared)
add_subdirectory(BMS)
add_subdirectory(DCM)
add_subdirectory(FSM)
add_subdirectory(PDM)
add_subdirectory(DIM)
