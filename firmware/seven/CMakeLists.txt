add_subdirectory(BMS)
add_subdirectory(DAM)
add_subdirectory(FSM)
add_subdirectory(RSM)
add_subdirectory(VC)

if ("${TARGET}" STREQUAL "binary")
    add_custom_target(seven_hexes)
    add_dependencies(seven_hexes
            seven_BMS_app.hex
            seven_DAM_app.hex
            seven_FSM_app.hex
            seven_RSM_app.hex
            seven_VC_app.hex
    )
endif ()
