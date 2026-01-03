@echo off
setlocal

cd /d "%~dp0"

echo ======================================
echo    Hybrid Renderer Full Rebuild
echo ======================================
echo.

REM Delete build directory
if exist "build" (
    echo Deleting old build directory...
    rmdir /s /q build
)

REM Create new build directory
echo Creating new build directory...
mkdir build
cd build

REM Configure
echo Configuring CMake...
cmake .. -G "Visual Studio 17 2022" -A x64
if errorlevel 1 (
    echo [ERROR] CMake configuration failed!
    pause
    exit /b 1
)

REM Build Debug
echo.
echo Building Debug configuration...
cmake --build . --config Debug
if errorlevel 1 (
    echo [ERROR] Debug build failed!
    pause
    exit /b 1
)

REM Build Release
echo.
echo Building Release configuration...
cmake --build . --config Release
if errorlevel 1 (
    echo [ERROR] Release build failed!
    pause
    exit /b 1
)

echo.
echo ======================================
echo   Full Rebuild Complete!
echo ======================================
echo.
echo Debug Executable:   build\bin\Debug\HybridRenderer.exe
echo Release Executable: build\bin\Release\HybridRenderer.exe
echo Debug Tests:        build\bin\Debug\HybridRenderer_tests.exe
echo Release Tests:      build\bin\Release\HybridRenderer_tests.exe
echo.

pause