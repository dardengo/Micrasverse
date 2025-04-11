@echo off
setlocal

:: Create build directory
if not exist build mkdir build
cd build

:: Configure
cmake ..

:: Build
cmake --build . --config Release

echo.
echo Build successful!

:: Check if executable exists directly in bin folder
if exist bin\micrasverse.exe (
  echo Run with: .\bin\micrasverse.exe
) else if exist bin\Release\micrasverse.exe (
  echo Run with: .\bin\Release\micrasverse.exe
) else (
  echo Executable not found. Check the build output for errors.
  echo Expected locations: .\bin\micrasverse.exe or .\bin\Release\micrasverse.exe
)
echo. 
