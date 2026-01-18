@echo off
echo ====================================
echo Available Tests
echo ====================================
echo.

cd /d "%~dp0"

set EXE_PATH=out\build\x64-Debug\bin\Debug\HybridRenderer_unit_tests.exe

if not exist "%EXE_PATH%" (
    echo ERROR: Test executable not found!
    echo Run build_tests.bat first.
    pause
    exit /b 1
)

"%EXE_PATH%" --gtest_list_tests

echo.
pause