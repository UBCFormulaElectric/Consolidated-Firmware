# Shared fakes, used in test fixtures to test all boards
set(HDRS_TO_FAKE
        "${SHARED_IO_INCLUDE_DIR}/io_time.h"
)
create_fake_library(
        "shared_fakes"
        "${HDRS_TO_FAKE}"
)
