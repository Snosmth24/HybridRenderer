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

**Date:** 2025-1-16
**Time:** 17:39

### RED (Tests Still Failing)
AssetLoaderTest.cpp is still failing. After debugging
found that the test asset png is not loading proporly and
the test crashes before Evaluation. 

### Green (Tests Finally Pass)
After tring to update Cmakelists and VS to retarget relative
paths to test assets I eventually changed to absolute paths and VS 
finally passed the tests. Running the tests from Console works absolutly fine
with relative paths though. 

**Status:** ✅ 3 test passing
**Coverage:** Constructor only
**Time:** 55 minutes

### REFACTOR
Fix VS relative path problem. Or just run tests from console only. - Better for Headless. 