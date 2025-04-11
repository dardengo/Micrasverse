#!/bin/bash

# Exit on error
set -e

# Create build directory
mkdir -p build
cd build

# Configure
cmake ..

# Build
cmake --build . -j$(nproc)

echo "Build successful!"

# Check if executable exists
if [ -f "bin/micrasverse" ]; then
  echo "Run with: ./bin/micrasverse"
else
  echo "Executable not found. Check the build output for errors."
  echo "Expected location: ./bin/micrasverse"
fi 
