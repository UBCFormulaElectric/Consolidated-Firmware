# CMake generated Testfile for 
# Source directory: C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/hexray/VC
# Build directory: C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/build_fw_test_msvc/firmware/hexray/VC
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[hexray_VC_test]=] "C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/build_fw_test_msvc/firmware/hexray/VC/Debug/hexray_VC_test.exe")
  set_tests_properties([=[hexray_VC_test]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/test/CMakeLists.txt;92;add_test;C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/hexray/VC/CMakeLists.txt;205;compile_gtest_executable;C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/hexray/VC/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[hexray_VC_test]=] "C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/build_fw_test_msvc/firmware/hexray/VC/Release/hexray_VC_test.exe")
  set_tests_properties([=[hexray_VC_test]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/test/CMakeLists.txt;92;add_test;C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/hexray/VC/CMakeLists.txt;205;compile_gtest_executable;C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/hexray/VC/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[hexray_VC_test]=] "C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/build_fw_test_msvc/firmware/hexray/VC/MinSizeRel/hexray_VC_test.exe")
  set_tests_properties([=[hexray_VC_test]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/test/CMakeLists.txt;92;add_test;C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/hexray/VC/CMakeLists.txt;205;compile_gtest_executable;C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/hexray/VC/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[hexray_VC_test]=] "C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/build_fw_test_msvc/firmware/hexray/VC/RelWithDebInfo/hexray_VC_test.exe")
  set_tests_properties([=[hexray_VC_test]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/test/CMakeLists.txt;92;add_test;C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/hexray/VC/CMakeLists.txt;205;compile_gtest_executable;C:/Users/hitar/OneDrive/Desktop/UBC_FE/Consolidated-Firmware/firmware/hexray/VC/CMakeLists.txt;0;")
else()
  add_test([=[hexray_VC_test]=] NOT_AVAILABLE)
endif()
subdirs("src/app/torque_vectoring")
