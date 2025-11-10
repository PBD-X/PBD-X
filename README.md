# PBD-X

A Position-Based Dynamics (PBD) cloth simulation with OpenGL visualization.

## Installation

### Prerequisites
- CMake 3.10+
- C++17 compiler
- OpenGL 3.3+

### Using vcpkg

vcpkg is a package manager that simplifies dependency installation. Follow the steps below for your platform.

#### 1. Install vcpkg

**Windows (PowerShell):**
```powershell
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

**macOS/Linux (Bash):**
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

#### 2. Install Dependencies

**Windows (MSVC):**
```powershell
.\vcpkg install glfw3:x64-windows glew:x64-windows glm:x64-windows
```

**Windows (MinGW):**
```powershell
.\vcpkg install glfw3:x64-mingw-dynamic glew:x64-mingw-dynamic glm:x64-mingw-dynamic
```

**macOS (Clang):**
```bash
./vcpkg install glfw3:x64-osx glew:x64-osx glm:x64-osx
```

**Linux (GCC):**
```bash
./vcpkg install glfw3:x64-linux glew:x64-linux glm:x64-linux
```

#### 3. Build the Project

**Windows (MSVC):**
```powershell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=<vcpkg-path>/scripts/buildsystems/cmake/vcpkg.cmake
cmake --build . --config Debug
```

**Windows (MinGW):**
```powershell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=<vcpkg-path>/scripts/buildsystems/cmake/vcpkg.cmake -G "MinGW Makefiles"
cmake --build .
```

**macOS (Clang):**
```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=<vcpkg-path>/scripts/buildsystems/cmake/vcpkg.cmake
cmake --build .
```

**Linux (GCC):**
```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=<vcpkg-path>/scripts/buildsystems/cmake/vcpkg.cmake
cmake --build .
```

Replace `<vcpkg-path>` with the path to your vcpkg installation.

#### 4. Run

**Windows:**
```powershell
.\Debug\PBD-X-Kevin.exe
```

**macOS/Linux:**
```bash
./PBD-X-Kevin
```

## Usage

### Controls

| Action | Key/Input |
|--------|-----------|
| Pan camera | Left mouse drag |
| Zoom camera | Mouse wheel scroll |
| Toggle gravity | G |
| Toggle wind | W |
| Increase simulation speed | ] |
| Decrease simulation speed | [ |

### Simulation Speed

- Default speed: 0.5x (half real-time)
- Press `[` to slow down, `]` to speed up
- Current speed is printed to console when adjusted

## Project Structure

- `src/core/` — Physics engine (Position-Based Dynamics)
- `src/gui/` — OpenGL renderer and application
- `CMakeLists.txt` — Build configuration
