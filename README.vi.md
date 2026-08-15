# RADC - ROP Assembly Decompiler & Recompiler (Tiếng Việt)

[![Build & Release](https://github.com/luongvantam/RADC-Decompiler/actions/workflows/build.yml/badge.svg)](https://github.com/luongvantam/RADC-Decompiler/actions/workflows/build.yml)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20macOS%20%7C%20Linux%20%7C%20WASM-blue.svg)](https://github.com/luongvantam/RADC-Decompiler)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-orange.svg)](https://en.cppreference.com/w/cpp/17)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-green.svg)](LICENSE)

**RADC (ROP Asm Decompiler)** là công cụ dịch ngược (decompiler) và tái biên dịch (recompiler) hiệu năng cao, được thiết kế chuyên dụng để phân tích và chuyển đổi các chuỗi byte hex ROP (dành cho dòng máy tính Casio ClassWiz fx-580VNX, fx-991CNX,...) thành mã nguồn assembly trực quan, dễ hiểu.

[**English (Tiếng Anh)**](README.md) | [**中文 (Chinese)**](README.zh.md)

---

## ✨ Tính Năng Nổi Bật

- 🔍 **Dịch Ngược Đa Tầng (Multi-Pass Decompilation)**: Phân tích luồng hex qua các bước prescan, dry-run và tái tạo symbol để trích xuất gadget, địa chỉ phân nhánh và vùng dữ liệu.
- 🔤 **Hỗ Trợ Bảng Ký Tự Tùy Chỉnh (`char_to_hex.json`)**: Tích hợp sẵn bộ giải mã ký tự Casio, nhận diện chính xác các ký tự tiếng Việt 2-byte có dấu (`Á`, `á`, `đ`, `ơ`, `ư`, ...) và các ký hiệu toán học đặc biệt của Casio (`√`, `÷`, `×`, `°`, `~`).
- 🔄 **Tích Hợp Re-Compiler & Xác Minh Tự Động**: Tự động biên dịch ngược lại mã asm thành byte để đối chiếu xác minh $100\%$ tính toàn vẹn với chuỗi hex ban đầu.
- 🏷️ **Tự Động Tính Toán Con Trỏ & Label**: Chuyển đổi các địa chỉ tuyệt đối thành các nhãn (`ADDR_XXXX`) và biểu thức bù trừ stack động (`eval(adr(...) + dist.main)`).
- 🌐 **Đa Nền Tảng**: Chạy tốt trên **Windows**, **macOS** (Apple Silicon & Intel), **Linux**, **Android (Termux)**, và trên trình duyệt web thông qua **WebAssembly (WASM)**.
- ⚡ **Giao Diện Menu Tương Tác**: Script `run.sh` / `run.bat` tự động quét file và xử lý nhanh chóng chỉ bằng 1 thao tác.

---

## 📥 Tải Bản Build Sẵn (Pre-built Binaries)

Bạn có thể tải các bản phát hành được biên dịch sẵn tại trang **[GitHub Releases](https://github.com/luongvantam/RADC-Decompiler/releases)**:
- `radc-windows-x86_64.zip` (Dành cho Windows 10/11 64-bit)
- `radc-macos-arm64.tar.gz` (Dành cho macOS chip Apple Silicon M1/M2/M3/M4)
- `radc-macos-x86_64.tar.gz` (Dành cho macOS chip Intel)
- `radc-linux-x86_64.tar.gz` (Dành cho Linux x86_64 Ubuntu / Debian / Fedora / Arch)

---

## 🛠️ Hướng Dẫn Tự Biên Dịch (Build from Source)

### Yêu cầu môi trường
- **CMake** phiên bản $\ge 3.10$
- **Trình biên dịch C++17** (GCC $\ge 7$, Clang $\ge 5$, hoặc MSVC $\ge 2017$)

#### Cài đặt công cụ cần thiết:
- **Ubuntu / Debian**:
  ```bash
  sudo apt update && sudo apt install -y cmake g++ build-essential
  ```
- **macOS (qua Homebrew)**:
  ```bash
  brew install cmake gcc
  ```
- **Android (qua Termux)**:
  ```bash
  pkg install cmake clang make
  ```
- **Windows**:
  Cài đặt [CMake](https://cmake.org/download/) và [Visual Studio Build Tools](https://visualstudio.microsoft.com/) hoặc [MinGW-w64](https://www.mingw-w64.org/).

---

### Cách 1: Sử Dụng CMake (Khuyên dùng)
```bash
# Clone repository
git clone https://github.com/luongvantam/RADC-Decompiler.git
cd RADC-Decompiler

# Tạo cấu hình và build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
File thực thi `radc` (hoặc `radc.exe`) sẽ nằm trong thư mục `build/`.

### Cách 2: Sử Dụng Script Build Nhanh
- **macOS / Linux / Android (Termux)**:
  ```bash
  sh build.sh
  ```
- **Windows**:
  ```cmd
  build.bat
  ```

### Cách 3: Biên Dịch Sang WebAssembly (WASM - Tuỳ chọn)
Việc biên dịch ra WebAssembly yêu cầu công cụ mở rộng [Emscripten SDK (emsdk)](https://github.com/emscripten-core/emsdk).

Bạn có thể chạy script tự động (sẽ gợi ý tải `emsdk` nếu máy chưa có):
```bash
sh build_wasm.sh
```

Hoặc cài đặt `emsdk` thủ công bằng liên kết:
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

## 🚀 Hướng Dẫn Sử Dụng

### 1. Chế Độ Menu Tương Tác
Đặt các file chứa mã hex của bạn vào thư mục `hex_ropchain/` (hoặc thư mục gốc dự án), sau đó chạy:

- **macOS / Linux / Android**:
  ```bash
  sh run.sh
  ```
- **Windows**:
  ```cmd
  run.bat
  ```

Chọn model (ví dụ `580vnx`) và nhập tên file cần decompile. Kết quả sẽ tự động lưu vào thư mục `asm_ropchain/`.

---

### 2. Sử Dụng Qua Dòng Lệnh (CLI)

```bash
./radc [--no-recompile] <thư_mục_model> <file_input_hex> <file_output_asm>
```

#### Ví dụ:
```bash
# Decompile chuỗi hex cho máy Casio fx-580VNX kèm xác minh compile
./radc 580vnx hex_ropchain/test.txt asm_ropchain/test.asm

# Decompile bỏ qua bước kiểm tra compile lại
./radc --no-recompile 580vnx hex_ropchain/input.txt asm_ropchain/input.asm
```

---

## 📝 Quy Chuẩn Cú Pháp File Input (Syntax Guide)

Bạn có thể chỉ định thông tin địa chỉ RAM ngay ở phần đầu file input hex:

```text
[src: 0xe9e0]
[dest: 0xd730]
34 7b 31 30 21 21 F6 E9 80 8f 30 30 7E 94 30 30 
30 30 33 30 00 00 41 42 43 44 00 00 00 00 00 00
```

- `[src: <địa chỉ>]`: Địa chỉ RAM gốc (nguồn).
- `[dest: <địa chỉ>]`: Địa chỉ RAM đích nơi payload được tải vào.
- Nội dung payload: Các byte hex cách nhau bởi dấu cách, tab hoặc xuống dòng (các chú thích trong ngoặc vuông `[...]` sẽ tự động được bỏ qua).

---

## 📂 Cấu Trúc Dự Án

```
RADC-Decompiler/
├── .github/workflows/
│   └── build.yml             # CI/CD tự động build cho macOS, Windows, Linux
├── 580vnx/                   # Dữ liệu cấu hình dòng máy fx-580VNX
│   ├── config.json
│   ├── gadgets.txt
│   ├── labels.txt
│   └── rom.bin               # (File ROM nhị phân nếu cần phân tích)
├── 991cnx_vf/                # Dữ liệu cấu hình dòng máy fx-991CNX
├── hex_ropchain/             # Thư mục chứa các file input hex
├── asm_ropchain/             # Thư mục lưu kết quả file assembly đã dịch ngược
├── src/                      # Mã nguồn C++
│   ├── CharTable.hpp/.cpp    # Quản lý bảng ánh xạ ký tự Casio <-> Hex
│   ├── char_to_hex.json      # Bảng chuyển đổi ký tự tiếng Việt & ký hiệu đặc biệt
│   ├── Decoder.hpp/.cpp      # Bộ giải mã lệnh
│   ├── radc.hpp/.cpp         # Động cơ decompiler cốt lõi
│   ├── compiler.hpp/.cpp     # Động cơ re-compiler
│   ├── replace.hpp/.cpp      # Bước tinh chỉnh symbol và label
│   └── main.cpp              # Điểm khởi chạy chương trình
├── web/                      # Giao diện web chạy bằng WebAssembly
├── CMakeLists.txt            # File cấu hình CMake
├── build.sh / build.bat      # Script build tự động cho máy cục bộ
├── run.sh / run.bat          # Script menu chạy tương tác
└── README.md
```

---

## ⚙️ Cấu Hình Thiết Bị / Model

Mỗi thư mục model (ví dụ `580vnx/`) bao gồm:
- `config.json`: File cấu hình liên kết đến các file gadget, label, rom.
- `gadgets.txt`: Bảng danh sách gadget và địa chỉ tương ứng.
- `labels.txt`: Bảng danh sách nhãn địa chỉ hàm và biến.
- `char_to_hex.json`: (Tùy chọn) Bảng ký tự riêng biệt nếu dòng máy dùng font khác.

---

## 🤝 Đóng Góp Phát Triển (Contributing)

Mọi đóng góp, báo cáo lỗi và Pull Request đều được hoan nghênh:
1. Fork dự án
2. Tạo nhánh tính năng (`git checkout -b feature/tinh-nang-moi`)
3. Commit thay đổi (`git commit -m 'Thêm tính năng mới'`)
4. Đẩy lên nhánh (`git push origin feature/tinh-nang-moi`)
5. Tạo Pull Request

---

## 📜 Giấy Phép (License)

Dự án được phân phối dưới giấy phép **GNU General Public License v3.0** (GPLv3). Xem chi tiết tại tệp [LICENSE](LICENSE).

*Tác giả: **luongvantam***
