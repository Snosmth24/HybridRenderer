@echo off
echo ====================================
echo Finding Test Executable
echo ====================================
echo.

cd /d "%~dp0"

echo Searching in build directory...
echo.

dir /s /b build\HybridRenderer_unit_tests.exe 2>nul

if errorlevel 1 (
    echo Not found!
    echo.
    echo Run build_tests.bat to build first.
) else (
    echo.
    echo Found executable above!
)

pause