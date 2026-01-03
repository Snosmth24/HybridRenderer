@echo off
echo ======================================
echo    Running Full Test Suite (GPU)
echo ======================================
echo.

cd build

REM Build both test executables
cmake --build . --config Debug --target HybridRenderer_unit_tests
cmake --build . --config Debug --target HybridRenderer_integration_tests

if errorlevel 1 (
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

REM Run unit tests
echo.
echo ======================================
echo    Unit Tests (Headless)
echo ======================================
bin\Debug\HybridRenderer_unit_tests.exe

set unit_result=%errorlevel%

REM Run integration tests
echo.
echo ======================================
echo    Integration Tests (GPU)
echo ======================================
bin\Debug\HybridRenderer_integration_tests.exe

set integration_result=%errorlevel%

echo.
echo ======================================
echo    Test Summary
echo ======================================

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