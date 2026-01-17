@echo off
echo ====================================
echo Running AssetLoader Tests
echo ====================================
echo.

cd /d "%~dp0"

REM Find executable
if exist build\bin\Debug\HybridRenderer_unit_tests.exe (
    set EXE_PATH=build\bin\Debug\HybridRenderer_unit_tests.exe
) else if exist build\bin\HybridRenderer_unit_tests.exe (
    set EXE_PATH=build\bin\HybridRenderer_unit_tests.exe
) else if exist build\Debug\HybridRenderer_unit_tests.exe (
    set EXE_PATH=build\Debug\HybridRenderer_unit_tests.exe
) else (
    echo ERROR: Cannot find test executable!
    echo Run build_tests.bat first!
    pause
    exit /b 1
)

echo Running: %EXE_PATH%
echo.
"%EXE_PATH%" --gtest_filter=AssetLoaderTest.* --gtest_color=yes

pause