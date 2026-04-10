# Build System Test Suite

This directory contains tests for the CMake build system and CI/CD infrastructure.

## Purpose

Build system tests verify that:
- CMake configuration works correctly
- The project compiles successfully
- Test executables are generated
- Test assets are deployed properly
- Clean builds work
- CI/CD integration functions

## Running Tests

### Shell Script (Linux/Mac/Git Bash)
```bash
chmod +x tests/build_system/test_build_system.sh
./tests/build_system/test_build_system.sh
```

### Python Script (Cross-platform)
```bash
python3 tests/build_system/test_build_system.py
```

## Tests Included

1. **CMake Configuration** - Verifies CMake can configure the project
2. **File Presence** - Checks required build files exist
3. **Build Directory** - Verifies build directory structure
4. **Compilation** - Tests project compiles without errors
5. **Executable Generation** - Verifies test executable is created
6. **Asset Deployment** - Checks test assets are copied correctly
7. **Test Execution** - Runs the actual unit tests
8. **CTest Integration** - Verifies CTest discovers tests
9. **Clean Build** - Tests building from clean state
10. **Required Files** - Checks all necessary build files present

## CI/CD Integration

These tests run automatically in GitHub Actions on every push.

## Test Output

Successful run: