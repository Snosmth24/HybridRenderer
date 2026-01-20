# Architecture Overview

## System Design

### Component Diagram
```
┌─────────────────────────────────────────────────────┐
│                  Application                        │
└──────────────────┬──────────────────────────────────┘
                   │
        ┌──────────┴──────────┐
        │                     │
        ▼                     ▼
┌───────────────┐    ┌────────────────┐
│    Renderer   │    │  AssetLoader   │
└───────┬───────┘    └────────┬───────┘
        │                     │
        │                     ├─────► stb_image
        │                     │
        ▼                     ▼
┌───────────────┐    ┌────────────────┐
│IGraphicsContext│   │  Texture Cache │
└───────┬───────┘    └────────────────┘
        │
        ├─────► MockGraphicsContext (Testing)
        │
        └─────► VulkanContext (Production)
```

### Asset Loading Architecture
```
loadTexture(path)
     │
     ▼
┌─────────────┐     YES    ┌──────────────┐
│ Check Cache ├────────────►│ Return Cache │
└──────┬──────┘             └──────────────┘
       │ NO
       ▼
┌─────────────┐
│  stbi_load  │ ← Disk I/O (~15ms)
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ Store Cache │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   Return    │
└─────────────┘
```

---

## Design Principles

### 1. Interface-Based Design
```cpp
// Abstraction enables testing without GPU
class IGraphicsContext {
public:
    virtual ~IGraphicsContext() = default;
    virtual bool initialize() = 0;
    virtual void cleanup() = 0;
};

// Production implementation
class VulkanContext : public IGraphicsContext { /* ... */ };

// Test implementation
class MockGraphicsContext : public IGraphicsContext { /* ... */ };
```

**Benefits:**
- Test without GPU hardware
- Fast test execution (<20ms vs ~3500ms)
- Easy to mock dependencies
- Supports dependency injection

---

### 2. RAII (Resource Acquisition Is Initialization)
```cpp
class AssetLoader {
    ~AssetLoader() {
        clearCache();  // Automatic cleanup
    }
};

struct Texture {
    ~Texture() {
        if (data) {
            stbi_image_free(data);  // Automatic memory management
        }
    }
};
```

**Benefits:**
- No memory leaks
- Exception-safe
- Automatic cleanup
- Predictable resource management

---

### 3. Single Responsibility Principle
```cpp
// AssetLoader: ONLY handles loading and caching
class AssetLoader {
    Texture* loadTexture(const std::string& path);
    void clearCache();
};

// Renderer: ONLY handles rendering
class Renderer {
    void render();
    void setTexture(Texture* tex);
};
```

**Benefits:**
- Easy to test
- Easy to understand
- Easy to modify
- Reusable components

---

## Testing Architecture

### Test Pyramid
```
        ┌─────────┐
        │ Manual  │ (0 tests - automation only)
        └─────────┘
       ┌───────────┐
       │Integration│ (17 tests - GPU required)
       └───────────┘
     ┌──────────────┐
     │     Unit     │ (66+ tests - headless)
     └──────────────┘
```

### Mock Object Pattern
```cpp
// Real implementation
class GLFWWindow : public IWindow {
    bool shouldClose() const override {
        return glfwWindowShouldClose(window);
    }
};

// Test double
class MockWindow : public IWindow {
    bool closed = false;
    
    bool shouldClose() const override {
        return closed;  // Controllable in tests!
    }
    
    void setClosed(bool value) {
        closed = value;  // Test can control this
    }
};
```

**Benefits:**
- No GPU required for 75% of tests
- Fast test execution
- Deterministic behavior
- Full control in tests

---

## Performance Optimization

### Caching Strategy

**Problem:** Loading textures from disk is slow (10-50ms per load)

**Solution:** Cache loaded textures in memory

**Implementation:**
```cpp
std::unordered_map<std::string, Texture*> cache;  // O(1) lookup

Texture* loadTexture(const std::string& path) {
    auto it = cache.find(path);  // Fast hash lookup
    if (it != cache.end()) {
        return it->second;  // Instant return (~0.01ms)
    }
    // ... load from disk ...
}
```

**Results:**
- First load: ~15,000µs (disk I/O)
- Cached load: ~10µs (hash table lookup)
- Speedup: ~1,500x

---

## Cross-Platform Considerations

### File Paths
```cpp
// ✅ CORRECT: Works on Windows + Linux
"test_assets/image.png"

// ❌ WRONG: Only works on Windows
"test_assets\\image.png"
```

### Path Handling
```cpp
// Modern C++17 approach
std::filesystem::path assetPath = basePath / "textures" / "player.png";
```

### Platform Differences Encountered
1. **Path separators** (`/` vs `\`)
2. **Memory allocator behavior** (address reuse)
3. **File system case sensitivity**

**Solution:** Always test on both Windows and Linux in CI/CD

---

## Memory Management

### Ownership Rules

**Before Caching:**
```cpp
Texture* tex = loader->loadTexture("image.png");
delete tex;  // ← Caller owns texture
```

**After Caching:**
```cpp
Texture* tex = loader->loadTexture("image.png");
// Don't delete! AssetLoader owns it
// Loader's destructor will clean up
```

### Cache Cleanup
```cpp
AssetLoader::~AssetLoader() {
    clearCache();  // Delete all cached textures
}

void AssetLoader::clearCache() {
    for (auto& pair : cache) {
        delete pair.second;  // Free each texture
    }
    cache.clear();  // Empty the map
}
```

---

## Build System

### CMake Structure
```cmake
# Project
project(HybridRenderer)

# Libraries
add_subdirectory(external/glfw)
add_subdirectory(external/googletest)

# Main executable
add_executable(HybridRenderer main.cpp ${SOURCES})

# Test executable
add_executable(HybridRenderer_unit_tests ${TEST_SOURCES})

# Copy test assets automatically
add_custom_command(TARGET HybridRenderer_unit_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/test_assets
        $<TARGET_FILE_DIR:HybridRenderer_unit_tests>/test_assets
)
```

---

## CI/CD Pipeline

### GitHub Actions Workflow
```yaml
jobs:
  unit-tests-windows:
    steps:
      - Checkout code
      - Verify dependencies
      - Configure CMake
      - Build
      - Copy test assets
      - Run tests
  
  unit-tests-linux:
    steps:
      - Checkout code
      - Install X11 dependencies
      - Verify dependencies
      - Configure CMake
      - Build
      - Copy test assets
      - Run tests
  
  status:
    needs: [unit-tests-windows, unit-tests-linux]
    steps:
      - Generate summary report
```

**Benefits:**
- Automated testing on every push
- Cross-platform validation
- Fast feedback (~2 minutes)
- Prevents platform-specific regressions

---

## Code Quality Practices

### Test-Driven Development
1. Write failing test (RED)
2. Write minimal code to pass (GREEN)
3. Improve code quality (REFACTOR)
4. Commit with descriptive message

### Code Reviews (Self)
- All commits reviewed before push
- Check for memory leaks
- Verify cross-platform compatibility
- Ensure tests pass

### Documentation
- Inline comments for complex logic
- TDD journey documentation
- Architecture diagrams
- Bug tracking with root cause analysis

---

## Future Architectural Improvements

Potential enhancements:
1. **Thread safety** - Add mutex for concurrent access
2. **LRU cache** - Evict least-recently-used textures when cache is full
3. **Async loading** - Load textures on background thread
4. **Resource pooling** - Reuse texture allocations
5. **Compression** - Support DDS/KTX compressed formats

---

## Summary

This architecture demonstrates:
- ✅ Clean separation of concerns
- ✅ Testable design
- ✅ Performance optimization
- ✅ Cross-platform compatibility
- ✅ Professional development practices

**Key achievement:** 75% of tests run headless (no GPU) while maintaining full functional coverage.