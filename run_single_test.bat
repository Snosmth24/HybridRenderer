@echo off
echo ====================================
echo Run Single Test
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

if "%~1"=="" (
    echo Usage: run_single_test.bat TestName
    echo.
    echo Example: run_single_test.bat AssetLoaderTest.LoaderCanBeCreated
    echo.
    echo Available tests:
    "%EXE_PATH%" --gtest_list_tests
    pause
    exit /b 0
)

echo Running test: %~1
echo.
"%EXE_PATH%" --gtest_filter=%~1 --gtest_color=yes

pause