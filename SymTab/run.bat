@echo off
setlocal

:: Set the compiler (make sure gcc is in your PATH)
set CC=gcc

:: Compile all files
%CC% -o inference.exe inference.c symtab.c

:: Check if compilation succeeded
if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b %errorlevel%
)

:: Run the executable
inference.exe

endlocal
