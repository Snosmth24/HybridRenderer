# TDD Journey: Asset Loading System

## Cycle 1: Basic Creation

**Date:** 2025-01-16  
**Time:** 10:00 

### RED (Write Failing Test)
Created `AssetLoaderTest.cpp` with test for basic instantiation.
Build failed - AssetLoader.h doesn't exist.

### GREEN (Minimal Implementation)
Created empty `AssetLoader` class.
Test passes.

### REFACTOR
None needed yet.

**Status:** ✅ 1 test passing
**Coverage:** Constructor only
**Time:** 15 minutes

## Lesson Learned: Windows Path Separators

**Date:** 2025-01-16

### Issue
Tests failed with paths copied from Windows File Explorer.

### Root Cause
Windows uses backslash `\` as path separator, but in C++ strings,
backslash is an escape character.

Copying `D:\VS repos\...` creates invalid escape sequences like `\V`.

### Solution
Always use forward slashes `/` in C++ path strings:
- Works on Windows, Linux, macOS
- No escaping needed
- Cross-platform compatible

### Examples
```cpp
// ❌ WRONG (copied from File Explorer)
"D:\VS repos\HybridRenderer\test_assets\test.png"

// ✅ CORRECT (forward slashes)
"D:/VS repos/HybridRenderer/test_assets/test.png"

// ✅ ALSO CORRECT (escaped backslashes)
"D:\\VS repos\\HybridRenderer\\test_assets\\test.png"

// ✅ ALSO CORRECT (raw string literal)
R"(D:\VS repos\HybridRenderer\test_assets\test.png)"
```

### Best Practice
Use `std::filesystem::path` for cross-platform path handling.

## Cycle 4: Error Handling - Missing Files

**Date:** 2025-01-16
**Time:** [Current time]

### RED (Write Failing Test)
Created test expecting exception when loading nonexistent file.

### GREEN (Implementation)
Code already throws `std::runtime_error` when `stbi_load()` fails!
Test passed immediately.

### REFACTOR
Enhanced test to verify error message contains filename.
This ensures helpful error messages for debugging.

**Status:** ✅ 4 tests passing
**Time:** 10 minutes
**Insight:** Sometimes TDD reveals you already handled the case!