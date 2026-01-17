@echo off
echo ====================================
echo Building HybridRenderer Unit Tests
echo ====================================
echo.

cd /d "%~dp0"

echo Current directory: %CD%
echo.

REM Create build directory if it doesn't exist
if not exist build (
    echo Creating build directory...
    mkdir build
)

echo Configuring CMake...
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug

if errorlevel 1 (
    echo.
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo Building unit tests...
cmake --build build --config Debug --target HybridRenderer_unit_tests

if errorlevel 1 (
    echo.
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ====================================
echo Build completed successfully!
echo ====================================
echo Executable location: build\bin\Debug\HybridRenderer_unit_tests.exe
pause