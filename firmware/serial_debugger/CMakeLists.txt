include(../cmake/embedded.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/nanopb/extra)
find_package(Nanopb REQUIRED)
nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS debug_modules.proto)

set(SRCS ${PROTO_SRCS})
set(HDRS ${CMAKE_CURRENT_BINARY_DIR} ${NANOPB_INCLUDE_DIRS})

embedded_library(
    "debug_modules"
    "${SRCS}"
    "${HDRS}"
    "cm4"
    TRUE
)
