# Expose the following include directories to the parent scope so TraceRecorder
# can compile without errors
list(APPEND SEGGER_RTT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/)
set(SEGGER_RTT_INCLUDE_DIRS ${SEGGER_RTT_INCLUDE_DIRS} PARENT_SCOPE)

# Compile static library
file(GLOB_RECURSE SOURCES "./*.*")
add_library(SEGGER_RTT STATIC ${SOURCES})
target_compile_options(SEGGER_RTT
    PUBLIC
        ${FPU_FLAGS}
    )
target_link_options(SEGGER_RTT
    PUBLIC
        ${FPU_FLAGS}
    )
