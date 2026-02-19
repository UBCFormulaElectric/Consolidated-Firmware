message("")
message("Auto diff lib")

# Eigen configuration for stack-only allocation (no heap allocation)
# These flags prevent dynamic memory allocation in Eigen
set(EIGEN_FLAGS
    -DEIGEN_STACK_ALLOCATION_LIMIT=0  # Disable dynamic allocation
    -DEIGEN_MAX_STATIC_ALIGN_BYTES=0  # No static alignment overhead
)

set(AUTO_DIFF_INCLUDE_DIRS
  # Required by autodiff internal includes like <autodiff/common/...>
  "${AUTO_DIFF_SOURCE_DIR}"
  # so we can directly include <dual.hpp>
  "${AUTO_DIFF_SOURCE_DIR}/autodiff/forward"
  # so we can directly include <gradient.hpp>
  "${AUTO_DIFF_SOURCE_DIR}/autodiff/forward/utils"
)


add_library(autodiff_interface INTERFACE)
target_link_libraries(autodiff_interface INTERFACE Eigen3::Eigen)
target_include_directories(autodiff_interface SYSTEM INTERFACE ${AUTO_DIFF_INCLUDE_DIRS})
