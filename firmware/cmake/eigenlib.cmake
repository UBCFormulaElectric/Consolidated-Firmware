message("")
message("⚙️ [eigenlib.cmake] Configuring Eigen library")

# Ensure we have a path to Eigen headers (CPM should set EIGEN_SOURCE_DIR)
if (NOT DEFINED EIGEN_SOURCE_DIR)
    find_path(EIGEN_SOURCE_DIR Eigen/Dense)
endif()
set(EIGEN_INCLUDE_DIRS ${EIGEN_SOURCE_DIR})

# Create an internal INTERFACE target then provide the standard alias
# Avoid creating a namespaced target directly because that can trigger
# CMake reservations for package-managed targets.
if (NOT TARGET eigen_interface)
    add_library(eigen_interface INTERFACE)
    if (EIGEN_INCLUDE_DIRS)
        target_include_directories(eigen_interface SYSTEM INTERFACE ${EIGEN_INCLUDE_DIRS})
    endif()
    target_compile_definitions(eigen_interface INTERFACE
        EIGEN_STACK_ALLOCATION_LIMIT=0
        EIGEN_MAX_STATIC_ALIGN_BYTES=0
    )
endif()

# Provide the conventional package target name as an alias if it doesn't exist
if (NOT TARGET Eigen3::Eigen)
    add_library(Eigen3::Eigen ALIAS eigen_interface)
endif()
