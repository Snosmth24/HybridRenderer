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

## Bug #7: VS fails to read Relative paths
**Severity:** Minimal  
**Status:** ✅ Fixed

**Description:**  
Tests that run from VS IDE, that require paths to textures assets, crash. VS fails to fallow relative 
paths from imported CMAKE projects. 

**Root Cause:**  
VS creates a seperate build directory and working Directory from which it maps file paths. 

**Symptoms:**
```
Test Crashes
data pointer: 0000000000000000
```

**Fix:**
```
Use Absolute paths in VS cpp
std::string path = <path>;
Texture* texture = loader->loadTexture(path);
```

**Test Added:** 
AssetLoaderTest, ShowWorkingDirectory
AssetLoaderTest, LoaderCanBeCreated
AssetLoaderTest, HasLoadTextureMethod
AssetLoaderTest, LoadsCorrectDimensions
**Lesson:** Run tests from Console

## Bug #8: CI workflow fails to read relative path
**Severity:** Blocker  
**Status:** ✅ Fixed

**Description:**  
Asset Tests work fine on local machine with relative paths but fail on CI servers. 

**Root Cause:**  
Github actions requier dedicated path variable to be defined in ci.yml file. 

**Symptoms:**
```
Tests fail to build on CI server - Exits with error 8 - failure to find file.

```

**Fix:**
```
# Copy to bin/Debug (where executable is on Windows)
        New-Item -ItemType Directory -Force -Path "build\bin\Debug\test_assets"
        Copy-Item -Path "test_assets\*" -Destination "build\bin\Debug\test_assets\" -Recurse -Force
```
```
# Find where the executable actually is
        EXEC_PATH=$(find build -name "HybridRenderer_unit_tests" -type f | head -n 1)
        if [ -n "$EXEC_PATH" ]; then
          EXEC_DIR=$(dirname "$EXEC_PATH")
          echo "Executable found at: $EXEC_PATH"
          echo "Copying to: $EXEC_DIR/test_assets"
          mkdir -p "$EXEC_DIR/test_assets"
          cp -r test_assets/* "$EXEC_DIR/test_assets/"
        fi
```	
**Test Added:** 
1. `ClearCacheDeletesAllTextures` - Verify cache is emptied
2. `ClearCacheOnEmptyCacheIsSafe` - Edge case: clearing empty cache
3. `MultipleClearCachesAreSafe` - Edge case: repeated clears
4. `DestructorClearsCache` - Verify destructor cleanup
5. `ClearCacheFreesMemory` - Verify memory is actually freed
**Lesson:** (Debug)Check file paths in yml and set path variable to implemnt relative file paths.

## Statistics

- **Total bugs found:** 8
- **Critical:** 1
- **Major:** 2
- **Medium:** 1
- **Blockers (CI):** 3
- **Minimal (CI):** 1
- **Caught by:** Manual testing (4), CI (4)
- **Prevention:** Added 76 automated tests

## QA Takeaway

-Most bugs found during development is a bug that would have reached users. Automated tests prevent regression of all fixed bugs.
-Performance testing should be seperated from functional testing. Performance tests can be flakey due to OS level caching which 
can cause repeated runs of tests to show better performance results. This leads to "it works on my machine" scenarios. Tests 
should either implement statistical analysis to account for OS level caching or test on dedicated VMs or containers.
-CI workflows do not follow relative file paths like a local machine. A dedicated file path variable must be defined in 
a yml file for github actions to function.  