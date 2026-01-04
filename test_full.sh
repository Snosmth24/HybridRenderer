#!/bin/bash

echo "========================================"
echo "   Full Test Suite (Unit + Integration)"
echo "========================================"
echo ""

cd build

echo "Building all tests..."
cmake --build . --config Debug --target HybridRenderer_unit_tests
cmake --build . --config Debug --target HybridRenderer_integration_tests

if [ $? -ne 0 ]; then
    echo "[ERROR] Build failed!"
    exit 1
fi

echo ""
echo "========================================"
echo "   Running Unit Tests (Fast)"
echo "========================================"
./bin/Debug/HybridRenderer_unit_tests

unit_result=$?

echo ""
echo "========================================"
echo "   Running Integration Tests (Slow)"
echo "========================================"
./bin/Debug/HybridRenderer_integration_tests

integration_result=$?

echo ""
echo "========================================"
echo "   Test Results"
echo "========================================"

if [ $unit_result -eq 0 ]; then
    echo "Unit Tests:        [PASSED]"
else
    echo "Unit Tests:        [FAILED]"
fi

if [ $integration_result -eq 0 ]; then
    echo "Integration Tests: [PASSED]"
else
    echo "Integration Tests: [FAILED]"
fi

if [ $unit_result -eq 0 ] && [ $integration_result -eq 0 ]; then
    echo ""
    echo "[SUCCESS] All tests passed!"
    exit 0
else
    echo ""
    echo "[ERROR] Some tests failed!"
    exit 1
fi