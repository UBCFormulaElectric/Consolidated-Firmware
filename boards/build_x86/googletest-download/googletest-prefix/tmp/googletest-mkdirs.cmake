# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/usr/src/fe-firmware/boards/build_x86/googletest-src"
  "/usr/src/fe-firmware/boards/build_x86/googletest-build"
  "/usr/src/fe-firmware/boards/build_x86/googletest-download/googletest-prefix"
  "/usr/src/fe-firmware/boards/build_x86/googletest-download/googletest-prefix/tmp"
  "/usr/src/fe-firmware/boards/build_x86/googletest-download/googletest-prefix/src/googletest-stamp"
  "/usr/src/fe-firmware/boards/build_x86/googletest-download/googletest-prefix/src"
  "/usr/src/fe-firmware/boards/build_x86/googletest-download/googletest-prefix/src/googletest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/usr/src/fe-firmware/boards/build_x86/googletest-download/googletest-prefix/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/usr/src/fe-firmware/boards/build_x86/googletest-download/googletest-prefix/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
