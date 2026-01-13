message("")
message("⚙️ [embedded_libs.cmake] Configuring library wrappers")
IF (NOT "${EMBEDDED_CMAKE_INCLUDED}" STREQUAL "TRUE")
    message(FATAL_ERROR "❌ embedded.cmake must be included before stmlib.cmake")
ENDIF ()
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

# ubsan is treated like a third party library
embedded_object_library("ubsan_cm4" "${SHARED_HW_INCLUDE_DIR_CPP}/hw_ubsan.cpp" "" TRUE "cm4")
embedded_object_library("ubsan_cm7" "${SHARED_HW_INCLUDE_DIR_CPP}/hw_ubsan.cpp" "" TRUE "cm7")
embedded_object_library("ubsan_cm33" "${SHARED_HW_INCLUDE_DIR_CPP}/hw_ubsan.cpp" "" TRUE "cm33")
message("  🔃 Registered target_link_ubsan() function")
function(target_link_ubsan ELF_NAME ARM_CORE)
    IF ("${ARM_CORE}" STREQUAL "cm4")
        target_link_libraries(${ELF_NAME} PRIVATE ubsan_cm4)
    ELSEIF ("${ARM_CORE}" STREQUAL "cm33")
        target_link_libraries(${ELF_NAME} PRIVATE ubsan_cm33)
    ELSEIF ("${ARM_CORE}" STREQUAL "cm7")
        target_link_libraries(${ELF_NAME} PRIVATE ubsan_cm7)
    ELSE ()
        message(FATAL_ERROR "❌ Unsupported ARM core: ${ARM_CORE}")
    endif ()
endfunction()
