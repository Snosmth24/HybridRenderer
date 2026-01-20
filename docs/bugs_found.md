# Bugs Found During Development

This document tracks all bugs discovered and fixed during development - demonstrating QA mindset and debugging skills.

---

## Bug #1: Crash on Window Close
**Severity:** Critical  
**Status:** ✅ Fixed

**Description:**  
Application crashed with access violation when closing the window.

**Root Cause:**  
GPU was still processing frames during cleanup. Called `cleanup()` before waiting for device idle.

**Symptoms:**
```
Exception thrown: read access violation.
this->device was 0xFFFFFFFFFFFFFFFF.
```

**Fix:**
```cpp
void VulkanContext::cleanup() {
    vkDeviceWaitIdle(device);  // ← Added this
    // ... rest of cleanup
}
```

**Test Added:** `VulkanContextTest.CleanupDoesntCrashAfterInit`

**Lesson:** Always synchronize GPU before cleanup.

---

## Bug #2: White Screen on First Frame
**Severity:** Major  
**Status:** ✅ Fixed

**Description:**  
First frame displayed white screen instead of clear color.

**Root Cause:**  
Image was acquired twice per frame (in both `beginFrame` and `endFrame`), causing index mismatch.

**Symptoms:**
- First frame: white screen
- Subsequent frames: correct rendering
- No validation errors

**Fix:**
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

**Test Added:** `VulkanContextTest.CanRenderSingleFrame`

**Lesson:** Careful state management across function boundaries.

---

## Bug #3: Uninitialized Swapchain Extent
**Severity:** Major  
**Status:** ✅ Fixed

**Description:**  
Random crashes during swapchain operations.

**Root Cause:**  
`swapChainExtent` member variable not initialized in constructor.

**Symptoms:**
```
Access violation reading location 0x0000000000000000
```

**Fix:**
```cpp
VulkanContext::VulkanContext(IWindow* window)
    : swapChainExtent{0, 0}  // ← Added initialization
    , swapChainImageFormat(VK_FORMAT_UNDEFINED)
{
}
```

**Test Added:** `VulkanContextTest.InitializesSuccessfully`

**Lesson:** Always initialize all member variables.

---

## Bug #4: Double Cleanup Crash
**Severity:** Medium  
**Status:** ✅ Fixed

**Description:**  
Calling `cleanup()` twice caused crash.

**Root Cause:**  
No guard against double cleanup - tried to destroy already-destroyed Vulkan objects.

**Fix:**
```cpp
void cleanup() {
    if (device == VK_NULL_HANDLE) {
        return;  // ← Added guard
    }
    // ... cleanup code
    device = VK_NULL_HANDLE;
}
```

**Test Added:** `VulkanContextTest.DoubleCleanupDoesntCrash`

**Lesson:** Make cleanup idempotent.

---

## Bug #5: CI Timeout on Submodule Fetch
**Severity:** Blocker (CI)  
**Status:** ✅ Fixed

**Description:**  
GitHub Actions timed out fetching large submodules.

**Root Cause:**  
GLFW + GoogleTest submodules (~100MB) too large for default timeout.

**Fix:**
- Committed dependencies directly
- Removed submodule complexity
- Faster CI (3 seconds vs 2 minutes)

**Lesson:** Optimize for CI environment constraints.

---

## Bug #6: Missing Namespace in Tests (Linux)
**Severity:** Blocker (CI)  
**Status:** ✅ Fixed

**Description:**  
Tests compiled on Windows but failed on Linux with "identifier not found" error.

**Root Cause:**  
Used `now()` instead of `std::chrono::high_resolution_clock::now()`. Windows compiler was more permissive.

**Fix:**
```cpp
auto start = std::chrono::high_resolution_clock::now();  // ← Fully qualified
```

**Lesson:** Different compilers have different strictness - test on multiple platforms.

---

## Bug #7: Uninitialized Pointer in Test Fixture

**Date:** 2025-01-16  
**Severity:** High (Crash)  
**Component:** Test Infrastructure  
**Platform:** All

### Symptoms
Tests crashed intermittently with no clear error message.

### Root Cause
Test fixture declared pointer without initialization:
```cpp
class AssetLoaderTest : public ::testing::Test {
protected:
    AssetLoader* loader;  // ← Uninitialized! Contains garbage
```

`TearDown()` attempted to delete garbage pointer → crash.

### Fix
```cpp
AssetLoader* loader = nullptr;  // ← Initialize to nullptr

void TearDown() override {
    if (loader) {  // ← Safe check before delete
        delete loader;
    }
}
```

### Prevention
- Always initialize member variables, especially pointers
- Use `= nullptr` for pointers
- Check for null before deleting in cleanup code

### Learning
C++ doesn't initialize variables automatically. Uninitialized pointers are a common source of undefined behavior and crashes.

---

## Bug #8: Path Separator Escape Sequence

**Date:** 2025-01-16  
**Severity:** High (Feature doesn't work)  
**Component:** Asset Loading  
**Platform:** All

### Symptoms
File loading failed with error "can't fopen" even though files existed.

### Root Cause
Copying paths from Windows File Explorer includes backslashes:
```cpp
std::string path = "test_assets\test.png";
                              ^^
// C++ interprets \t as TAB character!
// Actual path: "test_assets[TAB]est.png"
```

### Fix
```cpp
// Use forward slashes (works on Windows + Linux)
std::string path = "test_assets/test.png";
```

### Prevention
- Always use forward slashes in C++ path strings
- Windows accepts `/` as path separator
- Forward slashes are cross-platform compatible
- Or use `std::filesystem::path` for automatic handling

### Learning
Backslash is an escape character in C++ strings. Use `/` for paths or escape backslashes (`\\`).

---

## Bug #9: Platform-Specific Memory Allocator Behavior

**Date:** 2025-01-17  
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