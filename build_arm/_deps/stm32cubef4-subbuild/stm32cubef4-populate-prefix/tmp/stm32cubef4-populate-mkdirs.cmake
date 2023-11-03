# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/jugveersandher/UBC/UbcFormulaE/Consolidated-Firmware/build_arm/_deps/stm32cubef4-src"
  "/Users/jugveersandher/UBC/UbcFormulaE/Consolidated-Firmware/build_arm/_deps/stm32cubef4-build"
  "/Users/jugveersandher/UBC/UbcFormulaE/Consolidated-Firmware/build_arm/_deps/stm32cubef4-subbuild/stm32cubef4-populate-prefix"
  "/Users/jugveersandher/UBC/UbcFormulaE/Consolidated-Firmware/build_arm/_deps/stm32cubef4-subbuild/stm32cubef4-populate-prefix/tmp"
  "/Users/jugveersandher/UBC/UbcFormulaE/Consolidated-Firmware/build_arm/_deps/stm32cubef4-subbuild/stm32cubef4-populate-prefix/src/stm32cubef4-populate-stamp"
  "/Users/jugveersandher/UBC/UbcFormulaE/Consolidated-Firmware/build_arm/_deps/stm32cubef4-subbuild/stm32cubef4-populate-prefix/src"
  "/Users/jugveersandher/UBC/UbcFormulaE/Consolidated-Firmware/build_arm/_deps/stm32cubef4-subbuild/stm32cubef4-populate-prefix/src/stm32cubef4-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/jugveersandher/UBC/UbcFormulaE/Consolidated-Firmware/build_arm/_deps/stm32cubef4-subbuild/stm32cubef4-populate-prefix/src/stm32cubef4-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/jugveersandher/UBC/UbcFormulaE/Consolidated-Firmware/build_arm/_deps/stm32cubef4-subbuild/stm32cubef4-populate-prefix/src/stm32cubef4-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
