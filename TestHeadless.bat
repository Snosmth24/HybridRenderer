@echo off
echo ======================================
echo    Running Headless Tests (No GPU)
echo ======================================
echo.

cd build

REM Build unit tests
cmake --build . --config Debug --target HybridRenderer_unit_tests

if errorlevel 1 (
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

REM Run unit tests
echo.
echo Running unit tests...
bin\Debug\HybridRenderer_unit_tests.exe

if errorlevel 1 (
    echo.
    echo [ERROR] Some tests failed!
    pause
    exit /b 1
)

echo.
echo [SUCCESS] All headless tests passed!
pause