include(../../cmake/embedded.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/nanopb/extra)
find_package(Nanopb REQUIRED)
nanopb_generate_cpp(VC_PROTO_SRCS VC_PROTO_HDRS VC.proto)
nanopb_generate_cpp(BMS_PROTO_SRCS BMS_PROTO_HDRS BMS.proto)
nanopb_generate_cpp(FSM_PROTO_SRCS FSM_PROTO_HDRS FSM.proto)
nanopb_generate_cpp(RSM_PROTO_SRCS RSM_PROTO_HDRS RSM.proto)
nanopb_generate_cpp(CRIT_PROTO_SRCS CRIT_PROTO_HDRS CRIT.proto)

set(HDRS ${CMAKE_CURRENT_BINARY_DIR} ${NANOPB_INCLUDE_DIRS})

embedded_library(
    "VC_debug_modules"
    "${VC_PROTO_SRCS}"
    "${HDRS}"
    "cm7"
    TRUE
)

embedded_library(
    "BMS_debug_modules"
    "${BMS_PROTO_SRCS}"
    "${HDRS}"
    "cm7"
    TRUE
)

embedded_library(
    "FSM_debug_modules"
    "${FSM_PROTO_SRCS}"
    "${HDRS}"
    "cm4"
    TRUE
)

embedded_library(
    "RSM_debug_modules"
    "${RSM_PROTO_SRCS}"
    "${HDRS}"
    "cm4"
    TRUE
)

embedded_library(
    "CRIT_debug_modules"
    "${CRIT_PROTO_SRCS}"
    "${HDRS}"
    "cm4"
    TRUE
)
