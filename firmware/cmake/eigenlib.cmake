message("")
message("⚙️ [eigenlib.cmake] Configuring Eigen library")

set(EIGEN_INCLUDE_DIRS ${EIGEN_SOURCE_DIR})

add_library(eigen_interface INTERFACE)
target_include_directories(eigen_interface SYSTEM INTERFACE ${EIGEN_INCLUDE_DIRS})
target_compile_definitions(eigen_interface INTERFACE
    EIGEN_STACK_ALLOCATION_LIMIT=0
    EIGEN_MAX_STATIC_ALIGN_BYTES=0
    EIGEN_NO_MALLOC
)

# Strip Eigen's runtime asserts on target: each one stores a huge __PRETTY_FUNCTION__ string in flash.
# Host tests keep them to catch dimension/index bugs.
IF ("${TARGET}" STREQUAL "binary")
    target_compile_definitions(eigen_interface INTERFACE EIGEN_NO_DEBUG)
ENDIF ()

# Provide the conventional package target name as an alias if it doesn't exist
if (NOT TARGET Eigen3::Eigen)
    add_library(Eigen3::Eigen ALIAS eigen_interface)
endif()
