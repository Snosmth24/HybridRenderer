@echo off
setlocal

cd /d "%~dp0"

echo ======================================
echo    Hybrid Renderer Build Script
echo ======================================
echo.

REM Check if build directory exists
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

REM Configure if needed
if not exist "CMakeCache.txt" (
    echo Configuring CMake...
    cmake .. -G "Visual Studio 17 2022" -A x64
    if errorlevel 1 (
        echo [ERROR] CMake configuration failed!
        pause
        exit /b 1
    )
)

REM Build
echo.
echo Building Debug configuration...
cmake --build . --config Debug
if errorlevel 1 (
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

echo.
echo ======================================
echo   Build Complete!
echo ======================================
echo.
echo Executable: build\bin\Debug\HybridRenderer.exe
echo Tests:      build\bin\Debug\HybridRenderer_tests.exe
echo.

pause