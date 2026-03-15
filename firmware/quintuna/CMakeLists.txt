add_subdirectory(CRIT)
add_subdirectory(FSM)
add_subdirectory(DAM)
add_subdirectory(RSM)
add_subdirectory(VC)
add_subdirectory(VCR)
add_subdirectory(BMS)
add_subdirectory(PCM)

if ("${TARGET}" STREQUAL "binary")
    add_custom_target(quintuna_hexes)
    #    add_dependencies(quintuna_hexes
    #            quintuna_BMS.hex
    #            quintuna_CRIT.hex
    #            quintuna_DAM.hex
    #            quintuna_FSM.hex
    #            quintuna_RSM.hex
    #            quintuna_VC.hex
    #            quintuna_VCR_app.elf
    #            quintuna_PCM_app.elf
    #            f4dev.elf
    #            h7dev.hex
    #    )
endif ()
