@echo off
echo ====================================
echo Cleaning Build Directory
echo ====================================
echo.

cd /d "%~dp0"

if exist build (
    echo Deleting build directory...
    rmdir /s /q build
    echo Done!
) else (
    echo Build directory doesn't exist.
)

echo.
pause