#!/bin/bash

echo "========================================"
echo "   Hybrid Renderer Build Script"
echo "========================================"
echo ""

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

# Configure if needed
if [ ! -f "CMakeCache.txt" ]; then
    echo "Configuring CMake..."
    cmake .. -DCMAKE_BUILD_TYPE=Debug
    
    if [ $? -ne 0 ]; then
        echo "[ERROR] CMake configuration failed!"
        exit 1
    fi
fi

# Build
echo ""
echo "Building Debug configuration..."
cmake --build . --config Debug

if [ $? -ne 0 ]; then
    echo "[ERROR] Build failed!"
    exit 1
fi

echo ""
echo "========================================"
echo "   Build Complete!"
echo "========================================"
echo ""
echo "Executable: build/bin/Debug/HybridRenderer"
echo "Tests:      build/bin/Debug/HybridRenderer_unit_tests"
echo ""