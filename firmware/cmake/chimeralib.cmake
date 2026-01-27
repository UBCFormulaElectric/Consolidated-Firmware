message("")
message("⚙️ [chimeralib.cmake] Configuring Chimera Executable functions")

add_subdirectory(chimera_v2/proto)

message(" 🔃 Registered add_chimera_stm32f4() function")
function(add_chimera_stm32f4 NAME SRCS INCLUDE_DIRS)
    list(APPEND SRCS
            "${SHARED_CHIMERA_DIR}/chimera_v2.cpp"
            #            "${SHARED_HW_INCLUDE_DIR_CPP}/hw_usb.cpp"
    )
    list(APPEND INCLUDE_DIRS "${SHARED_CHIMERA_DIR}")
    embedded_binary(
            "${NAME}"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_boot.ld"
            "cm4"
    )
    target_link_libraries("${NAME}.elf" PRIVATE "chimera_v2_proto_cm4")
    target_compile_definitions("${NAME}.elf" PRIVATE "USE_CHIMERA")
endfunction()

message(" 🔃 Registered add_chimera_stm32h5() function")
function(add_chimera_stm32h5 NAME SRCS)
    list(APPEND SRCS
            "${SHARED_CHIMERA_DIR}/chimera_v2.cpp"
            #            "${SHARED_HW_INCLUDE_DIR_CPP}/hw_usb.cpp"
    )
    list(APPEND INCLUDE_DIRS "${SHARED_CHIMERA_DIR}")
    embedded_binary(
            "${NAME}"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_DIR}/stm32h562ritx/stm32h562ritx_boot.ld"
            "cm33"
    )
    target_link_libraries("${NAME}.elf" PRIVATE "chimera_v2_proto_cm7")
    target_compile_definitions("${NAME}.elf" PRIVATE "USE_CHIMERA")
endfunction()

message(" 🔃 Registered add_chimera_stm32h7() function")
function(add_chimera_stm32h7 NAME SRCS)
    list(APPEND SRCS
            "${SHARED_CHIMERA_DIR}/chimera_v2.cpp"
            #            "${SHARED_HW_INCLUDE_DIR_CPP}/hw_usb.cpp"
    )
    list(APPEND INCLUDE_DIRS "${SHARED_CHIMERA_DIR}")
    embedded_binary(
            "${NAME}"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_boot.ld"
            "cm7"
    )
    target_link_libraries("${NAME}.elf" PRIVATE "chimera_v2_proto_cm33")
    target_compile_definitions("${NAME}.elf" PRIVATE USE_CHIMERA)
endfunction()
