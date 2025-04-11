@echo off
setlocal enabledelayedexpansion

:: Remove old build directory for clean test
if exist build_test rmdir /s /q build_test
mkdir build_test
cd build_test

:: Configure with verbose output
echo Configuring CMake...
cmake -DCMAKE_VERBOSE_MAKEFILE=ON .. > cmake_output.log 2>&1
if %ERRORLEVEL% neq 0 (
  echo CMake configuration failed! See cmake_output.log for details.
  exit /b 1
)

:: Build just the micrasverse executable with verbose output
echo Building micrasverse executable...
cmake --build . --target micrasverse --config Release --verbose > build_output.log 2>&1
if %ERRORLEVEL% neq 0 (
  echo Build failed! See build_output.log for details.
  exit /b 1
)

echo Build completed.

:: Search for the executable in various locations
echo Searching for executable...
echo Current directory: %CD%

set FOUND=0

:: Check bin/Release directory
if exist bin\Release\micrasverse.exe (
  echo FOUND: bin\Release\micrasverse.exe
  set FOUND=1
)

:: Check bin directory
if exist bin\micrasverse.exe (
  echo FOUND: bin\micrasverse.exe
  set FOUND=1
)

:: Check Release directory
if exist Release\micrasverse.exe (
  echo FOUND: Release\micrasverse.exe
  set FOUND=1
)

:: If the executable is still not found, search for it
if !FOUND! equ 0 (
  echo Executable not found in expected locations. Searching...
  for /r %%i in (micrasverse.exe) do (
    echo FOUND: %%i
    set FOUND=1
  )
)

if !FOUND! equ 0 (
  echo ERROR: Executable not found anywhere!
) else (
  echo Build successful!
)

cd .. 
