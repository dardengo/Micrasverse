@echo off
setlocal enabledelayedexpansion

:: Default build type
set BUILD_TYPE=Debug
set CLEAN=0
set HELP=0

:: Parse command line arguments
:parse_args
if "%~1"=="" goto :args_done
if /i "%~1"=="--release" (
    set BUILD_TYPE=Release
    shift
    goto :parse_args
)
if /i "%~1"=="--debug" (
    set BUILD_TYPE=Debug
    shift
    goto :parse_args
)
if /i "%~1"=="--clean" (
    set CLEAN=1
    shift
    goto :parse_args
)
if /i "%~1"=="--help" (
    set HELP=1
    shift
    goto :parse_args
)
echo Unknown option: %~1
echo Use --help for usage information
exit /b 1
:args_done

:: Show help if requested
if %HELP%==1 (
    echo Usage: %0 [options]
    echo Options:
    echo   --release       Build in Release mode (default: Debug^)
    echo   --debug         Build in Debug mode
    echo   --clean         Clean build directory before building
    echo   --help          Show this help message
    exit /b 0
)

:: Check for required dependencies
where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Error: CMake is required but not installed or not in PATH.
    echo Please install CMake and ensure it is in your PATH.
    exit /b 1
)

where cl >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Error: Visual Studio compiler is required but not installed or not in PATH.
    echo Please install Visual Studio with C++ development tools.
    exit /b 1
)

:: Clean build directory if requested
if %CLEAN%==1 (
    echo Cleaning build directory...
    if exist build rmdir /s /q build
)

:: Create build directory
if not exist build mkdir build
cd build

:: Configure
echo Configuring CMake with build type: %BUILD_TYPE%
cmake .. -DCMAKE_BUILD_TYPE=%BUILD_TYPE%

:: Build
echo Building...
cmake --build . --config %BUILD_TYPE%

echo.
echo Build successful!

:: Check if executable exists
if exist bin\%BUILD_TYPE%\micrasverse.exe (
    echo Run with: .\bin\%BUILD_TYPE%\micrasverse.exe
) else if exist bin\micrasverse.exe (
    echo Run with: .\bin\micrasverse.exe
) else (
    echo Executable not found. Check the build output for errors.
    echo Expected locations: .\bin\micrasverse.exe or .\bin\%BUILD_TYPE%\micrasverse.exe
)
echo. 
