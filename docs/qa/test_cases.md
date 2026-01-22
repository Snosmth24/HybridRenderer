# Test Cases: Asset Loading System

## Test Case Format

Each test case follows this structure:
- **ID:** Unique identifier
- **Priority:** Critical/High/Medium/Low
- **Type:** Functional/Performance/Security/Compatibility
- **Description:** What is being tested
- **Preconditions:** Setup required
- **Test Steps:** How to execute
- **Expected Result:** What should happen
- **Actual Result:** What actually happened
- **Status:** Pass/Fail/Blocked/Not Run
- **Automated:** Yes/No

---

## Functional Test Cases

### TC-001: Load Valid PNG Image

**Priority:** Critical  
**Type:** Functional  
**Automated:** Yes

**Description:**  
Verify that the asset loader can successfully load a valid PNG image file.

**Preconditions:**
- AssetLoader instance created
- test_256x256.png exists in test_assets/

**Test Steps:**
1. Call `loader->loadTexture("test_assets/test_256x256.png")`
2. Verify returned Texture pointer is not null
3. Verify texture dimensions are correct
4. Verify texture data pointer is not null

**Expected Result:**
- Texture pointer is valid
- width = 256
- height = 256
- channels = 3 or 4
- data != nullptr

**Actual Result:**  
✅ PASS - All assertions passed

**Automation:**  
`TEST_F(AssetLoaderTest, LoadsCorrectDimensions)`

**Notes:**  
This is the core happy path test case.

---

### TC-002: Load Non-Existent File

**Priority:** High  
**Type:** Functional - Error Handling  
**Automated:** Yes

**Description:**  
Verify that attempting to load a non-existent file throws an appropriate exception.

**Preconditions:**
- AssetLoader instance created
- Specified file does NOT exist

**Test Steps:**
1. Call `loader->loadTexture("nonexistent_file.png")`
2. Catch exception
3. Verify exception is std::runtime_error
4. Verify error message contains filename

**Expected Result:**
- std::runtime_error thrown
- Error message contains "nonexistent_file.png"
- Error message contains stb_image error reason

**Actual Result:**  
✅ PASS - Exception thrown with correct message

**Automation:**  
`TEST_F(AssetLoaderTest, ThrowsExceptionForMissingFile)`

**Notes:**  
Critical for preventing crashes in production.

---

### TC-003: Load JPEG Image

**Priority:** High  
**Type:** Functional  
**Automated:** Planned

**Description:**  
Verify that the asset loader can load JPEG format images in addition to PNG.

**Preconditions:**
- AssetLoader instance created
- test_photo.jpg exists in test_assets/

**Test Steps:**
1. Call `loader->loadTexture("test_assets/test_photo.jpg")`
2. Verify returned Texture pointer is not null
3. Verify texture data is loaded
4. Verify channels = 3 (JPEG typically has no alpha)

**Expected Result:**
- Texture loads successfully
- Data is valid
- Channels = 3 (RGB)

**Actual Result:**  
⏳ NOT RUN - Planned for Cycle 10

**Automation:**  
Planned

---

### TC-004: Load Multiple Different Images

**Priority:** Medium  
**Type:** Functional  
**Automated:** Yes

**Description:**  
Verify that multiple different images can be loaded and managed simultaneously.

**Preconditions:**
- AssetLoader instance created
- Multiple test images exist

**Test Steps:**
1. Load test_256x256.png
2. Load test_512x512.png
3. Verify both textures have correct dimensions
4. Verify cache size = 2

**Expected Result:**
- First texture: 256x256
- Second texture: 512x512
- Cache contains 2 entries
- Different memory addresses

**Actual Result:**  
✅ PASS

**Automation:**  
`TEST_F(AssetLoaderTest, CachesDifferentTextures)`

---

## Cache Behavior Test Cases

### TC-005: Cache Returns Same Pointer

**Priority:** Critical  
**Type:** Functional - Caching  
**Automated:** Yes

**Description:**  
Verify that loading the same image twice returns the cached instance (same pointer).

**Preconditions:**
- AssetLoader instance created
- Cache is empty

**Test Steps:**
1. Load test_256x256.png → save pointer as ptr1
2. Load test_256x256.png again → save pointer as ptr2
3. Compare pointers

**Expected Result:**
- ptr1 == ptr2 (same memory address)
- Cache size = 1
- Only one disk I/O occurred

**Actual Result:**  
✅ PASS

**Automation:**  
`TEST_F(AssetLoaderTest, CachesLoadedTextures)`

**Notes:**  
Core caching functionality validation.

---

### TC-006: Clear Cache

**Priority:** High  
**Type:** Functional - Cache Management  
**Automated:** Yes

**Description:**  
Verify that clearCache() removes all cached textures and frees memory.

**Preconditions:**
- AssetLoader has cached textures

**Test Steps:**
1. Load 2-3 different textures
2. Verify cache size > 0
3. Call clearCache()
4. Verify cache size = 0
5. Load texture again
6. Verify new texture loads successfully

**Expected Result:**
- Cache size becomes 0
- No crashes or memory errors
- Fresh loading works after clear

**Actual Result:**  
✅ PASS

**Automation:**  
`TEST_F(AssetLoaderTest, ClearCacheRemovesAllTextures)`

---

### TC-007: Cache Size Tracking

**Priority:** Medium  
**Type:** Functional  
**Automated:** Yes

**Description:**  
Verify that getCacheSize() accurately reports number of cached textures.

**Preconditions:**
- AssetLoader instance created

**Test Steps:**
1. Verify initial cache size = 0
2. Load texture A → verify size = 1
3. Load texture A again → verify size still = 1
4. Load texture B → verify size = 2
5. Clear cache → verify size = 0

**Expected Result:**
- Cache size accurately tracks unique textures
- Repeated loads don't increase size

**Actual Result:**  
✅ PASS

**Automation:**  
`TEST_F(AssetLoaderTest, CacheSizeRemainsConstant)`

---

## Performance Test Cases

### TC-008: Cache Performance Improvement

**Priority:** High  
**Type:** Performance  
**Automated:** Yes

**Description:**  
Verify that cached texture loading is significantly faster than disk loading.

**Preconditions:**
- AssetLoader instance created
- OS file cache warmed up (to isolate app cache)

**Test Steps:**
1. Warm up OS cache by loading texture once
2. Clear application cache
3. Measure time to load texture (cache miss)
4. Measure time to load same texture (cache hit)
5. Compare times

**Expected Result:**
- Cache hit is faster than cache miss
- Typical speedup: 100x - 10,000x depending on file size and OS cache state

**Actual Result:**  
✅ PASS - Cache hit consistently faster

**Automation:**  
`TEST_F(AssetLoaderTest, CacheReducesLoadTime)`

**Notes:**  
Exact speedup varies due to OS-level caching. Test validates relative improvement, not absolute values.

---

### TC-009: Load Time Benchmark

**Priority:** Medium  
**Type:** Performance  
**Automated:** Partial

**Description:**  
Establish baseline performance metrics for texture loading.

**Preconditions:**
- Clean system state
- Test images available

**Test Steps:**
1. Load 256x256 PNG (cold) → measure time
2. Load 512x512 PNG (cold) → measure time
3. Load 1024x1024 PNG (cold) → measure time
4. Record baseline metrics

**Expected Result:**
- 256x256: < 50ms
- 512x512: < 100ms
- 1024x1024: < 200ms

**Actual Result:**  
✅ PASS - All within acceptable ranges

**Automation:**  
Manual benchmarking script

**Notes:**  
Actual times vary by hardware. These are guidelines.

---

## Cross-Platform Test Cases

### TC-010: Windows Path Handling

**Priority:** Critical  
**Type:** Compatibility  
**Automated:** Yes (via CI/CD)

**Description:**  
Verify that file paths work correctly on Windows platform.

**Preconditions:**
- Running on Windows
- Test assets copied to build directory

**Test Steps:**
1. Load texture using forward slashes: "test_assets/test.png"
2. Verify successful load
3. Verify paths with spaces work (if applicable)

**Expected Result:**
- Forward slashes work on Windows
- Textures load successfully
- No path-related errors

**Actual Result:**  
✅ PASS

**Automation:**  
GitHub Actions Windows runner

---

### TC-011: Linux Path Handling

**Priority:** Critical  
**Type:** Compatibility  
**Automated:** Yes (via CI/CD)

**Description:**  
Verify that file paths work correctly on Linux platform.

**Preconditions:**
- Running on Ubuntu 22.04+
- Test assets copied to build directory

**Test Steps:**
1. Load texture using forward slashes
2. Verify case-sensitive path handling
3. Verify successful load

**Expected Result:**
- Paths work with forward slashes
- Case sensitivity respected
- No platform-specific errors

**Actual Result:**  
✅ PASS

**Automation:**  
GitHub Actions Linux runner

---

### TC-012: Memory Allocator Behavior

**Priority:** High  
**Type:** Compatibility  
**Automated:** Yes

**Description:**  
Verify that tests don't make assumptions about platform-specific memory allocator behavior.

**Preconditions:**
- Tests run on both Windows and Linux

**Test Steps:**
1. Execute full test suite on Windows
2. Execute full test suite on Linux
3. Compare results

**Expected Result:**
- All tests pass on both platforms
- No platform-specific failures

**Actual Result:**  
✅ PASS (after fix)

**Automation:**  
CI/CD cross-platform validation

**Notes:**  
Initial implementation failed on Linux due to pointer comparison test. Fixed by testing behavior instead of implementation.

**Bug Reference:** Bug #9 in bugs_found.md

---

## Memory Management Test Cases

### TC-013: No Memory Leaks

**Priority:** Critical  
**Type:** Memory Safety  
**Automated:** Partial

**Description:**  
Verify that loading and unloading textures doesn't leak memory.

**Preconditions:**
- Valgrind or ASAN available (Linux)
- AssetLoader instance

**Test Steps:**
1. Run tests under memory checker
2. Load multiple textures
3. Clear cache
4. Destroy AssetLoader
5. Check for memory leaks

**Expected Result:**
- Zero memory leaks reported
- All allocated memory is freed

**Actual Result:**  
✅ PASS - No leaks detected

**Automation:**  
Valgrind in local testing (manual)

**Notes:**  
Future: Add to CI/CD pipeline

---

### TC-014: Destructor Cleanup

**Priority:** Critical  
**Type:** Memory Safety  
**Automated:** Yes

**Description:**  
Verify that AssetLoader destructor properly cleans up cached textures.

**Preconditions:**
- None

**Test Steps:**
1. Create AssetLoader
2. Load textures
3. Destroy AssetLoader
4. Verify no crashes
5. Check memory cleanup (manual)

**Expected Result:**
- Destructor executes without crash
- All textures freed
- No dangling pointers

**Actual Result:**  
✅ PASS

**Automation:**  
`TEST_F(AssetLoaderTest, DestructorClearsCache)`

---

## Edge Case Test Cases

### TC-015: Empty Cache Operations

**Priority:** Medium  
**Type:** Edge Case  
**Automated:** Yes

**Description:**  
Verify that operations on empty cache are safe.

**Preconditions:**
- AssetLoader with empty cache

**Test Steps:**
1. Call getCacheSize() → expect 0
2. Call clearCache() → expect no crash
3. Call getCacheSize() again → expect 0

**Expected Result:**
- No crashes
- Operations handle empty state gracefully

**Actual Result:**  
✅ PASS

**Automation:**  
`TEST_F(AssetLoaderTest, ClearCacheOnEmptyCacheIsSafe)`

---

### TC-016: Repeated Cache Clears

**Priority:** Low  
**Type:** Edge Case  
**Automated:** Yes

**Description:**  
Verify that calling clearCache() multiple times is safe.

**Preconditions:**
- AssetLoader instance

**Test Steps:**
1. Load texture
2. clearCache()
3. clearCache() again
4. clearCache() again
5. Verify no crashes

**Expected Result:**
- No crashes or errors
- Cache remains empty

**Actual Result:**  
✅ PASS

**Automation:**  
`TEST_F(AssetLoaderTest, MultipleClearCachesAreSafe)`

---

## Test Summary

### Test Execution Status

| Status | Count | Percentage |
|--------|-------|------------|
| ✅ Pass | 14 | 87.5% |
| ❌ Fail | 0 | 0% |
| ⏳ Not Run | 2 | 12.5% |
| **Total** | **16** | **100%** |

### Coverage by Priority

| Priority | Total | Pass | Fail | Coverage |
|----------|-------|------|------|----------|
| Critical | 6 | 6 | 0 | 100% |
| High | 5 | 5 | 0 | 100% |
| Medium | 4 | 3 | 0 | 75% |
| Low | 1 | 0 | 0 | 0% |

### Coverage by Type

| Type | Test Cases | Automated | Manual |
|------|------------|-----------|--------|
| Functional | 6 | 5 | 1 |
| Performance | 2 | 2 | 0 |
| Compatibility | 3 | 3 | 0 |
| Memory Safety | 2 | 1 | 1 |
| Edge Cases | 3 | 3 | 0 |

---

## Traceability

See `traceability_matrix.md` for mapping of requirements to test cases.

---

**Document Version:** 1.0  
**Last Updated:** January 2025  
**Author:** [Tigran Amiragov]