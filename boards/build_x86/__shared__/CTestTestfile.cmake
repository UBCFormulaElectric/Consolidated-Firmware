# CMake generated Testfile for 
# Source directory: /usr/src/fe-firmware/boards/shared
# Build directory: /usr/src/fe-firmware/boards/build_x86/__shared__
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(SHARED_test "/usr/src/fe-firmware/boards/build_x86/__shared__/SHARED_test")
set_tests_properties(SHARED_test PROPERTIES  _BACKTRACE_TRIPLES "/usr/src/fe-firmware/boards/CMakeLists.txt;312;add_test;/usr/src/fe-firmware/boards/CMakeLists.txt;487;compile_googletest_executable;/usr/src/fe-firmware/boards/shared/CMakeLists.txt;94;create_arm_binary_or_tests_for_board;/usr/src/fe-firmware/boards/shared/CMakeLists.txt;0;")
subdirs("third_party/profiler-cortex-m4")
subdirs("third_party/TraceRecorder")
subdirs("third_party/SEGGER_RTT")
