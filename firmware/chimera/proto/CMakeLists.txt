nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS shared.proto VC.proto BMS.proto FSM.proto RSM.proto CRIT.proto)

set(SRCS ${PROTO_SRCS})
set(HDRS ${CMAKE_CURRENT_BINARY_DIR} ${NANOPB_INCLUDE_DIRS})

embedded_library(
    "debug_modules"
    "${SRCS}"
    "${HDRS}"
    "cm4"
    TRUE
)
