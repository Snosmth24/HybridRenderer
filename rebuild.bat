@echo off
echo ====================================
echo Full Rebuild
echo ====================================
echo.

cd /d "%~dp0"

echo Step 1: Cleaning...
if exist build (
    rmdir /s /q build
    echo Build directory deleted.
)

echo.
echo Step 2: Configuring...
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug

if errorlevel 1 (
    echo Configuration failed!
    pause
    exit /b 1
)

echo.
echo Step 3: Building...
cmake --build build --config Debug --target HybridRenderer_unit_tests

if errorlevel 1 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo ====================================
echo Rebuild complete!
echo ====================================
pause