# Builds libdimos_jsoncan: the Rust jsoncan (cppcodegen) RX/data-capture code for
# the DIMOS node, wrapped in a C API (jsoncan_bridge) that Dart loads via FFI.
#
# Code is generated at build time from can_bus/${DIMOS_CAR}. By default the
# cppcodegen binary is built with cargo; pass -DJSONCAN_BINARY_GENERATE=OFF to
# use a cppcodegen already on PATH instead (e.g. in a Yocto build).
cmake_minimum_required(VERSION 3.16)
project(dimos_jsoncan LANGUAGES CXX)

set(DIMOS_CAR "hexray" CACHE STRING "Car whose CAN JSON (can_bus/<car>) the dashboard decodes")

get_filename_component(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)
set(SCRIPTS_DIR "${REPO_ROOT}/scripts")
set(CAN_JSON_DIR "${REPO_ROOT}/can_bus/${DIMOS_CAR}")
set(CAR "${DIMOS_CAR}")

if (NOT EXISTS "${CAN_JSON_DIR}/DIMOS")
    message(FATAL_ERROR "❌ ${CAN_JSON_DIR} has no DIMOS node. Add can_bus/${DIMOS_CAR}/DIMOS (see can_bus/hexray/DIMOS).")
endif ()

include("${SCRIPTS_DIR}/code_generation/jsoncan-rust/jsoncan.cmake")
jsoncan_sources_cpp(
        DIMOS
        "${CMAKE_CURRENT_BINARY_DIR}/jsoncan"
        TRUE
        "${CMAKE_CURRENT_BINARY_DIR}/jsoncan/${DIMOS_CAR}.dbc"
        "${CAN_JSON_DIR}"
)
# The dashboard only decodes and captures; it never transmits, so skip the TX/reroute sources.
list(FILTER CAN_SRCS INCLUDE REGEX "/(app_canUtils|app_canRx|app_canDataCapture|io_canRx)\\.cpp$")

add_library(dimos_jsoncan SHARED
        "${CMAKE_CURRENT_SOURCE_DIR}/jsoncan_bridge/jsoncan_bridge.cpp"
        ${CAN_SRCS}
)
set_target_properties(dimos_jsoncan PROPERTIES OUTPUT_NAME "dimos_jsoncan")
target_compile_features(dimos_jsoncan PRIVATE cxx_std_20)
target_include_directories(dimos_jsoncan PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/jsoncan_bridge"
        ${CAN_INCLUDE_DIRS}
)
