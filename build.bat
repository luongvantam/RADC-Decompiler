@echo off
echo Compiling RADC Decompiler using CMake...

if not exist build mkdir build
cd build || exit /b 1

cmake ..
cmake --build . --config Release

if %errorlevel% equ 0 (
    copy Release\radc.exe ..\radc.exe 2>nul || copy radc.exe ..\radc.exe 2>nul
    echo Build successful! Executable is 'radc.exe' in the main folder.
) else (
    echo Build failed!
    exit /b %errorlevel%
)
