# Consolidated-Firmware
A consolidated repository for gathering all firmware under one roof. 

## Continuous Integration

These steps are needed to pass the TRAVIS CI test during pull request, as the CI will replicate these steps faithfully.

#### Installing Dependecies
Make sure you have cmake version 3.0+, make 3.0+, python 3, and latest version of ARM GNU Embedded Toolchain installed. You can follow these links to download and install them:

https://cmake.org/install/ 

http://gnuwin32.sourceforge.net/packages/make.htm (Windows)

http://ftp.gnu.org/gnu/make/ (Linux)

https://www.python.org/downloads/

https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

Make sure to add the binary executables to **PATH**. To add these, find **Environment Variables** in your start menu and then add the appropriate paths to **PATH**:

```
C:\Program Files (x86)\GNU Tools Arm Embedded\<VERSION>\bin
C:\Program Files (x86)\GnuWin32\bin
```

Or wherever the binaries of your cmake, make, and GCC have been installed. 

Updating the clang-format binary inside the project clang-format directory to the latest one is worthwhile, as each iteration becomes more intelligent at handling syntax. Currently, we are on the latest (clang LLVM 7.0), but new releases are out annually. Make sure to grab the binary for Ubuntu 14.04, as Travis CI is running this version of the OS. The link is available at:

http://releases.llvm.org/

#### Running Continuous Integration Checks
To pass the continuous integration, start at the root directory and call:

(Windows)
```
> cd src
> cmake CMakeLists.txt -G "MSYS Makefiles"
> make
```

(Linux)
```
> cd src
> cmake CMakeLists.txt
> make
```

This will use the GCC compiler to build the project (Keil typically uses the ARM compiler; make sure you know the difference). Address all the build errors that arise from compiling with GCC. (Generally, the proprietary ARM compiler will overlook errors that the GCC will explicitly call out).

Then, change into the `clang-format` directory and run `python check_formatting_CI.py`. 

```
> cd clang-format
> python check_formatting_CI.py
```

The script is path-dependent so make sure you are running it from the clang-format dir. Ultimately, this script allows clang-format to search your source code and automatically enforce coding conventions. 

#### CommentPragmas
In `.clang-format`, the line `CommentPragmas: '\/\*(\*(?!\/_|[^*])*\*\/'` is ineffective because it's being preempted by `ReflowComments`. We are hoping that the next version of `clang-format` will resolve this.

## CMakeLists
CMake requires two Cube-generated files in order to compile:

1. A startup assembly file written in GCC (not ARM) syntax.
1. A linker directive. 

In Cube, after selecting the corresponding board for a module and choosing to generate the project with `Makefile` as the compile option, retrieve the `startup_stm32fxxxxx.s` and `STM32Fxxxxx_FLASH.ld` files. Append a `_gcc` to the assembly file in order to distinguish it from the Keil genered ARM syntax file and leave it inside the module's `MDK-ARM` folder. Leave the linker file inside the top module directory. 
