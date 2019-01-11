# Consolidated-Firmware
A consolidated repository for gathering all firmware under one roof. 

## Python Setup
We use python both for CI (see below), and to generate C code from the `.dbc` defining messages passed over CAN. Python dependencies are managed via [pipenv](https://pipenv.readthedocs.io/en/latest/). To install all required dependencies in a [python virtual environment](https://realpython.com/python-virtual-environments-a-primer/), navigate to the root of this repository and run `pipenv sync`.

## Continuous Integration (CI)

We run (and require) continuous integration on every pull request before it is merged. This automatically makes sure the code builds, and checks for formatting errors.

### Running Continuous Integration on Your Local Machine

You may want to run CI on your local machine (for example, to debug why your code does not pass it). Please follow the below steps to do so:

1. **Install Dependencies**: There are several dependencies required in order to mimic what CI is doing. *(Note: All links below are for windows. All equivalent linux utilities may be easily installed via the package manager for your linux distribution (`apt-get` in Ubuntu, etc.))*
  * GNU Make: http://gnuwin32.sourceforge.net/packages/make.htm
  * CMake: https://cmake.org/install/
  * Python 3+ (*Python < 3 will NOT work*): https://www.python.org/downloads/
  * ARM GNU Embedded Toolchain: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
2. **Modify your `PATH` Environment Variable**: Make sure to add the binary executables to `PATH`. To add these, find `Environment Variables` in your start menu and then add the appropriate paths to `PATH`:
```
C:\Program Files (x86)\GNU Tools Arm Embedded\<VERSION>\bin
C:\Program Files (x86)\GnuWin32\bin
...
```
(*These paths should include wherever the `cmake`, `make`, and `GCC` binaries have been installed.*)

3. **Running CI - Build Check**: Run the following commands (starting from the **root directory** of this repository) to build the code as CI does:
  * *Windows:*
  ```
  cd src
  cmake CMakeLists.txt -G "MSYS Makefiles"
  make
  ```
  * *Linux:*
  ```
  cd src
  cmake CMakeLists.txt
  make
  ```
4. **Running CI - Formatting Check**: Run the following commands (starting from the **root directory** of this project) to check formatting as CI does:
  * *Windows and Linux:*
  ```
  cd clang-format
  python check_formatting_CI.py
  ```

#### CommentPragmas
In `.clang-format`, the line `CommentPragmas: '\/\*(\*(?!\/_|[^*])*\*\/'` is ineffective because it's being preempted by `ReflowComments`. We are hoping that the next version of `clang-format` will resolve this.

## CMakeLists
CMake requires two Cube-generated files in order to compile:

1. A startup assembly file written in GCC (not ARM) syntax.
1. A linker directive. 

In Cube, after selecting the corresponding board for a module and choosing to generate the project with `Makefile` as the compile option, retrieve the `startup_stm32fxxxxx.s` and `STM32Fxxxxx_FLASH.ld` files. Append a `_gcc` to the assembly file in order to distinguish it from the Keil genered ARM syntax file and leave it inside the module's `MDK-ARM` folder. Leave the linker file inside the top module directory. 
