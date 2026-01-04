# Integration Tests

## Overview

Integration tests verify the application works with **real hardware and drivers**.

## Requirements

- **GPU:** NVIDIA, AMD, or Intel GPU with Vulkan support
- **Drivers:** Latest GPU drivers installed
- **Display:** Monitor or display server (X11, Wayland, Windows Desktop)
- **Vulkan SDK:** Installed with validation layers

## Execution Time

- **Unit tests:** < 20ms (no GPU)
- **Integration tests:** 3-5 seconds (real GPU)

⚠️ **Run unit tests first for fast feedback!**

## What These Test

### WindowIntegrationTest
- Real GLFW window creation
- OS window manager interaction
- Actual input handling
- Multiple window management

### VulkanContextIntegrationTest
- Real Vulkan initialization
- Real GPU command submission
- Real swapchain management
- Real driver validation

## When to Run

**Always:**
- Before commits (unit tests only)
- In CI/CD (unit tests only)

**Occasionally:**
- Before releases (integration tests)
- After driver updates
- When testing on new hardware
- When debugging GPU-specific issues

## Running Integration Tests
```cmd
# Build integration tests
cmake --build . --config Debug --target HybridRenderer_integration_tests

# Run integration tests
bin\Debug\HybridRenderer_integration_tests.exe

# Run specific test
bin\Debug\HybridRenderer_integration_tests.exe --gtest_filter="VulkanContextIntegrationTest.InitializesWithRealGPU"
```

## Test Organization
```
tests/
├── unit/                    # Fast, no GPU (always run)
│   ├── MockObjectsTest.cpp
│   └── RendererUnitTest.cpp
│
└── integration/             # Slow, needs GPU (run occasionally)
    ├── WindowIntegrationTest.cpp
    └── VulkanContextIntegrationTest.cpp
```

## CI/CD Strategy

| Environment | Unit Tests | Integration Tests |
|-------------|-----------|------------------|
| Developer machine | ✅ Every save | ✅ Before commit |
| Pull request | ✅ Always | ❌ Skip (no GPU) |
| Main branch | ✅ Always | ✅ If GPU available |
| Release builds | ✅ Always | ✅ Always |

## Performance Baseline

**Unit Tests:**
- MockObjectsTest: ~5ms
- RendererUnitTest: ~10ms
- **Total: ~15ms**

**Integration Tests:**
- WindowIntegrationTest: ~500ms
- VulkanContextIntegrationTest: ~3000ms
- **Total: ~3500ms**

**Integration tests are 200x slower!** That's why we separate them.