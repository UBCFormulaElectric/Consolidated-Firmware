REM For windows the clang-format executable comes from downloading and installing the latest LLVM
REM package (under "Pre-Built Binaries" at "www.releases.llvm.org/download.html"),
REM then copying the "clang-format.exe" file from "C:\Program Files\LLVM\bin" to here
REM (then renaming it to "clang-format-VERSION_NUMBER.exe")

REM The version of the clang executable to use
set CLANG_VERSION=4.0

REM Figure out what directory this script is in
set CURR_DIR=%~dp0

REM The command we use to format a single file
set FORMAT_CMD="%CURR_DIR%clang-format-%CLANG_VERSION%.exe" -i --style=file

cd %CURR_DIR%..
for /r %%i in (*.c *.cc *.cpp *.h *.hh *.hpp) do %FORMAT_CMD% "%%i"

REM Formatting Finished - Hold the Console Window Open
pause
