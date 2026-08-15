#!/bin/bash
set -e

echo "=================================================="
echo "   RADC Decompiler - WebAssembly (WASM) Build     "
echo "=================================================="

# 1. Try to load emsdk environment if present
if [ -f "./emsdk/emsdk_env.sh" ]; then
    source ./emsdk/emsdk_env.sh > /dev/null 2>&1
elif [ -n "$EMSDK" ] && [ -f "$EMSDK/emsdk_env.sh" ]; then
    source "$EMSDK/emsdk_env.sh" > /dev/null 2>&1
fi

# 2. Check if emcc is available
if ! command -v emcc > /dev/null 2>&1; then
    printf "\033[0;33m[Notice] Emscripten (emcc) was not found in your environment.\033[0m\n\n"
    echo "The Emscripten SDK (emsdk) is an external dependency used to build the WebAssembly version."
    echo "Official repository: https://github.com/emscripten-core/emsdk"
    echo ""
    echo "Manual setup steps:"
    echo "  git clone https://github.com/emscripten-core/emsdk.git"
    echo "  cd emsdk"
    echo "  ./emsdk install latest"
    echo "  ./emsdk activate latest"
    echo "  source ./emsdk_env.sh"
    echo "  cd .."
    echo "  ./build_wasm.sh"
    echo ""
    read -p "Do you want to automatically clone and setup emsdk now? [y/N]: " choice
    if [ "$choice" = "y" ] || [ "$choice" = "Y" ]; then
        echo "Cloning emsdk from https://github.com/emscripten-core/emsdk.git..."
        git clone https://github.com/emscripten-core/emsdk.git emsdk
        cd emsdk
        ./emsdk install latest
        ./emsdk activate latest
        source ./emsdk_env.sh
        cd ..
    else
        echo "Build cancelled. Please setup emsdk and run this script again."
        exit 1
    fi
fi

mkdir -p build_wasm

echo "Compiling WebAssembly module..."
em++ -O3 -std=c++17 src/*.cpp -o build_wasm/radc.js -s NODERAWFS=1

if [ $? -eq 0 ]; then
    printf "\033[0;32m[Success] WebAssembly build completed! Output in 'build_wasm/'\033[0m\n"
    echo "To test with Node.js:"
    echo "  node build_wasm/radc.js <model> <input_file> <output_file>"
else
    printf "\033[0;31m[Error] Build failed!\033[0m\n"
    exit 1
fi
