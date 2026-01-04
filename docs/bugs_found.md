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

## Statistics

- **Total bugs found:** 6
- **Critical:** 1
- **Major:** 2
- **Medium:** 1
- **Blockers (CI):** 2
- **Caught by:** Manual testing (4), CI (2)
- **Prevention:** Added 66 automated tests

## QA Takeaway

Every bug found during development is a bug that would have reached users. Automated tests prevent regression of all fixed bugs.