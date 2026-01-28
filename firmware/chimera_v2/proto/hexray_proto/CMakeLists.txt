message("")
message("⚙️ Generating Hexray Chimera v2 Protobuf Sources")

# Generate srcs/headers.
nanopb_generate_cpp(
        PROTO_SRCS PROTO_HDRS
        shared.proto f4dev.proto crit.proto dam.proto rsm.proto fsm.proto bms.proto vc.proto
)

set(SRCS ${PROTO_SRCS})
set(HDRS ${CMAKE_CURRENT_BINARY_DIR} ${NANOPB_INCLUDE_DIRS})

# Register library.
message("  📚 Registering Hexray chimera_v2_proto_cm4/7 libraries")
embedded_library(
        "hexray_chimera_v2_proto_cm4"
        "${SRCS}"
        "${HDRS}"
        "cm4"
        TRUE
)

embedded_library(
        "hexray_chimera_v2_proto_cm7"
        "${SRCS}"
        "${HDRS}"
        "cm7"
        TRUE
)

embedded_library(
        "hexray_chimera_v2_proto_cm33"
        "${SRCS}"
        "${HDRS}"
        "cm33"
        TRUE
)