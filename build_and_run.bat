@echo off
cls
echo ====================================
echo Build and Run All Tests
echo ====================================
echo.

cd /d "%~dp0"

echo Building...
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug --target HybridRenderer_unit_tests

if errorlevel 1 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Running all tests...
echo.

REM Find and run executable
if exist build\bin\Debug\HybridRenderer_unit_tests.exe (
    build\bin\Debug\HybridRenderer_unit_tests.exe --gtest_color=yes
) else if exist build\bin\HybridRenderer_unit_tests.exe (
    build\bin\HybridRenderer_unit_tests.exe --gtest_color=yes
) else if exist build\Debug\HybridRenderer_unit_tests.exe (
    build\Debug\HybridRenderer_unit_tests.exe --gtest_color=yes
) else (
    echo ERROR: Cannot find executable!
    pause
    exit /b 1
)

pause