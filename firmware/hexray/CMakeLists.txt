add_subdirectory(CRIT)
add_subdirectory(DAM)
add_subdirectory(FSM)
add_subdirectory(RSM)

if ("${TARGET}" STREQUAL "binary")
    add_custom_target(hexray_hexes)
    add_dependencies(hexray_hexes
            hexray_CRIT_app.hex
            hexray_DAM_app.hex
            hexray_FSM_app.hex
            hexray_RSM_app.hex
    )
endif ()
