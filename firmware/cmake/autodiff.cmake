message("")
message("Auto diff lib")

# Eigen configuration for stack-only allocation (no heap allocation)
# These flags prevent dynamic memory allocation in Eigen
set(EIGEN_FLAGS
    -DEIGEN_STACK_ALLOCATION_LIMIT=0  # Disable dynamic allocation
    -DEIGEN_MAX_STATIC_ALIGN_BYTES=0  # No static alignment overhead
)

if (NOT DEFINED AUTO_DIFF_SOURCE_DIR)
  find_path(AUTO_DIFF_SOURCE_DIR autodiff/forward/dual.hpp)
endif()

if (NOT AUTO_DIFF_SOURCE_DIR)
  message(FATAL_ERROR "AUTO_DIFF_SOURCE_DIR not set or autodiff headers not found. Fetch autodiff or set AUTO_DIFF_SOURCE_DIR.")
endif()

set(AUTO_DIFF_INCLUDE_DIRS
  # Required by autodiff internal includes like <autodiff/common/...>
  "${AUTO_DIFF_SOURCE_DIR}"
  # so we can directly include <dual.hpp>
  "${AUTO_DIFF_SOURCE_DIR}/autodiff/forward"
  # so we can directly include <gradient.hpp>
  "${AUTO_DIFF_SOURCE_DIR}/autodiff/forward/utils"
)

if (NOT TARGET autodiff::autodiff)
  if(NOT TARGET autodiff_interface)
    add_library(autodiff_interface INTERFACE)
    target_link_libraries(autodiff_interface INTERFACE Eigen3::Eigen)
    target_include_directories(autodiff_interface SYSTEM INTERFACE ${AUTO_DIFF_INCLUDE_DIRS})
  endif()
endif()

add_library(autodiff::autodiff ALIAS autodiff_interface)
