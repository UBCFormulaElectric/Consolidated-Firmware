# Native Environment Setup

The following is a guide to setup the build system on native Linux, Windows, and Mac, using CLion as the chosen IDE. This was the main workflow until September 2023, where it was deprecated in favour of containerizing the build system with Docker.

**Warning:** This guide may not be 100% accurate, especially on Windows and Mac.

## Table of Contents
- [Environment Setup](#environment-setup)
  - [Environment Dependencies](#environment-dependencies)
  - [Python Package Dependencies](#python-package-dependencies)
  - [STM32CubeMX](#stm32cubemx)
  - [CLion](#clion)

## Environment Setup
### Environment Dependencies
Follow these steps to compile and debug code in **CLion**:

#### 1. Install Dependencies:
##### Ubuntu 18.04
You need to install `git lfs` following the instructions here: https://git-lfs.github.com/. Don't forget to run `git lfs install` afterwards.

Then, `cd` to the root directory of the repo and run the following commands:
```
pipenv install
pipenv shell
git lfs pull
environment/scripts/install_gcc_arm_none_eabi.sh /usr/local
python environment/scripts/install_stm32cubemx.py /usr/local/STM32CubeMX ./tools/en.STM32CubeMX_v5-3-0.zip
sudo apt-get install openocd
```
##### Windows
  * GNU Make: http://gnuwin32.sourceforge.net/packages/make.htm
  * CMake: https://cmake.org/install/
  * Python 3+ (*Python < 3 will NOT work*): https://www.python.org/downloads/
  * ARM GNU Embedded Toolchain: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads (Check `install_gcc_arm_none_eabi.sh` for which version to download)
  * STM32CubeMX: https://www.st.com/en/development-tools/stm32cubemx.html (MUST be installed in '\Program Files\STMicroelectronics\STM32Cube\STM32CubeMX")
  * MinGW: https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download (**select 32-bit verison/i686 architecture**)
  * Java: https://www.java.com/download/ie_manual.jsp
	  * **MAKE SURE YOU HAVE JAVA 8, otherwise STM32CubeMX will not run!

##### Mac OS
First, install Homebrew and CLion. Then install the following programs using Homebrew:
```
brew install git-lfs pipenv openocd gcc
brew install --cask clion
```
Verify the programs above were installed by running `<program name> --version`.
Now, we need to install the `arm-none-eabi` compiler toolchain for ARM processors. To avoid bugs associated with different versions, go to the [ARM website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) and download the *9-2019-q4-major* version (link [here](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads#:~:text=gcc-arm-none-eabi-9-2019-q4-major-mac.tar.bz2)).

##### All Operating Systems
Then, set up the repo:
```
git lfs install
git clone <repo link>
cd Consolidated-Firmware
pipenv install
pipenv shell
git lfs pull
```
Install cube: `python3 environment/scripts/install_stm32cubemx.py /usr/local/STM32CubeMX ./tools/en.STM32CubeMX_v5-3-0.zip`
  * J-Link Software and Documentation Pack: https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack
    ##### Submodules:
    In the root of the directory run the following command:
    ```
    git submodule update --init --recursive
    ```
    to fetch all of the submodules.

#### 2. Modify your `PATH` Environment Variable:
##### Ubuntu 18.04 and Mac OS
Modify the `PATH` variable by adding the following to your `.bashrc`:
```
export PATH="$PATH:/path/to/arm-none-eabi-gcc"
export PATH="$PATH:/path/to/STM32CubeMX"
```

Always invoke `clion` from the command line. That way `clion` will load `.bashrc` and read the modifications you made the the `PATH` variable.

##### Windows
Find `Environment Variables` in your start menu and then add the appropriate paths to `PATH`:
```
C:\Program Files (x86)\GNU Tools Arm Embedded\<VERSION>\bin
C:\Program Files (x86)\GnuWin32\bin
C:\Program FIles (x86)\...\path\to\STM32CubeMX
...
```

These paths should include wherever the `cmake`, `make`, `arm-none-eabi-gcc`, and STM32CubeMX binaries have been installed.

Create a new enviroment variable under your user variables named `PYTHONPATH`. Set the value of the variable to path for the root directory of your fork:
```
C:\path\to\Consolidated-Firmware-Username;
```
Don't forget the semicolon, otherwise Python scripts won't work.

![image](https://github.com/UBCFormulaElectric/Consolidated-Firmware/assets/20777515/4464ddd9-5e1c-4903-b9b6-90283abb17ee)


### Python Package Dependencies
We use python both for CI (see below), and to generate C code from the `.dbc` defining messages passed over CAN. Python dependencies are managed via [pipenv](https://pipenv.readthedocs.io/en/latest/). To install all required dependencies in a [python virtual environment](https://realpython.com/python-virtual-environments-a-primer/), navigate to the root of this repository and run `pipenv install`.

### STM32CubeMX
**Note: Do not migrate project when opening .ioc file. Newer versions of FreeRTOS (>9) are incompatible with Segger SysView.**

### CLion
We edit, compile, and debug our code using [CLion](https://www.jetbrains.com/clion/). Students can obtain a CLion educational license [here](https://www.jetbrains.com/shop/eform/students). Open the `boards` directory in CLion. To compile this CLion project , you must **manually** add CMake profiles by going to: `Settings -> Build, Execution, Deployment -> CMake`. Click on the `+` to add two profiles with the following settings:

```
// First CMake profile:
Name: embedded
Build type: Debug
CMake options: -DPLATFORM=arm

// Second CMake profile:
Name: gtest
Build type: Debug
CMake options: -DPLATFORM=x86
```

In each project, there will be two configurations to use: `<board>_SeggerGDB.elf` and `OCD <board>`. Either one can be used for flashing and debugging, but the `<board>_SeggerGDB.elf` has unlimited flash breakpoints among some other extra functionalities. Use `<board>_SeggerGDB.elf` whenever possible.

##### Windows
Under **File->Settings->Build, Execution, Deployment...->Toolchains**:
Set the default toolchain to be MinGW and provide the file path to the 32-bit version of MinGW, ie:
```
C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32
```
Next, set the debugger to be ARM GDB, ie:
```
C:\Program Files (x86)\GNU Tools ARM Embedded\8 2019-q3-update\bin\arm-none-eabi-gdb.exe
```

For each project under **Run->Edit Configurations->Embedded GDB Server**, select `<board_name>_SeggerGDB`:
Then set the GDB Server to be JLinkGDBServer, ie:
```
C:\Program Files (x86)\SEGGER\JLink\JLinkGDBServer.exe
```

##### Mac OS
Under **File->Settings->Build, Execution, Deployment...->Toolchains**:
Set the default C compiler to be the GNU C and C++ compilers you installed earlier. If you installed `gcc` using `brew` your C/C++ compiler paths should be something similar to the following:
- C compiler: `/usr/local/Cellar/gcc/<some version number>/bin/gcc-11`
- C++ compiler: `/usr/local/Cellar/gcc/<some version number>/bin/g++-11`

Set the debugger to be `arm-none-eabi-gdb`, ie:

Debugger: `/path/to/arm-none-eabi-gcc/bin/arm-none-eabi-gdb`

Note: whenever you get an error saying *"`<program name>` cannot be opened because the developer cannot be verified"*, go to **System Preferences --> Security & Privacy --> General**, and click **Allow Anyway** next to the part saying *"`<program name>` was blocked from use because it is not from an identified developer"*.
