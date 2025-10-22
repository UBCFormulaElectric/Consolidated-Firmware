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
    add_library(${LIB_NAME} INTERFACE)
    target_sources(${LIB_NAME} INTERFACE ${COMMIT_INFO_SRC})
    target_include_directories(${LIB_NAME} INTERFACE ${COMMIT_INFO_INCLUDE_DIR})
endfunction()

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
    add_library("${CAR}_${BOARD}_jsoncan" INTERFACE)
    target_sources("${CAR}_${BOARD}_jsoncan" INTERFACE ${CAN_SRCS})
    no_checks("${CAN_SRCS}")

    target_include_directories("${CAR}_${BOARD}_jsoncan" SYSTEM INTERFACE ${CAN_INCLUDE_DIRS})
endfunction()