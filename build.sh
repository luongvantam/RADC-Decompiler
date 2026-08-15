#!/bin/sh
echo "Compiling RADC Decompiler using CMake..."

mkdir -p build
cd build || exit 1

# Detect OS to apply specific generators if needed
OS=$(uname -s 2>/dev/null || echo "Unknown")
if [ "$OS" = "Darwin" ]; then
    # Generate build system using Unix Makefiles to avoid Xcode on macOS
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
else
    # Let CMake choose the default generator (Ninja/Makefiles) for Linux/Android
    cmake -DCMAKE_BUILD_TYPE=Release ..
fi

# Build
cmake --build . --config Release

if [ $? -eq 0 ]; then
    cp radc ../radc
    echo "Build successful! Executable is 'radc' in the main folder."
else
    echo "Build failed!"
    exit 1
fi
