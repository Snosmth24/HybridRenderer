@echo off
echo ========================================
echo   Full Test Suite (Unit + Integration)
echo ========================================
echo.

cd build

echo Building all tests...
cmake --build . --config Debug --target HybridRenderer_unit_tests
cmake --build . --config Debug --target HybridRenderer_integration_tests

if errorlevel 1 (
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo   Running Unit Tests (Fast)
echo ========================================
bin\Debug\HybridRenderer_unit_tests.exe

set unit_result=%errorlevel%

echo.
echo ========================================
echo   Running Integration Tests (Slow)
echo ========================================
bin\Debug\HybridRenderer_integration_tests.exe

set integration_result=%errorlevel%

echo.
echo ========================================
echo   Test Results
echo ========================================

if %unit_result% equ 0 (
    echo Unit Tests:        [PASSED]
) else (
    echo Unit Tests:        [FAILED]
)

if %integration_result% equ 0 (
    echo Integration Tests: [PASSED]
) else (
    echo Integration Tests: [FAILED]
)

if %unit_result% equ 0 if %integration_result% equ 0 (
    echo.
    echo [SUCCESS] All tests passed!
    pause
    exit /b 0
) else (
    echo.
    echo [ERROR] Some tests failed!
    pause
    exit /b 1
)