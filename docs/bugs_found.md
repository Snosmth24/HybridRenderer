# Bugs Found During Development

This document tracks all bugs discovered and fixed during development - demonstrating QA mindset and debugging skills.

---

## Bug ID: BUG-001

## Bug #1: Crash on Window Close

**Reported By:** [Tigran Amiragov]  
**Date Reported:** 2026-01-16  
**Severity:** Critical 
**Priority:** P0   
**Status:**  Fixed 
  
**Platform:** Windows / Linux  

---

## Summary

Application crashed with access violation when closing the window.

---

## Environment

**Operating System:** Windows 11 / Ubuntu 22.04  
**Compiler:** MSVC 19.29 / GCC 11.4  
**Build Type:** Debug
---

## Steps to Reproduce

1. Run program in debug mode
2. Close window after all initialization is complete
3. Program should crash with exception thrown.


---

## Expected Behavior

Program exits smoothly when window is closed manually or program is told to close.

---

## Actual Behavior

Program crashes when window is closed.

---

## Evidence

**Console Output:**
```
Exception thrown: read access violation.
this->device was 0xFFFFFFFFFFFFFFFF.
```

**Screenshot:**  N/A

**Test Case:** Planned

---

## Root Cause Analysis

**Investigation:**
When the window is closed the program throws a read access violation error. This is due to a synchronization bug
that allows for the application to continue performing cleanup operations after frame destruction. 

**Root Cause:**
GPU was still processing frames during cleanup. Called `cleanup()` before waiting for device idle.

**Code Location:**


---

## Fix

**Solution:**
Use the provided vkDeviceWaitIdle(device) function at the beggining of cleanup to sybchronize device. 

**Code Changes:**
```cpp
void VulkanContext::cleanup() {
        // ... rest of cleanup
}
```
// After
```cpp
void VulkanContext::cleanup() {
    vkDeviceWaitIdle(device);  // ← Added this
    // ... rest of cleanup
}
```
**Commit:** [b15dbbe]

---

## Verification

**Test Case:** N/A  
**Verification Steps:**
1. Run Updated CI workflow
2. Program Exits smoothly w/o exception

**Verified By:** [Tigran Amiragov]  
**Verification Date:** 2026-01-16 
**Status:** ✅ Verified 

---

## Regression Risk

**Impact:**  High  
**Areas Affected:** [All]  
**Regression Tests:** Pending

---

## Notes

Synchonization is a persistant refression risk throught all development. Planning dedicated test suite for common 
synchronization bugs. 

---

## Bug ID: BUG-002

**Title:** White Screen on First Frame

**Reported By:** [Tigran Amiragov]  
**Date Reported:** 2026-01-16  
**Severity:**  High  
**Priority:**  P1  
**Status:**  Fixed   
**Found In:** [b15dbbe]  
**Fixed In:** [b15dbbe]  
**Platform:**  Both  

---

## Summary

First frame displayed white screen instead of clear color.

---

## Environment

**Operating System:** Windows 11 / Ubuntu 22.04  
**Compiler:** MSVC 19.29 / GCC 11.4  
**Build Type:** Debug / Release  


---

## Steps to Reproduce

1. Run program in debug mode
2. Wait for window to open for the first time
3. Displayed window color should be white


---

## Expected Behavior

When window opens for the first time it should be a solid black.
After shader integration (added after bug was discovered) screen should have a 
multi-colored triangle with black background.

---

## Actual Behavior

Window is initially a solid white color until it is resized.

---

## Evidence

**Console Output:**
```
No error message
```



**Test Case:** Pending

---

## Root Cause Analysis

**Investigation:**
The window was a solid white color at start up but changed to black After
resizing the dimensions manually. This implied that when the area was redrawn 
the correct color was applied. This suggested only the initial frame was bugged

**Root Cause:**
Image was acquired twice per frame (in both `beginFrame` and `endFrame`),
 causing index mismatch.

**Code Location:**
- File: `src/vulkan/BulkanContext.cpp
- Line: 688
- Function: `VulkanContext::beginFrame()`

---

## Fix

**Solution:**
Store a 'currentImageIndex' variable the synchronize beginframe and endframe.
This prevents multiple aquasitions on the same frame.

**Code Changes:**
```cpp
// Store image index as member variable
uint32_t currentImageIndex;

void beginFrame() {
    vkAcquireNextImageKHR(..., &currentImageIndex);  // ← Store here
}

void endFrame() {
    // Use stored index, don't acquire again
}
```

**Commit:** [b15dbbe]

---

## Verification

**Test Case:** Pending  
**Verification Steps:**
1. Run program in debug mode
2. Initial frame of window should be of a colored triangle with solid black
background.

**Verified By:** [Tigran Amiragov]  
**Verification Date:** 2026-01-16  
**Status:** ✅ Verified 

---

## Regression Risk

**Impact:** Medium 
**Areas Affected:** VulkanContext  
**Regression Tests:** All Vulkan integration tests

---

## Notes

Vulkan has by design very little verification enabled by default. This causes a 
wide range of possible regression scenarios that can result from different platforms or 
devices. Usually there will be no error message for this perticular bug. Recommend dedicated 
regression test suite for physical device platforms.
---

## Bug ID: BUG-003

**Title:** Uninitialized Swapchain Extent

**Reported By:** [Tigran Amiragov]  
**Date Reported:** 2026-01-16 
**Severity:**  High 
**Priority:**  P1   
**Status:**  Fixed  
**Found In:** [b15dbbe]  
**Fixed In:** [b15dbbe]  
**Platform:**  Both  

---

## Summary

Random crashes during swapchain operations.

---

## Environment

**Operating System:** Windows 11 / Ubuntu 22.04  
**Compiler:** MSVC 19.29 / GCC 11.4  
**Build Type:** Debug / Release  


---

## Steps to Reproduce

1. Run Program in Debug mode multiple times


---

## Expected Behavior

Program runs and exits smoothly

---

## Actual Behavior

Program Crashes randomly.

---

## Evidence

**Console Output:**
```
Access violation reading location 0x0000000000000000
```



**Test Case:** Pending

---

## Root Cause Analysis

**Investigation:**
[What you discovered during debugging]

**Root Cause:**
[The underlying cause of the bug]

**Code Location:**
- File: `src/VulkanContext.cpp`
- Line: 91
- Function: `VulkanContext::VulkanContext(IWindow* window)`

---

## Fix

**Solution:**
Initialize swapChainExtant explicitly in VulkanContext constructor

**Code Changes:**
```cpp
// Before
[VulkanContext::VulkanContext(IWindow* window)
        :swapChainImageFormat(VK_FORMAT_UNDEFINED)
{
}

// After
VulkanContext::VulkanContext(IWindow* window)
    : swapChainExtent{0, 0}  // ← Added initialization
    , swapChainImageFormat(VK_FORMAT_UNDEFINED)
{
}
```

**Commit:** [b15dbbe]

---

## Verification

**Test Case:** Pending  
**Verification Steps:**
1. Run program in debug mode multiple times
2. Program Should run and exit smoothly with no access reading violations.

**Verified By:** [Tigran Amiragov]  
**Verification Date:** 2026-01-16  
**Status:** ✅ Verified 

---

## Regression Risk

**Impact:** Low   
**Areas Affected:** VulkanContext 
**Regression Tests:** All CI tests run program in requiered w/ requiered conditions.

---

## Notes

Always initialize member variables. 
---

## Bug ID: BUG-004

**Title:** Double Cleanup Crash

**Reported By:** [Tigran Amiragov]  
**Date Reported:** 2026-01-16  
**Severity:**  Medium 
**Priority:**  P1   
**Status:**  Fixed  
**Found In:** [b15dbbe]  
**Fixed In:** [b15dbbe]  
**Platform:**  Both  

---

## Summary

The cleanup() function was being called multiple times. Causing a crash do to Null pointer
dereferencing.

---

## Environment

**Operating System:** Windows 11 / Ubuntu 22.04  
**Compiler:** MSVC 19.29 / GCC 11.4  
**Build Type:** Debug / Release  


---

## Steps to Reproduce

1. Run the program in debug mode
2. Exit the program or let it terminate
3. Crash


---

## Expected Behavior

Program should exit smoothly

---

## Actual Behavior

Program crashes instead of closing smoothly

---

## Evidence

**Console Output:**
```
N/A
```


**Test Case:** Pending

---

## Root Cause Analysis

**Investigation:**
[What you discovered during debugging]

**Root Cause:**
No guard against double cleanup - tried to destroy already-destroyed Vulkan objects.

**Code Location:**
- File: `src/vulkan/VulkanContext.cpp
- Line: 869
- Function: `cleanUp()`

---

## Fix

**Solution:**
Added check to top of cleanUp() function to exit if device hand is already null.

**Code Changes:**
```cpp
// Before
void cleanup() {
	// ... cleanup code
    device = VK_NULL_HANDLE;
	}

// After
void cleanup() {
    if (device == VK_NULL_HANDLE) {
        return;  // ← Added guard
    }
    // ... cleanup code
    device = VK_NULL_HANDLE;
```

**Commit:** [git commit hash]

---

## Verification

**Test Case:** Pending
**Verification Steps:**
1. Run program in debug mode
2. Exit program
3. Program Should exit smoothly without crashing.

**Verified By:** [Tigran Amiragov]  
**Verification Date:** 2026-01-16 
**Status:** ✅ Verified 

---

## Regression Risk

**Impact:** Medium  
**Areas Affected:** VulkanContext  
**Regression Tests:** All regression tests

---

## Notes

This is a common synchronization issue and is easily checked implicitly by all integration tests.
---

## Bug #5: CI Timeout on Submodule Fetch
**Reported By:** [Tigran Amiragov]  
**Date:** 2026-01-16  
**Priority:** P1  
**Status:** ✅ Fixed & Verified  
**Platform:** All

### Summary
GitHub Actions timed out fetching large submodules. GLFW + GoogleTest submodules (~100MB) too large for default timeout.

### Environment
- OS: Windows 11, Ubuntu 22.04
- Compiler: MSVC, GCC
- Build: Debug

### Steps to Reproduce
1. Push Local repo/branch to CI server
2. Cmake fails to build due to server timeout 
3. No tests run

### Expected Behavior
CI server builds project headless and runs tests smoothly.

### Actual Behavior
CI server fails to build project and no tests run

### Root Cause
Using Git Submodules for GLFW and Vulkan dependencies cause CI server to timeout when trying to download packages (~100MB) - too large.

### Fix
Include GLFW and Vulkan libraries explicitly in local repo and github.

### Verification
- Test Case: All CI workflows
- Verification: Subsequent pushes to Github successfully build on CI server. 
- Status: ✅ Verified

### Prevention
- Include large Dependency libraries explicitly in repo - no submodules. 

### Related Test Cases
- All tests

---

## Bug ID: BUG-006

**Title:** Missing Namespace in Tests (Linux)

**Reported By:** [Tigran Amiragov]  
**Date Reported:** 2026-01-156 
**Severity:** Critical  
**Priority:** P0  
**Status:**  Fixed   
**Found In:** [b15dbbe]  
**Fixed In:** [b15dbbe]  
**Platform:**  Linux 

---

## Summary

Tests compiled on Windows but failed on Linux with "identifier not found" error.

---

## Environment

**Operating System:** Windows 11 / Ubuntu 22.04  
**Compiler:** MSVC 19.29 / GCC 11.4  
**Build Type:** Debug  


---

## Steps to Reproduce

1. Push local repo to github
2. CI workflow automatically initiates
3. linux build fails 

---

## Expected Behavior

CI workflow successfully builds on linux version

---

## Actual Behavior

Linux build fails due to lack of explicit namespace usage in source code.

---

## Evidence

**Console Output:**
```
Run ./build/bin/Debug/HybridRenderer_unit_tests
/home/runner/work/_temp/7901f545-bec7-45da-9798-9654b5dedbcf.sh: line 1: ./build/bin/Debug/HybridRenderer_unit_tests: No such file or directory
Error: Process completed with exit code 127.
```

**Test Case:** TC-009

---

## Root Cause Analysis

**Investigation:**
Linux is much more strict with namespace usage and won't auto complete function names.

**Root Cause:**
Used `now()` instead of `std::chrono::high_resolution_clock::now()`. Windows compiler was more permissive.

**Code Location:**
- File: `/tests/unit/RendererUnitTest.cpp`
- Line: 279
- Function: `TEST_F(RendererUnitTest, RenderingIsFast)`

---

## Fix

**Solution:**
Fully qualified functions using `std::chrono::high_resolution_clock::now()`

**Code Changes:**

```cpp
auto start = std::chrono::high_resolution_clock::now();  // ← Fully qualified
```


**Commit:** [b15dbbe]

---

## Verification

**Test Case:** Pending 
**Verification Steps:**
1. Run github CI workflow
2. All platform versions build and run

**Verified By:** [Tigran Amiragov]  
**Verification Date:** 2016-01-16  
**Status:** ✅ Verified 

---

## Regression Risk

**Impact:**  High  
**Areas Affected:** All on linux platform
**Regression Tests:** All Tests

---

## Notes

The regression risk is high due to that as more code is added the usage on namespaces will increase. As such 
all tests potentially have a regression risk if full name space qualification is forgotten. 
---

## Bug ID: BUG-007

**Title:** Uninitialized Pointer in Test Fixture

**Reported By:** [Tigran Amiragov]  
**Date Reported:** 2026-01-16  
**Severity:**  High 
**Priority:** P1 
**Status:** Fixed  
**Found In:** [b15dbbe]  
**Fixed In:** [b15dbbe]  
**Platform:**  Both  

---

## Summary

Test fixture pointer was uninitialized, causing intermittent crashes during test cleanup.

---

## Environment

**Operating System:** Windows 11 / Ubuntu 22.04  
**Compiler:** MSVC 19.29 / GCC 11.4  
**Build Type:** Debug / Release  

---

### Steps to Reproduce
1. Run AssetLoaderTest suite
2. Tests pass but crash during teardown
3. Crash is non-deterministic

---

## Expected Behavior

Tests should clean up gracefully without crashes.

---

## Actual Behavior

Intermittent crashes with no clear error message.

---

## Evidence

**Console Output:**
```
The following tests FAILED:
	 51 - AssetLoaderTest.HasLoadTextureMethod (Failed)
	 52 - AssetLoaderTest.LoadsCorrectDimensions (Failed)
	 54 - AssetLoaderTest.CachesLoadedTextures (Failed)
	 55 - AssetLoaderTest.CachesDifferentTextures (Failed)
	 56 - AssetLoaderTest.ClearCacheRemovesAllTextures (Failed)
	 57 - AssetLoaderTest.CacheReducesLoadTime (Failed)
	 58 - AssetLoaderTest.MultipleLoadsShowCacheBenefit (Failed)
	 59 - AssetLoaderTest.CacheSizeRemainsConstant (Failed)
	 60 - AssetLoaderTest.DifferentPathsCreateSeparateCacheEntries (Failed)
Errors while running CTest
Error: Process completed with exit code 1.
```


**Test Case:** TC-013

---

## Root Cause Analysis

**Investigation:**
```cpp
class AssetLoaderTest : public ::testing::Test {
protected:
    AssetLoader* loader;  // ← UNINITIALIZED
    
    void TearDown() override {
        delete loader;  // ← Deleting garbage pointer!
    }
};
```

**Root Cause:**
C++ doesn't initialize pointers automatically. Uninitialized pointer contains garbage value. Attempting to delete garbage causes undefined behavior.

**Code Location:**
- File: `src/tests/unit/asset/AssetLoaderTest.cpp
- Line: 16
- Function: `TearDown()`

---

## Fix

**Solution:**
```cpp
AssetLoader* loader = nullptr;  // ← Initialize to nullptr

void TearDown() override {
    if (loader) {  // ← Safety check
        delete loader;
        loader = nullptr;
    }
}
```

**Commit:** [b15dbbe]

---

## Verification

**Test Case:** TC-013  
**Verification Steps:**
1. Run github CI workflow
2. All platform versions build and run

**Verified By:** [Tigran Amiragov]  
**Verification Date:** 2026-01-16  
**Status:** ✅ Verified 

---

## Regression Risk

**Impact:**  High  
**Areas Affected:** All on linux platform
**Regression Tests:** All Tests

---

## Bug ID: BUG-008

**Title:** Path Separator Escape Sequence

**Reported By:** [Tigran Amiragov]  
**Date Reported:** 2026-01-16  
**Severity:**  High 
**Priority:**  P0
**Status:**  Fixed   
**Found In:** [b15dbbe]  
**Fixed In:** [b15dbbe]  
**Platform:**  Both  

---

## Summary

Path Separator Escape Sequence

---

## Environment

**Operating System:** Windows 11 / Ubuntu 22.04  
**Compiler:** MSVC 19.29 / GCC 11.4  
**Build Type:** Debug / Release  


---

## Steps to Reproduce

1. Run Assetloading test suite
2. test fails with can't fopen error

---

## Expected Behavior

Test should pass

---

## Actual Behavior

Test fails due to file path bug

---

## Evidence

**Console Output:**
```
N/A
```



**Test Case:** TC-001

---

## Root Cause Analysis

**Investigation:**
While running the TDD workflow tests continued to fail even after implementation. 

**Root Cause:**
Copying paths from Windows File Explorer includes backslashes:

**Code Location:**
- File: `src/tests/unit/asset/AssetLoaderTest.cpp`
- Line: 32
- Function: `TEST_F(AssetLoaderTest, HasLoadTextureMethod)`

---

## Fix

**Solution:**
- Always use forward slashes in C++ path strings
- Windows accepts `/` as path separator
- Forward slashes are cross-platform compatible
- Or use `std::filesystem::path` for automatic handling

**Code Changes:**
```cpp
// Before
std::string path = "test_assets\test.png";

// After
// Use forward slashes (works on Windows + Linux)
std::string path = "test_assets/test.png";
```

**Commit:** [b15dbbe]

---

## Verification

**Test Case:** TC-001
**Verification Steps:**
1. Run TDD asset loader test suite
2. tests pass

**Verified By:** [Tigran Amiragov]  
**Verification Date:** 2026-01-16  
**Status:** ✅ Verified 

---

## Regression Risk

**Impact:** Low   
**Areas Affected:** All  
**Regression Tests:** None

---

## Notes

This is a common and general syntax error not related to business logic. Erros can occur when copy-pasting file paths 
from windows file explorer since the '/' are reversed. 
Backslash is an escape character in C++ strings. Use `/` for paths or escape backslashes (`\\`).
---

## Bug #9: Platform-Specific Memory Allocator Behavior

**Date:** 2026-01-17  
**Severity:** Medium (Test flakiness)  
**Component:** Test Suite  
**Platform:** Linux-specific

### Symptoms
```
✅ Windows: Tests pass
❌ Linux: Same tests fail
```

Tests that compared pointers failed on Linux but passed on Windows.

### Root Cause
Test assumed memory allocator wouldn't reuse addresses:
```cpp
Texture* old = load("test.png");
delete old;  // Free memory at address 0x1000

Texture* new = load("test.png");
// Linux allocator reuses 0x1000
// Windows allocator gives new address 0x2000

EXPECT_NE(old, new);  // FAILS on Linux!
```

Linux's glibc allocator is more aggressive about reusing freed memory than Windows allocator.

### Fix
Test functionality instead of implementation details:
```cpp
// ❌ BAD: Tests implementation (allocator behavior)
EXPECT_NE(old_ptr, new_ptr);

// ✅ GOOD: Tests functionality
EXPECT_EQ(cache_size, 0);
EXPECT_EQ(texture->width, 256);
```

### Prevention
- Test observable behavior, not implementation details
- Memory addresses are implementation details
- Allocator behavior is platform-specific
- Focus on "what" code does, not "how"

### Learning
**Critical QA principle:** Test the contract (public API behavior), not the implementation (internal details). Implementation can vary across platforms while behavior remains consistent.

---

## Bug Summary

| # | Description | Severity | Platform | Status |
|---|-------------|----------|----------|--------|
| 1-6 | [Previous bugs from renderer] | - | - | Fixed |
| 7 | Uninitialized pointer | High | All | Fixed |
| 8 | Path separator escaping | High | All | Fixed |
| 9 | Allocator pointer comparison | Medium | Linux | Fixed |

---

## Key Patterns

### Root Cause Categories
1. **Memory management** (33%): Uninitialized pointers
2. **String handling** (33%): Escape sequences
3. **Platform differences** (33%): Allocator behavior

### Detection Methods
- Unit tests (100%)
- Cross-platform CI/CD (33%)
- Manual testing (0%)

### Prevention Strategies
- Initialize all variables
- Test on multiple platforms
- Test behavior, not implementation
- Use cross-platform abstractions (`std::filesystem`)

---

## Conclusion

All bugs were caught during development through:
- ✅ Comprehensive unit testing
- ✅ Cross-platform CI/CD
- ✅ Test-Driven Development methodology

**Zero bugs reached production.**

## QA Takeaway

-Most bugs found during development is a bug that would have reached users. Automated tests prevent regression of all fixed bugs.
-Performance testing should be seperated from functional testing. Performance tests can be flakey due to OS level caching which 
can cause repeated runs of tests to show better performance results. This leads to "it works on my machine" scenarios. Tests 
should either implement statistical analysis to account for OS level caching or test on dedicated VMs or containers.
-CI workflows do not follow relative file paths like a local machine. A dedicated file path variable must be defined in 
a yml file for github actions to function.  