# Bug Report Template

Use this template for all bug reports.

---

## Bug ID: BUG-XXX

**Title:** [Concise bug description]

**Reported By:** [Your Name]  
**Date Reported:** YYYY-MM-DD  
**Severity:** Critical / High / Medium / Low  
**Priority:** P0 / P1 / P2 / P3  
**Status:** Open / In Progress / Fixed / Verified / Closed  
**Found In:** [Version/Commit]  
**Fixed In:** [Version/Commit]  
**Platform:** Windows / Linux / Both  

---

## Summary

[One-sentence description of the bug]

---

## Environment

**Operating System:** Windows 11 / Ubuntu 22.04  
**Compiler:** MSVC 19.29 / GCC 11.4  
**Build Type:** Debug / Release  
**Hardware:** [If relevant]

---

## Steps to Reproduce

1. [First step]
2. [Second step]
3. [Third step]
4. [Observe result]

---

## Expected Behavior

[What should happen]

---

## Actual Behavior

[What actually happens]

---

## Evidence

**Console Output:**
```
[Paste error messages]
```

**Screenshot:** [If applicable]

**Test Case:** TC-XXX

---

## Root Cause Analysis

**Investigation:**
[What you discovered during debugging]

**Root Cause:**
[The underlying cause of the bug]

**Code Location:**
- File: `src/path/to/file.cpp`
- Line: 123
- Function: `functionName()`

---

## Fix

**Solution:**
[Description of the fix]

**Code Changes:**
```cpp
// Before
[old code]

// After
[new code]
```

**Commit:** [git commit hash]

---

## Verification

**Test Case:** TC-XXX  
**Verification Steps:**
1. [How to verify fix]
2. [Expected result]

**Verified By:** [Your Name]  
**Verification Date:** YYYY-MM-DD  
**Status:** ✅ Verified / ❌ Not Fixed

---

## Regression Risk

**Impact:** Low / Medium / High  
**Areas Affected:** [List of components]  
**Regression Tests:** [Which tests validate this doesn't break again]

---

## Notes

[Any additional information]