#!/bin/bash

# Build System Test Suite
# Tests CMake configuration, compilation, and test infrastructure

# DO NOT use set -e - we want to handle errors gracefully
# set -e would exit on first error, we want all tests to run

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
TEST_BUILD_DIR="$PROJECT_ROOT/build_test_temp"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counter
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Helper functions
pass() {
    echo -e "${GREEN}✓ PASS${NC}: $1"
    ((TESTS_PASSED++))
    ((TESTS_RUN++))
}

fail() {
    echo -e "${RED}✗ FAIL${NC}: $1"
    echo -e "  ${RED}Reason: $2${NC}"
    ((TESTS_FAILED++))
    ((TESTS_RUN++))
}

test_header() {
    echo ""
    echo -e "${YELLOW}========================================${NC}"
    echo -e "${YELLOW}TEST: $1${NC}"
    echo -e "${YELLOW}========================================${NC}"
}

cleanup() {
    if [ -d "$TEST_BUILD_DIR" ]; then
        rm -rf "$TEST_BUILD_DIR"
    fi
}

# Setup
echo "Build System Test Suite"
echo "Project Root: $PROJECT_ROOT"
echo ""

# Cleanup any previous test builds
cleanup

# =============================================================================
# TEST 1: CMakeLists.txt Exists
# =============================================================================
test_header "CMakeLists.txt File Presence"

if [ -f "$PROJECT_ROOT/CMakeLists.txt" ]; then
    pass "CMakeLists.txt exists in project root"
else
    fail "CMakeLists.txt presence" "File not found"
fi

# =============================================================================
# TEST 2: CMake Configuration
# =============================================================================
test_header "CMake Configuration (Fresh Build)"

# Run cmake and capture output
CMAKE_OUTPUT=$(cmake -B "$TEST_BUILD_DIR" -S "$PROJECT_ROOT" 2>&1)
CMAKE_EXIT_CODE=$?

if [ $CMAKE_EXIT_CODE -eq 0 ]; then
    pass "CMake configuration succeeds"
else
    fail "CMake configuration" "CMake failed with exit code $CMAKE_EXIT_CODE"
    echo "$CMAKE_OUTPUT"
fi

# =============================================================================
# TEST 3: Build Directory Structure
# =============================================================================
test_header "Build Directory Structure"

if [ -d "$TEST_BUILD_DIR" ]; then
    pass "Build directory created"
else
    fail "Build directory creation" "Directory not found at $TEST_BUILD_DIR"
fi

if [ -f "$TEST_BUILD_DIR/CMakeCache.txt" ]; then
    pass "CMakeCache.txt generated"
else
    fail "CMakeCache.txt generation" "File not found"
fi

# =============================================================================
# TEST 4: Compilation
# =============================================================================
test_header "Project Compilation"

# Only try to compile if CMake succeeded
if [ $CMAKE_EXIT_CODE -eq 0 ]; then
    BUILD_OUTPUT=$(cmake --build "$TEST_BUILD_DIR" --config Debug 2>&1)
    BUILD_EXIT_CODE=$?
    
    if [ $BUILD_EXIT_CODE -eq 0 ]; then
        pass "Project compiles successfully"
    else
        fail "Compilation" "Build failed with exit code $BUILD_EXIT_CODE"
        # Optionally show last few lines of build output
        echo "$BUILD_OUTPUT" | tail -20
    fi
else
    fail "Compilation" "Skipped - CMake configuration failed"
    BUILD_EXIT_CODE=1
fi

# =============================================================================
# TEST 5: Test Executable Exists
# =============================================================================
test_header "Test Executable Generation"

# Check multiple possible locations
TEST_EXE_FOUND=false
TEST_EXE_PATH=""
POSSIBLE_LOCATIONS=(
    "$TEST_BUILD_DIR/bin/Debug/HybridRenderer_unit_tests"
    "$TEST_BUILD_DIR/bin/Debug/HybridRenderer_unit_tests.exe"
    "$TEST_BUILD_DIR/bin/HybridRenderer_unit_tests"
    "$TEST_BUILD_DIR/bin/HybridRenderer_unit_tests.exe"
    "$TEST_BUILD_DIR/Debug/HybridRenderer_unit_tests"
    "$TEST_BUILD_DIR/Debug/HybridRenderer_unit_tests.exe"
    "$TEST_BUILD_DIR/HybridRenderer_unit_tests"
    "$TEST_BUILD_DIR/HybridRenderer_unit_tests.exe"
)

for location in "${POSSIBLE_LOCATIONS[@]}"; do
    if [ -f "$location" ]; then
        TEST_EXE_FOUND=true
        TEST_EXE_PATH="$location"
        break
    fi
done

if [ "$TEST_EXE_FOUND" = true ]; then
    pass "Test executable generated at: $TEST_EXE_PATH"
else
    fail "Test executable generation" "Executable not found. Checked locations: ${POSSIBLE_LOCATIONS[*]}"
    
    # Debug: show what actually exists in build directory
    echo "  Debug: Contents of $TEST_BUILD_DIR:"
    find "$TEST_BUILD_DIR" -name "*unit_tests*" 2>/dev/null || echo "  No unit_tests files found"
fi

# =============================================================================
# TEST 6: Test Assets Deployment (Enhanced Diagnostics)
# =============================================================================
test_header "Test Assets Deployment"

if [ "$TEST_EXE_FOUND" = true ]; then
    EXE_DIR="$(dirname "$TEST_EXE_PATH")"
    
    echo "  Executable location: $TEST_EXE_PATH"
    echo "  Looking for test_assets in: $EXE_DIR"
    
    # First, verify source test_assets exist
    if [ ! -d "$PROJECT_ROOT/test_assets" ]; then
        fail "Test assets source" "test_assets directory not found in project root: $PROJECT_ROOT/test_assets"
        echo "  You need to create test_assets/ with test images!"
    else
        echo "  Source test_assets found in project root ✓"
        
        # Check if POST_BUILD copied them
        if [ -d "$EXE_DIR/test_assets" ]; then
            if [ -f "$EXE_DIR/test_assets/test_256x256.png" ]; then
                pass "Test assets deployed correctly by CMake POST_BUILD"
            else
                fail "Test asset deployment" "test_assets directory exists but files are missing"
                echo "  Debug: Contents of $EXE_DIR/test_assets:"
                ls -lah "$EXE_DIR/test_assets/" || echo "  Cannot list directory"
            fi
        else
            # POST_BUILD didn't work, this is the actual failure
            fail "Test assets deployment" "CMake POST_BUILD did not copy test_assets to $EXE_DIR"
            
            echo "  Debug: Build output directory contents:"
            ls -lah "$EXE_DIR/" | head -15
            
            echo "  Debug: Checking if CMakeLists.txt has POST_BUILD command:"
            if grep -q "POST_BUILD" "$PROJECT_ROOT/CMakeLists.txt"; then
                echo "  POST_BUILD command found in CMakeLists.txt ✓"
            else
                echo "  WARNING: POST_BUILD command NOT found in CMakeLists.txt!"
                echo "  Add this to CMakeLists.txt after add_executable(HybridRenderer_unit_tests ...):"
                echo ""
                echo "  add_custom_command(TARGET HybridRenderer_unit_tests POST_BUILD"
                echo "      COMMAND \${CMAKE_COMMAND} -E copy_directory"
                echo "          \${CMAKE_SOURCE_DIR}/test_assets"
                echo "          \$<TARGET_FILE_DIR:HybridRenderer_unit_tests>/test_assets"
                echo "  )"
            fi
        fi
    fi
else
    fail "Test assets deployment" "Cannot check - executable not found"
fi

# =============================================================================
# TEST 7: Test Execution
# =============================================================================
test_header "Test Suite Execution"

if [ "$TEST_EXE_FOUND" = true ]; then
    # Run tests
    TEST_OUTPUT=$("$TEST_EXE_PATH" --gtest_brief=1 2>&1)
    TEST_EXIT_CODE=$?
    
    if [ $TEST_EXIT_CODE -eq 0 ]; then
        pass "All unit tests pass"
    else
        fail "Test execution" "Some unit tests failed (exit code: $TEST_EXIT_CODE)"
        echo "$TEST_OUTPUT"
    fi
else
    fail "Test execution" "Cannot run tests - executable not found"
fi

# =============================================================================
# TEST 8: CTest Integration
# =============================================================================
test_header "CTest Integration"

if command -v ctest &> /dev/null; then
    if [ $CMAKE_EXIT_CODE -eq 0 ] && [ $BUILD_EXIT_CODE -eq 0 ]; then
        # Run CTest - we're testing if CTest WORKS, not if all tests pass
        cd "$TEST_BUILD_DIR"
        CTEST_OUTPUT=$(ctest --output-on-failure 2>&1)
        CTEST_EXIT_CODE=$?
        cd "$PROJECT_ROOT"
        
        # CTest exit codes:
        # 0 = all tests passed
        # 8 = some tests failed (but CTest itself worked)
        # Others = CTest infrastructure problem
        
        if [ $CTEST_EXIT_CODE -eq 0 ]; then
            pass "CTest discovers and runs tests (all tests passed)"
        elif [ $CTEST_EXIT_CODE -eq 8 ]; then
            pass "CTest discovers and runs tests (some tests failed, but CTest works)"
            echo "  Note: CTest infrastructure is working. Test failures are in application tests."
        else
            fail "CTest execution" "CTest failed with exit code $CTEST_EXIT_CODE (infrastructure problem)"
            echo "$CTEST_OUTPUT" | tail -20
        fi
    else
        fail "CTest integration" "Skipped - build failed"
    fi
else
    echo -e "${YELLOW}⊘ SKIP${NC}: CTest not available on this system"
fi
# =============================================================================
# TEST 9: Clean Build
# =============================================================================
test_header "Clean Build Test"

# Remove build directory
cleanup

# Try to build again from scratch
CLEAN_CMAKE_OUTPUT=$(cmake -B "$TEST_BUILD_DIR" -S "$PROJECT_ROOT" 2>&1)
CLEAN_CMAKE_EXIT=$?

CLEAN_BUILD_OUTPUT=$(cmake --build "$TEST_BUILD_DIR" 2>&1)
CLEAN_BUILD_EXIT=$?

if [ $CLEAN_CMAKE_EXIT -eq 0 ] && [ $CLEAN_BUILD_EXIT -eq 0 ]; then
    pass "Clean build succeeds"
else
    fail "Clean build" "Failed to build from clean state (CMake: $CLEAN_CMAKE_EXIT, Build: $CLEAN_BUILD_EXIT)"
fi

# =============================================================================
# TEST 10: Build System Files Present
# =============================================================================
test_header "Build System Files Check"

REQUIRED_FILES=(
    "CMakeLists.txt"
    ".github/workflows/ci.yml"
)

for file in "${REQUIRED_FILES[@]}"; do
    if [ -f "$PROJECT_ROOT/$file" ]; then
        pass "Required file exists: $file"
    else
        fail "Required file check" "$file not found"
    fi
done

# =============================================================================
# TEST 11: External Dependencies
# =============================================================================
test_header "External Dependencies Check"

EXTERNAL_DIRS=(
    "external/glfw"
    "external/googletest"
)

for dir in "${EXTERNAL_DIRS[@]}"; do
    if [ -d "$PROJECT_ROOT/$dir" ]; then
        pass "External dependency exists: $dir"
    else
        fail "External dependency" "$dir not found"
    fi
done

# =============================================================================
# Summary
# =============================================================================
echo ""
echo -e "${YELLOW}========================================${NC}"
echo -e "${YELLOW}BUILD SYSTEM TEST SUMMARY${NC}"
echo -e "${YELLOW}========================================${NC}"
echo -e "Tests Run:    $TESTS_RUN"
echo -e "${GREEN}Passed:       $TESTS_PASSED${NC}"
if [ $TESTS_FAILED -gt 0 ]; then
    echo -e "${RED}Failed:       $TESTS_FAILED${NC}"
else
    echo -e "Failed:       $TESTS_FAILED"
fi

# Calculate pass rate
if [ $TESTS_RUN -gt 0 ]; then
    PASS_RATE=$((TESTS_PASSED * 100 / TESTS_RUN))
    echo -e "Pass Rate:    ${PASS_RATE}%"
fi

echo ""

# Cleanup
cleanup

# Exit with appropriate code
if [ $TESTS_FAILED -gt 0 ]; then
    echo -e "${RED}Build system tests FAILED${NC}"
    exit 1
else
    echo -e "${GREEN}All build system tests PASSED ✓${NC}"
    exit 0
fi