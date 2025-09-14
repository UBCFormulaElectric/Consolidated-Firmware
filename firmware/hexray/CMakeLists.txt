
add_subdirectory(FSM)

if ("${TARGET}" STREQUAL "binary")
    add_custom_target(hexray_hexes)
    add_dependencies(quintuna_hexes
            hexray_FSM.hex
    )
endif ()
