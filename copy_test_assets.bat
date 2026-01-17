@echo off
echo ====================================
echo Copying Test Assets
echo ====================================
echo.

cd /d "%~dp0"

if not exist test_assets (
    echo ERROR: test_assets folder not found!
    pause
    exit /b 1
)

REM Copy to all possible locations
if exist build\bin\Debug (
    echo Copying to build\bin\Debug\test_assets
    xcopy /E /I /Y test_assets build\bin\Debug\test_assets
)

if exist build\bin (
    echo Copying to build\bin\test_assets
    xcopy /E /I /Y test_assets build\bin\test_assets
)

if exist build\Debug (
    echo Copying to build\Debug\test_assets
    xcopy /E /I /Y test_assets build\Debug\test_assets
)

REM Also copy to build root
echo Copying to build\test_assets
xcopy /E /I /Y test_assets build\test_assets

echo.
echo Test assets copied!
pause