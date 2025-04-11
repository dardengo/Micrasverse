@echo off
setlocal enabledelayedexpansion

echo ===== MICRASVERSE BUILD DIAGNOSTIC TOOL =====
echo.

:: Check if CMake is installed
where cmake >nul 2>&1
if %ERRORLEVEL% neq 0 (
  echo ERROR: CMake not found in PATH. Please install CMake and try again.
  exit /b 1
)
echo [OK] CMake found

:: Print CMake version
echo CMake version:
cmake --version
echo.

:: Clean build directory for fresh test
echo Cleaning build directory...
if exist build_diag rmdir /s /q build_diag
mkdir build_diag
cd build_diag

:: Detect available generators
echo Available CMake generators:
cmake --help | findstr "* =" | findstr /v "Variables Properties"
echo.

:: Configure with detailed output
echo Configuring CMake...
cmake -DCMAKE_VERBOSE_MAKEFILE=ON .. > cmake_output.log 2>&1
if %ERRORLEVEL% neq 0 (
  echo ERROR: CMake configuration failed!
  type cmake_output.log
  exit /b 1
)
echo [OK] CMake configuration successful

:: Show what generator was used
echo Used CMake generator:
cmake . -LA | findstr CMAKE_GENERATOR
echo.

:: Build the project
echo Building project...
cmake --build . --verbose > build_output.log 2>&1
if %ERRORLEVEL% neq 0 (
  echo ERROR: Build failed!
  type build_output.log
  exit /b 1
)
echo [OK] Build completed

:: Print all targets
echo Available targets:
cmake --build . --target help
echo.

:: Search for the executable
echo Searching for micrasverse executable...
set FOUND=0

echo Checking specific directories:
for %%p in (
  "bin\micrasverse.exe"
  "bin\Release\micrasverse.exe"
  "bin\Debug\micrasverse.exe" 
  "Release\micrasverse.exe"
  "Debug\micrasverse.exe"
) do (
  if exist %%p (
    echo FOUND: %%p
    set FOUND=1
  ) else (
    echo NOT FOUND: %%p
  )
)

if !FOUND! equ 0 (
  echo Performing deep search for executable...
  for /r %%i in (micrasverse.exe) do (
    echo FOUND: %%i
    set FOUND=1
  )
)

:: Check status
if !FOUND! equ 0 (
  echo ERROR: Executable not found anywhere!
  echo Checking CMake cache for output paths:
  cmake -LA | findstr -i output_directory
  echo.
  echo Listing bin directory contents:
  if exist bin (
    dir /s /b bin
  ) else (
    echo bin directory not found
  )
) else (
  echo [OK] Executable found
)

echo.
echo ===== DIAGNOSTIC COMPLETE =====
echo.
echo If you're still having issues, please check:
echo 1. src/CMakeLists.txt to ensure the micrasverse target is correctly defined
echo 2. The build_output.log for any compile/link errors
echo 3. The cmake_output.log for any configuration issues

cd .. 
