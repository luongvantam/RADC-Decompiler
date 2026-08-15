# RADC - ROP Assembly Decompiler & Recompiler

[![Build & Release](https://github.com/luongvantam/RADC-Decompiler/actions/workflows/build.yml/badge.svg)](https://github.com/luongvantam/RADC-Decompiler/actions/workflows/build.yml)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20macOS%20%7C%20Linux%20%7C%20WASM-blue.svg)](https://github.com/luongvantam/RADC-Decompiler)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-orange.svg)](https://en.cppreference.com/w/cpp/17)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-green.svg)](LICENSE)

**RADC (ROP Asm Decompiler)** is a high-performance disassembler, decompiler, and re-compiler designed to analyze and reconstruct Return-Oriented Programming (ROP) payload hex streams (specifically tailored for Casio ClassWiz fx-580VNX, fx-991CNX, and related architectures) into clean, human-readable assembly code.

[**Tiếng Việt (Vietnamese)**](README.vi.md) | [**中文 (Chinese)**](README.zh.md)

---

## ✨ Features

- 🔍 **Multi-Pass Decompilation**: Analyzes raw hex chains through pre-scan, dry-run, and symbol reconstruction passes to extract gadgets, addresses, and data blocks.
- 🔤 **Custom Character Table (`char_to_hex.json`)**: Built-in support for Casio character encoding tables, including full 2-byte Vietnamese accented characters (`Á`, `á`, `đ`, `ơ`, `ư`, ...) and Casio mathematical/special symbols (`√`, `÷`, `×`, `°`, `~`).
- 🔄 **Integrated Re-Compiler & Verification**: Verifies decompiled assembly by recompiling it back to bytecode and asserting $100\%$ byte-level consistency against original payloads.
- 🏷️ **Dynamic Pointer & Label Resolution**: Automatically translates absolute addresses into relocatable label references (`ADDR_XXXX`) and stack relative evaluations (`eval(adr(...) + dist.main)`).
- 🌐 **Cross-Platform**: Runs natively on **Windows**, **macOS** (Apple Silicon & Intel), **Linux**, **Android (Termux)**, and in modern web browsers via **WebAssembly (WASM)**.
- ⚡ **Interactive CLI**: Easy-to-use menu runner (`run.sh` / `run.bat`) with auto-search and batch processing.

---

## 📥 Download Pre-built Binaries

You can download pre-compiled releases directly from the **[GitHub Releases](https://github.com/luongvantam/RADC-Decompiler/releases)** page:
- `radc-macos-universal.tar.gz` (macOS Universal - Apple Silicon M1/M2/M3/M4 & Intel Mac)
- `radc-windows-x86_64.zip` (Windows 10/11 64-bit)
- `radc-linux-x86_64.tar.gz` (Linux x86_64 Ubuntu / Debian / Fedora / Arch)
- `radc-wasm.tar.gz` (WebAssembly Node.js / Web build)

---

## 🛠️ Building from Source

### Prerequisites
- **CMake** $\ge 3.10$
- **C++17 compliant compiler** (GCC $\ge 7$, Clang $\ge 5$, or MSVC $\ge 2017$)

#### Install Dependencies:
- **Ubuntu / Debian**:
  ```bash
  sudo apt update && sudo apt install -y cmake g++ build-essential
  ```
- **macOS (via Homebrew)**:
  ```bash
  brew install cmake gcc
  ```
- **Android (via Termux)**:
  ```bash
  pkg install cmake clang make
  ```
- **Windows**:
  Install [CMake](https://cmake.org/download/) and [Visual Studio Build Tools](https://visualstudio.microsoft.com/) or [MinGW-w64](https://www.mingw-w64.org/).

---

### Option 1: Build with CMake (Recommended)
```bash
# Clone the repository
git clone https://github.com/luongvantam/RADC-Decompiler.git
cd RADC-Decompiler

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
The resulting binary `radc` (or `radc.exe`) will be located in `build/`.

### Option 2: Quick Build Scripts
- **macOS / Linux / Android (Termux)**:
  ```bash
  sh build.sh
  ```
- **Windows**:
  ```cmd
  build.bat
  ```

### Option 3: WebAssembly (WASM) Build (Optional)
WebAssembly compilation requires the external [Emscripten SDK (emsdk)](https://github.com/emscripten-core/emsdk). 

You can either run the automated script (which prompts to download emsdk if missing):
```bash
sh build_wasm.sh
```

Or install `emsdk` manually:
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
cd ..
sh build_wasm.sh
```

---

## 🚀 Usage

### 1. Interactive Mode
Place your raw hex payload files inside `hex_ropchain/` (or current directory), then execute:

- **macOS / Linux / Android**:
  ```bash
  sh run.sh
  ```
- **Windows**:
  ```cmd
  run.bat
  ```

Follow the on-screen prompt to choose your model (e.g. `580vnx`) and input file. The decompiled output will be saved to `asm_ropchain/`.

---

### 2. Command Line Interface (CLI)

```bash
./radc [--no-recompile] <model_directory> <input_hex_file> <output_asm_file>
```

#### Examples:
```bash
# Decompile for Casio fx-580VNX with re-compile verification
./radc 580vnx hex_ropchain/test.txt asm_ropchain/test.asm

# Decompile without re-compile validation pass
./radc --no-recompile 580vnx hex_ropchain/input.txt asm_ropchain/input.asm
```

---

## 📝 Input Syntax Guide

You can specify payload configuration headers directly at the top of your hex input file:

```text
[src: 0xe9e0]
[dest: 0xd730]
34 7b 31 30 21 21 F6 E9 80 8f 30 30 7E 94 30 30 
30 30 33 30 00 00 41 42 43 44 00 00 00 00 00 00
```

- `[src: <address>]`: Original RAM source address.
- `[dest: <address>]`: Destination RAM address where the payload is placed.
- Hex payload body: Hex bytes separated by spaces, tabs, or newlines (brackets/comments `[...]` are ignored).

---

## 📂 Project Structure

```
RADC-Decompiler/
├── .github/workflows/
│   └── build.yml             # Multi-platform CI/CD (macOS, Windows, Linux)
├── 580vnx/                   # Casio fx-580VNX configuration
│   ├── config.json
│   ├── gadgets.txt
│   ├── labels.txt
│   └── rom.bin               # (Optional ROM inspection binary)
├── 991cnx_vf/                # Casio fx-991CNX configuration
├── hex_ropchain/             # Raw hex input payload files
├── asm_ropchain/             # Decompiled assembly output files
├── src/                      # C++ source code
│   ├── CharTable.hpp/.cpp    # Custom character & font table manager
│   ├── char_to_hex.json      # Character-to-Hex mapping table
│   ├── Decoder.hpp/.cpp      # Instruction decoder
│   ├── radc.hpp/.cpp         # Decompiler core engine
│   ├── compiler.hpp/.cpp     # ROP assembly re-compiler
│   ├── replace.hpp/.cpp      # Symbol replacement pass
│   └── main.cpp              # Entry point
├── web/                      # WebAssembly frontend UI
├── CMakeLists.txt            # CMake build definitions
├── build.sh / build.bat      # Native build scripts
├── run.sh / run.bat          # Interactive runner scripts
└── README.md
```

---

## ⚙️ Model Configuration

Each target model folder contains:
- `config.json`: Configuration referencing `gadgets`, `labels`, and optional `char_to_hex` tables.
- `gadgets.txt`: Known gadget definitions and addresses.
- `labels.txt`: Memory label and function symbol mappings.
- `char_to_hex.json`: (Optional) Model-specific character mapping file.

---

## 🤝 Contributing

Contributions, issue reports, and pull requests are welcome!
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📜 License

Distributed under the **GNU General Public License v3.0** (GPLv3). See [LICENSE](LICENSE) for more details.

*Author: **luongvantam***
