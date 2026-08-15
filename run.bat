@echo off
setlocal EnableDelayedExpansion

if not exist "hex_ropchain" mkdir "hex_ropchain"
if not exist "asm_ropchain" mkdir "asm_ropchain"

if not exist "radc.exe" (
    echo Executable 'radc.exe' not found. Trying to build...
    call build.bat
    if !errorlevel! neq 0 (
        echo [Error] Failed to build radc.exe! Please compile it manually.
        pause
        exit /b 1
    )
)

:loop
cls
echo ========================================
echo       radc - ROP Asm Decompiler
echo ========================================
echo Tip: Enter 'q' or 'exit' to quit.
echo.

set "filename="
set /p filename="Enter filename to decompile (e.g. test.txt): "

if "!filename!"=="" goto exit_loop
if /I "!filename!"=="q" goto exit_loop
if /I "!filename!"=="exit" goto exit_loop

:: Output filename = input name (no extension) + .asm
for %%I in ("!filename!") do set "out_filename=%%~nI.asm"

set "input_path="
if exist "hex_ropchain\!filename!" (
    set "input_path=hex_ropchain\!filename!"
) else if exist "!filename!" (
    set "input_path=!filename!"
) else (
    echo [Error] File not found: '!filename!'
    echo Please place your hex payload file into the hex_ropchain\ directory or the current directory.
    echo Press any key to try again...
    pause >nul
    goto loop
)

set "output_path=asm_ropchain\!out_filename!"
set "model_name=580vnx"

echo.
echo [*] Processing and decompiling '!input_path!'...

radc.exe "!model_name!" "!input_path!" "!output_path!"

if !errorlevel! equ 0 (
    echo.
    echo [OK] Decompilation completed successfully!
    echo - Output saved to: !output_path!
) else (
    echo.
    echo [Error] An error occurred while executing radc.exe.
)

echo.
echo Press any key to continue...
pause >nul
goto loop

:exit_loop
echo.
echo [*] Exiting program. Goodbye!
