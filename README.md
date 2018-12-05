# Consolidated-Firmware
A consolidated repository for gathering all firmware under one roof. 


## Continuous Integration
To pass the continuous integration, first change directory into the src directory and build CMakeLists.txt, then call make:

`>cmake CMakeLists.txt && make`

Make sure you have cmake version +3.0 installed and added to your environment paths. This will use the GCC compiler to build the project. Keil typically uses the ARM compiler but is able to work with either GCC or ARM. Keil will likely have already installed the ARM GNU Embedded Toolchain, but just in case, here's the link to download the latest version: 

https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads.

Then, run the `python check_formatting_CI.py` from inside the project directory! The script is path-dependent so make sure you are running it from the clang-format dir. Ultimately, this script allows clang-format to search your source code and automatically enforce coding conventions. 

These steps are needed to pass the TRAVIS CI test during pull request, as the CI will replicate these steps faithfully.
