REM change directory to the batch file directory
SET CURR_DIR=%~dp0

echo %CURR_DIR%

CALL %CURR_DIR%fix_formatting.bat
if %errorlevel% EQU 0 (
    goto CLANG_PASS
)

:CLANG_ERROR
    REM Formatting Failed
    echo "There was an error with formatting!!"
    exit 1

:CLANG_PASS
REM Check if anything changed

FOR /F %%F in ('git diff-index --quiet HEAD --') do SET CHANGES_EXIST=%%F

if %errorlevel% EQU 0 (
    goto GIT_PASS
)

:GIT_ERROR
    echo "There was an error in running git diff-index --quiet HEAD --"
    exit 1

:GIT_PASS
if %CHANGE_EXIST% EQ 0(
    REM No changes
    echo Formatting check PASSED, great work!
    exit 0
) 
else(
    REM Changes
    FOR /F %i IN (`git diff --color:"%%H" -n 1`) DO SET CHANGES=%i
    echo %CHANGES%
    echo 'Formatting check FAILED, see above for what changed!!'
    exit 1
)

