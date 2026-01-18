@echo off
echo ====================================
echo Quick Test (AssetLoader Only)
echo ====================================
echo.

cd /d "%~dp0"

echo Building...
cmake --build out/build/x64-Debug --config Debug --target HybridRenderer_unit_tests

if errorlevel 1 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Running AssetLoader tests...
echo.
out\build\x64-Debug\bin\Debug\HybridRenderer_unit_tests.exe --gtest_filter=AssetLoaderTest.* --gtest_color=yes

pause