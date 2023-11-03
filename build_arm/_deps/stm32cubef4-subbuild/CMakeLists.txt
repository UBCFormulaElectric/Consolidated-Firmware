# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.25.0)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(stm32cubef4-populate NONE)


# Pass through things we've already detected in the main project to avoid
# paying the cost of redetecting them again in ExternalProject_Add()
set(GIT_EXECUTABLE [==[/usr/bin/git]==])
set(GIT_VERSION_STRING [==[2.39.3 (Apple Git-145)]==])
set_property(GLOBAL PROPERTY _CMAKE_FindGit_GIT_EXECUTABLE_VERSION
  [==[/usr/bin/git;2.39.3 (Apple Git-145)]==]
)


include(ExternalProject)
ExternalProject_Add(stm32cubef4-populate
                     "UPDATE_DISCONNECTED" "False" "GIT_REPOSITORY" "https://github.com/UBCFormulaElectric/STM32CubeF4.git" "GIT_TAG" "3e907f8"
                    SOURCE_DIR          "/Users/jugveersandher/UBC/UbcFormulaE/Consolidated-Firmware/build_arm/_deps/stm32cubef4-src"
                    BINARY_DIR          "/Users/jugveersandher/UBC/UbcFormulaE/Consolidated-Firmware/build_arm/_deps/stm32cubef4-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


