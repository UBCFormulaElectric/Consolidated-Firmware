# Get sources files and build static library
file(GLOB_RECURSE SOURCES "./*.*")
add_library (SEGGER_RTT STATIC ${SOURCES})
