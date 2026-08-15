# RADC - ROP Asm Decompiler (中文)

**RADC (ROP Asm Decompiler)** 是一款用于将原始的 ROP 十六进制 payload 反编译并转换为易读的 ROP 汇编代码的工具。

[English](README.md) | [Vietnamese (Tiếng Việt)](README.vi.md)

## 语法指南 (Syntax Guide)

为了更好地进行反编译，您可以使用以下语法在十六进制 payload 上方指定源地址和目标地址：

```text
[src: <addr src>]
[dest: <addr dest>]
<hex chain>
```

*由 luongvantam 编写*

## 文件夹结构

- `hex_ropchain/`: 在此放置原始的十六进制 ROP 有效载荷输入文件。
- `asm_ropchain/`: 反编译输出的汇编文件将保存在此处。
- `src/`: C++ 源代码。
- `580vnx/`: 包含特定设备的配置文件（如 `config.json`、`labels.txt`、`gadgets.txt`）。

## 环境要求 (Prerequisites)

在构建或运行此工具之前，请确保已为您的操作系统安装了所需的依赖包：

- **macOS (通过 Homebrew)**: `brew install cmake gcc`
- **Ubuntu / Debian**: `sudo apt install cmake g++ build-essential`
- **Android (通过 Termux)**: `pkg install cmake clang make`
- **Windows**: 安装 [CMake](https://cmake.org/download/) 和 [MinGW-w64](https://www.mingw-w64.org/) (或 Visual Studio C++ build tools)。

如果您想构建 WebAssembly (WASM) 版本，还需要额外安装 [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html)。

## 如何构建 (Build)

### 方法 1：使用 CMake（推荐）
需要在系统上安装 `cmake`。
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### 方法 2：使用快捷构建脚本
如果您只安装了 C++ 编译器（如 `g++` 或 `clang++`）而没有 CMake，可以直接运行提供的脚本：
- **macOS / Linux / Android (Termux)**:
  ```bash
  sh build.sh
  ```
- **Windows**:
  ```cmd
  build.bat
  ```

## 如何运行

RADC 提供了一个交互式控制台界面，以实现极速反编译。如果尚未存在，脚本会自动创建 `hex_ropchain/` 和 `asm_ropchain/` 文件夹。

- **macOS / Linux / Android**:
  ```bash
  sh run.sh
  ```
- **Windows**:
  ```cmd
  run.bat
  ```

1. 输入您的文件名（例如：`test.txt`）。
2. 脚本将自动在 `hex_ropchain/` 目录或根目录中搜索该文件。
3. 反编译后的输出文件将自动保存至 `asm_ropchain/` 目录中。

### 命令行手动运行
您也可以通过命令行直接运行生成的可执行文件：
```bash
./radc <model_folder> <input_file> <output_file>
# 示例：
./radc 580vnx hex_ropchain/test.txt asm_ropchain/test.txt
```

## 配置

RADC 会读取指定设备文件夹中的 `config.json` 文件（例如 `580vnx/config.json`）来查找依赖项：
- `gadgets.txt`: 已知 ROP gadget 的名称列表。
- `labels.txt`: 已知内存地址列表（标签）。
- `rom.bin`: （可选）用于高级指令分析。

## 许可证 (License)
更多详细信息，请参阅 `LICENSE` 文件。
