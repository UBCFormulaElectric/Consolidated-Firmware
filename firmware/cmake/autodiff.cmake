message("")
message("Auto diff lib")

set(AUTO_DIFF_INCLUDE_DIRS
    "${AUTO_DIFF_SOURCE_DIR}/autodiff/forward"
    "${AUTO_DIFF_SOURCE_DIR}/autodiff/reverse"
)

# Dummy source file as library is header only

set(AUTO_DIFF_DUMMY_SRC "${CMAKE_BINARY_DIR}/_autodiff_dummy.cpp")
if (NOT EXISTS "${AUTO_DIFF_DUMMY_SRC}")
  file(WRITE "${AUTO_DIFF_DUMMY_SRC}" "// dummy TU for header-only autodiff\n")
endif()

embedded_library(
  "auto_diff_cm4"
  "${AUTO_DIFF_DUMMY_SRC}"
  "${AUTO_DIFF_INCLUDE_DIRS}"
  "cm4"
  TRUE
)

embedded_library(
  "auto_diff_cm7"
  "${AUTO_DIFF_DUMMY_SRC}"
  "${AUTO_DIFF_INCLUDE_DIRS}"
  "cm7"
  TRUE
)