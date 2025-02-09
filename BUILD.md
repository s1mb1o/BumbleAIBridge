# BUILD.md

# Building BumbleAIBridge

This document provides instructions for building and testing BumbleAIBridge on various operating systems.

## Supported Platforms
- **Linux**: Ubuntu/Debian
- **macOS**
- **Windows**: Visual Studio 2017 and MinGW

## Dependencies
Before building, make sure you have the following dependencies installed:
- **Qt 6** 
  Including modules such as QtCore and QtTest.
- **CMake** 
  Version 3.10 or later.
- **Compiler**:
  - **Linux/macOS**: GCC or Clang
  - **Windows**: Visual Studio 2017 (or later) or MinGW.
- **Optional**: QtCreator for a convenient development environment

## Building with QtCreator
1. Open **QtCreator**.
2. Select **"Open Project"** and locate `CMakeLists.txt` in the project root.
3. Ensure the correct **compiler** and **Qt 6 version** are selected.
4. Click **Build Project**.

## Building with CMake (Command Line)
### Linux/macOS
```sh
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Windows (MSVC)
```sh
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

### Windows (MinGW)
```sh
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

### Additional Notes
- CMake Options:
You can customize your build with additional CMake options. For example, to specify a custom Qt6 installation path:
```bash
cmake -DQt6_DIR=/path/to/Qt6 ..
```

- Troubleshooting:
If you encounter issues, verify that all prerequisites are installed and that your environment variables (e.g., PATH, QTDIR) are correctly set.

## Run an example
```sh
cd examples/basic_example
./basic_example
```

## Running Tests
To run the unit tests, execute:

```sh
ctest --output-on-failure
```

Or, run the test executable directly:

```sh
./tests/BumbleAIBridgeTest
```

On Windows:
```sh
tests\Release\BumbleAIBridgeTest.exe
```