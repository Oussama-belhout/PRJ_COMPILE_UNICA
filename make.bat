@echo off
rem Check if the user provided an index as an argument

    echo No index provided. Running steps up to the compilation phase...
    rem --- Step 1: Generate the parser with Bison ---
    echo Generating parser with Bison...
    bison -d miniC.y
    if %errorlevel% neq 0 (
        echo Bison failed. Exiting...
        exit /b
    )

    rem --- Step 2: Generate the lexer with Flex ---
    echo Generating lexer with Flex...
    flex ANSI-C.l
    if %errorlevel% neq 0 (
        echo Flex failed. Exiting...
        exit /b
    )

    rem --- Step 3: Compile the lexer, parser, and libraries ---
    echo Compiling with GCC...
    gcc -o compiler lex.yy.c miniC.tab.c ast.c symtab.c graphgen.c -lfl -lm 
    if %errorlevel% neq 0 (
        echo Compilation failed. Exiting...
        exit /b
    )
    
    echo Compilation completed. Now you can run the tests manually.
if "%1"=="" (
    exit /b
)

rem Set the index from the argument if it was provided
set index=%1

rem --- Step 4: Create an output folder for the results ---
mkdir .\TestResults

rem --- Step 5: Get the list of test files in the Tests directory ---
echo Listing all test files...
setlocal enabledelayedexpansion
set count=0
for %%F in (.\Tests\*.c) do (
    set /a count+=1
    set file[!count!]=%%F
)

rem Check if the index is valid
if %index% leq 0 (
    echo Invalid index! Please provide a positive number.
    exit /b
)

if %index% gtr %count% (
    echo Index out of range! There are only %count% test files.
    exit /b
)

rem --- Step 6: Get the file corresponding to the provided index ---
set testFile=!file[%index%]!

rem --- Step 7: Run the test on the selected file ---
echo ====================================================
echo Testing file: !testFile!
echo ----------------------------------------------------

rem Run the test and save output to the console and the .out file
compiler.exe < !testFile! > .\TestResults\test_%index%.out 2>&1

rem Print the output directly to the console
echo Output: ***************************************
compiler.exe < !testFile!
echo ***********************************************

rem Check the compilation result
if %errorlevel% neq 0 (
    echo Test failed for !testFile!. Exiting...
    exit /b
) else (
    echo Test passed for !testFile!.
    dot -Tpdf Graph.dot -o Graph.pdf
)

echo Test completed for !testFile!.

