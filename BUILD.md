# Building Micrasverse

This document provides instructions for building the Micrasverse project on different platforms.

## Prerequisites

### Windows
- CMake (version 3.14 or higher)
- Visual Studio 2019 or later with C++ development tools
- Git (for cloning the repository and managing submodules)

### Linux
- CMake (version 3.14 or higher)
- GCC/G++ (with C++17 support)
- Git (for cloning the repository and managing submodules)
- OpenGL development libraries
- GLFW development libraries

## Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/Micrasverse.git
   cd Micrasverse
   ```

2. Initialize and update submodules:
   ```bash
   git submodule update --init --recursive
   ```

## Building the Project

### Windows

Use the provided `new_build.bat` script:

```bash
# Build in Debug mode (default)
new_build.bat

# Build in Release mode
new_build.bat --release

# Clean build directory and rebuild
new_build.bat --clean

# Show help
new_build.bat --help
```

Alternatively, you can use CMake directly:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```

### Linux

Use the provided `new_build.sh` script:

```bash
# Make the script executable (if not already)
chmod +x new_build.sh

# Build in Debug mode (default)
./new_build.sh

# Build in Release mode
./new_build.sh --release

# Clean build directory and rebuild
./new_build.sh --clean

# Specify number of jobs for parallel building
./new_build.sh --jobs=4

# Show help
./new_build.sh --help
```

Alternatively, you can use CMake directly:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . -j$(nproc)
```

## Running the Application

After a successful build, the executable will be located in:
- Windows: `build/bin/Debug/micrasverse.exe` or `build/bin/Release/micrasverse.exe`
- Linux: `build/bin/micrasverse` or `build/bin/Debug/micrasverse` or `build/bin/Release/micrasverse`

## Troubleshooting

### Common Issues

1. **CMake not found**: Ensure CMake is installed and in your PATH.
2. **Compiler not found**: Ensure you have a C++ compiler installed and in your PATH.
3. **Missing dependencies**: Install the required dependencies for your platform.
4. **Build errors**: Check the build output for specific error messages.

### Getting Help

If you encounter issues not covered in this document, please:
1. Check the project's issue tracker
2. Consult the project documentation
3. Contact the development team

## Advanced Build Options

### CMake Options

You can pass additional options to CMake when configuring the project:

```bash
# Enable shared libraries
cmake .. -DBUILD_SHARED_LIBS=ON

# Specify installation directory
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install
```

### Environment Variables

- `CMAKE_BUILD_TYPE`: Set the build type (Debug, Release, RelWithDebInfo, MinSizeRel)
- `CMAKE_CXX_FLAGS`: Add custom compiler flags 
