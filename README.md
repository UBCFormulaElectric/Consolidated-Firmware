# Consolidated-Firmware
A consolidated repository for gathering all firmware under one roof. 


CONTINUOUS INTEGRATION 
To pass the continuous integration, first change directory into the 
src directory and cmake the CMakeLists.txt, then call make:

-cmake CMakeLists.txt
-make

Make sure you have cmake version +3.0 installed and added to your 
environment paths

Then run the check_formatting_CI.bat from the Consolidated Firmware 
directory if you're a Windows user. Else, use the bash script. This 
will run clang-format to automatically format all your files.
