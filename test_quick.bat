@echo off
echo ========================================
echo   Quick Test (Unit Tests - No GPU)
echo ========================================
echo.

cd build

echo Building unit tests...
cmake --build . --config Debug --target HybridRenderer_unit_tests

if errorlevel 1 (
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

echo.
echo Running unit tests (headless, fast)...
echo.

bin\Debug\HybridRenderer_unit_tests.exe

if errorlevel 1 (
    echo.
    echo [ERROR] Some tests failed!
    pause
    exit /b 1
) else (
    echo.
    echo [SUCCESS] All unit tests passed!
    pause
    exit /b 0
)