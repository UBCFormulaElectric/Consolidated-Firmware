# Consolidated Firmware setup for Windows Machines
## Installing Dependencies
* GNU Make: http://gnuwin32.sourceforge.net/packages/make.htm
* CMake: 
  * https://cmake.org/download/, and find "Windows x64 Installer" (or "Windows i386 Installer" if you are running 32 bit Windows??????)
  * https://cmake.org/install/
* Python 3+ (*Python < 3 will NOT work*): https://www.python.org/downloads/
* ARM GNU Embedded Toolchain: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
  * Check `/environment/scripts/install_gcc_arm_none_eabi.sh` for which version to download
* STM32CubeMX: https://www.st.com/en/development-tools/stm32cubemx.html (MUST be installed in '\Program Files\STMicroelectronics\STM32Cube\STM32CubeMX")
* MinGW: https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download (**select 32-bit verison/i686 architecture**)
* Java: https://www.java.com/download/ie_manual.jsp
    * Java 11 is known to work
## Setting up the Python Virtual Environment
```shell
pipenv install
pipenv shell # activate the virtual environment
```
## Pulling all large files and submodules
```shell
git lfs pull
git submodule update --init --recursive
```
## Clion Configurations
### Toolchains
```
Name: min_gw
Toolset:
CMake: Bundled (or your own)
Build Tool: ninja.exe (not selected, greyed out)
C Compiler: gcc.exe (not selected, greyed out)
C++ Compiler: c++.exe (not selected, greyed out)
Debugger: Bundled GDB

Name: arm
Toolset: C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32
CMake: Bundled (or your own)
Build Tool: C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\mingw32-make.exe
C Compiler: C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\gcc.exe
C++ Compiler: C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\g++.exe
Debugger: C:\Program Files (x86)\GNU Tools Arm Embedded\9 2019-q4-major\bin\arm-none-eabi-gdb.exe
```
### CMake Profiles
```
// First CMake profile:
Name: embedded
Build type: Debug
CMake options: -DPLATFORM=arm
Toolchain: arm

// Second CMake profile:
Name: gtest
Build type: Debug
CMake options: -DPLATFORM=x86
Toolchain: min_gw
```