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
