#!/bin/bash

echo "========================================"
echo "   Quick Test (Unit Tests - No GPU)"
echo "========================================"
echo ""

cd build

echo "Building unit tests..."
cmake --build . --config Debug --target HybridRenderer_unit_tests

if [ $? -ne 0 ]; then
    echo "[ERROR] Build failed!"
    exit 1
fi

echo ""
echo "Running unit tests (headless, fast)..."
echo ""

./bin/Debug/HybridRenderer_unit_tests

if [ $? -ne 0 ]; then
    echo ""
    echo "[ERROR] Some tests failed!"
    exit 1
else
    echo ""
    echo "[SUCCESS] All unit tests passed!"
    exit 0
fi