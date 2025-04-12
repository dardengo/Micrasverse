#!/bin/bash

# Exit on error
set -e

# Default build type
BUILD_TYPE="Debug"
NUM_JOBS=$(nproc)

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    --release)
      BUILD_TYPE="Release"
      shift
      ;;
    --debug)
      BUILD_TYPE="Debug"
      shift
      ;;
    --clean)
      echo "Cleaning build directory..."
      rm -rf build
      shift
      ;;
    --jobs=*)
      NUM_JOBS="${1#*=}"
      shift
      ;;
    --help)
      echo "Usage: $0 [options]"
      echo "Options:"
      echo "  --release       Build in Release mode (default: Debug)"
      echo "  --debug         Build in Debug mode"
      echo "  --clean         Clean build directory before building"
      echo "  --jobs=N        Use N jobs for building (default: number of processors)"
      echo "  --help          Show this help message"
      exit 0
      ;;
    *)
      echo "Unknown option: $1"
      echo "Use --help for usage information"
      exit 1
      ;;
  esac
done

# Check for required dependencies
check_dependency() {
  if ! command -v $1 &> /dev/null; then
    echo "Error: $1 is required but not installed."
    echo "Please install it using your package manager."
    exit 1
  fi
}

check_dependency cmake
check_dependency g++

# Create build directory
mkdir -p build
cd build

# Configure
echo "Configuring CMake with build type: $BUILD_TYPE"
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE

# Build
echo "Building with $NUM_JOBS jobs..."
cmake --build . -j$NUM_JOBS

echo "Build successful!"

# Check if executable exists
if [ -f "bin/micrasverse" ]; then
  echo "Run with: ./bin/micrasverse"
elif [ -f "bin/$BUILD_TYPE/micrasverse" ]; then
  echo "Run with: ./bin/$BUILD_TYPE/micrasverse"
else
  echo "Executable not found. Check the build output for errors."
  echo "Expected locations: ./bin/micrasverse or ./bin/$BUILD_TYPE/micrasverse"
fi
