# Shared fakes, used in test fixtures to test all boards
set(HDRS_TO_FAKE
        "${SHARED_IO_INCLUDE_DIR}/io_time.h"
)
create_fake_library(
        "shared_fakes"
        "${HDRS_TO_FAKE}"
)
target_include_directories("shared_fakes" PRIVATE "${SHARED_IO_INCLUDE_DIR}")
target_include_directories("shared_fakes" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
