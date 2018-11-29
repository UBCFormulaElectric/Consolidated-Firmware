# Consolidated-Firmware
A consolidated repository for gathering all firmware under one roof. 


## Continuous Integration
To pass the continuous integration, first change directory into the 
src directory and cmake the CMakeLists.txt, then call make:

`>cmake CMakeLists.txt`
`>make`

Make sure you have cmake version +3.0 installed and added to your 
environment paths

Then, if you're a Windows user, run the `check_formatting_CI.bat` 
from the Consolidated Firmware directory. Else, use the bash 
script of the same name. This will run clang-format to 
automatically format all your files.
