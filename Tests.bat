@echo off
setlocal

cd /d "%~dp0"

echo ======================================
echo    Running Tests
echo ======================================
echo.

REM Check if test executable exists
if not exist "build\bin\Debug\HybridRenderer_tests.exe" (
    echo [ERROR] Test executable not found!
    echo Please build the project first.
    pause
    exit /b 1
)

cd build

REM Run tests
echo Running tests...
echo.
bin\Debug\HybridRenderer_tests.exe

if errorlevel 1 (
    echo.
    echo [ERROR] Some tests failed!
) else (
    echo.
    echo [SUCCESS] All tests passed!
)

echo.
pause