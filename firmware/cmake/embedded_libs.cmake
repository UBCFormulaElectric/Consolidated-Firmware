message("")
message("⚙️ [embedded_libs.cmake] Configuring library wrappers")
set(EMBEDDED_LIBS_INCLUDED TRUE)

# Generate library with header file for commit message
message("  🔃 Registered commit_info_library() function")
function(commit_info_library
        BIND_TARGET
        LIB_NAME
        OUTPUT_PATH
)
    commit_info_generate_sources(${BIND_TARGET} ${OUTPUT_PATH})
    embedded_interface_library(
            "${LIB_NAME}"
            "${COMMIT_INFO_SRC}"
            "${COMMIT_INFO_INCLUDE_DIR}"
            FALSE
    )
endfunction()

set(CAN_DIR ${CMAKE_SOURCE_DIR}/can_bus)

# Generates library ${CAR}_${BOARD}_jsoncan
message("  🔃 Registered jsoncan_library() function")
function(jsoncan_embedded_library BOARD CAR JSONCAN_DIR)
    jsoncan_sources(
            ${BOARD}
            ${JSONCAN_DIR}
            TRUE
            "${CAN_DIR}/dbcs/${CAR}.dbc"
            "${CAN_DIR}/${CAR}"
    )
    embedded_interface_library(
            "${CAR}_${BOARD}_jsoncan"
            "${CAN_SRCS}"
            "${CAN_INCLUDE_DIRS}"
            TRUE
    )
endfunction()