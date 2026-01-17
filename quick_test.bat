@echo off
cls
echo ====================================
echo Quick Test - AssetLoader
echo ====================================
echo.

cd /d "%~dp0"

echo [1/2] Building...
cmake --build build --config Debug --target HybridRenderer_unit_tests

if errorlevel 1 (
    echo.
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo [2/2] Running AssetLoader tests...
echo.

REM Find executable
if exist build\bin\Debug\HybridRenderer_unit_tests.exe (
    build\bin\Debug\HybridRenderer_unit_tests.exe --gtest_filter=AssetLoaderTest.* --gtest_color=yes
) else if exist build\bin\HybridRenderer_unit_tests.exe (
    build\bin\HybridRenderer_unit_tests.exe --gtest_filter=AssetLoaderTest.* --gtest_color=yes
) else if exist build\Debug\HybridRenderer_unit_tests.exe (
    build\Debug\HybridRenderer_unit_tests.exe --gtest_filter=AssetLoaderTest.* --gtest_color=yes
) else (
    echo ERROR: Cannot find test executable!
    pause
    exit /b 1
)

pause