#set(QUINTUNA_DIR "${FIRMWARE_DIR}/quintuna")
#set(SHARED_QUINTUNA_DIR "${QUINTUNA_DIR}/shared")
#set(SHARED_EMBEDDED_QUINTUNA_DIR "${SHARED_QUINTUNA_DIR}/src")
#set(SHARED_CAR "quintuna")
#set(SHARED_APP_INCLUDE_QUINTUNA_DIR "${SHARED_EMBEDDED_QUINTUNA_DIR}/app")
#set(SHARED_IO_INCLUDE_QUINTUNA_DIR "${SHARED_EMBEDDED_QUINTUNA_DIR}/io")
#set(SHARED_HW_INCLUDE_QUINTUNA_DIR "${SHARED_EMBEDDED_QUINTUNA_DIR}/hw")

add_subdirectory(CRIT)
add_subdirectory(FSM)
add_subdirectory(DAM)
add_subdirectory(RSM)
add_subdirectory(VC)
add_subdirectory(BMS)

if ("${TARGET}" STREQUAL "binary")
    add_custom_target(quintuna_hexes)
    add_dependencies(quintuna_hexes
        quintuna_BMS.hex
        quintuna_CRIT.hex
        quintuna_DAM.hex
        quintuna_FSM.hex
        quintuna_RSM.hex
        quintuna_VC.hex
        f4dev.elf
        h7dev.hex
    )
endif()
