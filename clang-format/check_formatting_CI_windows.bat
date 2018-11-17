REM change directory to the batch file directory
SET CURR_DIR=%cd% %~dp0

REM Format all the files
if errorlevel 1 goto CLANG_ERROR

:CLANG_ERROR
    REM Formatting Failed
    echo "There was en error with formatting!!"
    exit 1

REM Check if anything changed
call git diff-index --quiet HEAD -- 

if errorlevel 1 goto FORMATTING_ERROR
    REM No changes
    printf 'Formatting check PASSED, great work!'
    exit 0

:FORMATTING_ERROR
    REM Changes
    FOR /F %i IN (`git diff --color:"%%H" -n 1`) DO SET CHANGES=%i
    echo "$CHANGES"
    echo 'Formatting check FAILED, see above for what changed!!'
    exit 1

