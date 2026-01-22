# Hybrid Renderer

[![CI](https://github.com/Snosmth24/HybridRenderer/actions/workflows/ci.yml/badge.svg)](https://github.com/Snosmth24/HybridRenderer/actions/workflows/ci.yml)
[![Tests](https://img.shields.io/badge/tests-66%20passing-brightgreen)](https://github.com/Snosmth24/HybridRenderer)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-blue)](https://github.com/Snosmth24/HybridRenderer)
<!-- Code coverage -->
[![Coverage](https://img.shields.io/badge/coverage-90%25-brightgreen)](coverage.html)

<!-- Last commit -->
[![Last Commit](https://img.shields.io/github/last-commit/Snosmth24/HybridRenderer)](https://github.com/Snosmth24/HybridRenderer/commits)

<!-- Issues -->
[![Issues](https://img.shields.io/github/issues/Snosmth24/HybridRenderer)](https://github.com/Snosmth24/HybridRenderer/issues)

<!-- Stars -->
[![Stars](https://img.shields.io/github/stars/Snosmth24/HybridRenderer?style=social)](https://github.com/Snosmth24/HybridRenderer)

# HybridRenderer

GPU rendering application with comprehensive automated testing framework, built to demonstrate professional QA engineering practices.

## Project Overview

**Primary Goal:** Demonstrate QA engineering expertise through automated testing, CI/CD, and quality-focused architecture.

**Key Achievement:** Comprehensive test suite that validates GPU rendering functionality without requiring GPU hardware on every test run.

---

## Features

### Core Rendering
- Vulkan 1.3 graphics pipeline
- GLFW window management
- Cross-platform support (Windows, Linux)

### Asset Loading System ✨ NEW
- **Texture loading** with PNG and JPEG support
- **Intelligent caching** for performance optimization (1000x+ speedup)
- **Error handling** with descriptive error messages
- **Memory management** with automatic cleanup
- **Cross-platform** file path handling

### Testing Infrastructure
- **66+ automated tests** (49 unit, 17+ integration)
- **Mock objects** for GPU-free testing
- **Performance validation** for cache optimization
- **Cross-platform CI/CD** (Windows + Linux)
- **Test-Driven Development** methodology


## QA Methodology & Process

This project demonstrates professional QA engineering practices through comprehensive test documentation and systematic validation.

### Test Artifacts

#### 📋 Test Planning
- **[Test Plan](docs/qa/test_plan.md)** - Strategy, scope, and approach
- **[Test Cases](docs/qa/test_cases.md)** - 16 detailed test scenarios
- **[Traceability Matrix](docs/qa/traceability_matrix.md)** - Requirements coverage mapping

#### 🐛 Defect Management
- **[Bug Reports](docs/bugs_found.md)** - Formal defect tracking with root cause analysis
- **[Bug Template](docs/qa/bug_report_template.md)** - Standardized reporting format

#### 📊 Test Results
- **[Test Results](docs/test_results.md)** - Execution records and metrics
- **[Performance Benchmarks](docs/performance_benchmarks.md)** - Load time and cache metrics

#### 🏗️ Technical Documentation
- **[Architecture](docs/architecture.md)** - System design and testing approach
- **[TDD Journey](docs/tdd/asset_loader_journey.md)** - Development process documentation

### Testing Approach

**Test Strategy:**
- Test-Driven Development (TDD) methodology
- Automated testing with CI/CD validation
- Cross-platform testing (Windows/Linux)
- Risk-based test prioritization

**Test Coverage:**
- 92.3% requirements coverage (12/13 requirements)
- 100% code coverage (asset loading system)
- 87.5% automated test execution
- Zero critical/high severity open bugs

**Quality Metrics:**
- 16 test cases documented and executed
- 3 bugs found and fixed during development
- 100% pass rate on both platforms
- <2 minute CI/CD execution time

---

## Test Coverage

This project showcases:
- ✅ **Automated Testing** - 83 tests, 100% automated
- ✅ **CI/CD Pipeline** - GitHub Actions, multi-platform
- ✅ **Headless Testing** - Unit tests run without GPU
- ✅ **Mock-Based Testing** - Fast iteration (< 20ms)
- ✅ **Cross-Platform** - Windows + Linux
- ✅ **Bug Documentation** - [8 bugs found and fixed](docs/bugs_found.md)

### Unit Tests (Headless - No GPU Required)
```
✓ 49 renderer unit tests (<20ms each)
✓ 16 asset loader tests
  - Texture loading (PNG, JPEG)
  - Cache correctness
  - Performance optimization
  - Error handling
  - Memory management
```

### Integration Tests (GPU Required)
```
✓ 17 integration tests (~3500ms total)
  - Graphics pipeline validation
  - Vulkan context initialization
  - Cross-platform rendering
```

### Performance Tests
```
✓ Cache hit/miss validation
✓ Load time optimization (>1000x speedup measured)
✓ Cross-platform consistency
```

---

## Quick Start

### Prerequisites
```bash
# Windows
- Visual Studio 2019+ with C++ tools
- CMake 3.20+
- Git

# Linux
sudo apt-get install build-essential cmake libgl1-mesa-dev \
  libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libx11-dev
```

### Build and Test
```bash
# Clone repository
git clone https://github.com/[Snosmth24]/HybridRenderer.git
cd HybridRenderer

# Build
cmake -B build
cmake --build build

# Run all tests
cd build
ctest --output-on-failure

# Or run specific test suites
./bin/HybridRenderer_unit_tests --gtest_filter=AssetLoaderTest.*
```

## Architecture Highlights

### Interface-Based Design
```cpp
// Dependency injection enables testing without GPU
IWindow* window = useTestMode ? new MockWindow() : new GLFWWindow();
IGraphicsContext* context = useTestMode ? new MockGraphicsContext() : new VulkanContext();
```

### Asset Loading with Caching
```cpp
// First load: Reads from disk (~15ms)
Texture* tex1 = loader->loadTexture("player.png");

// Second load: Returns cached texture (~0.01ms)
Texture* tex2 = loader->loadTexture("player.png");  // Same pointer!
```

### Test-Driven Development
- All features developed using TDD methodology
- Red → Green → Refactor cycle documented
- 100% code coverage for asset loading system

---

## CI/CD Pipeline

### GitHub Actions Workflow
- **Automated builds** on push/PR
- **Cross-platform testing** (Windows + Linux)
- **Test result reporting** with summary generation
- **Fast feedback** (~2 minute builds)

## Status

[![CI - Unit Tests](https://github.com/Snosmth24/HybridRenderer/actions/workflows/ci.yml/badge.svg)](https://github.com/Snosmth24/HybridRenderer/actions/workflows/ci.yml)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-blue)](#quick-start)
[![C++](https://img.shields.io/badge/C%2B%2B-17-blue)](#technologies)
[![Tests](https://img.shields.io/badge/tests-66%2B%20passing-success)](#test-coverage)

**Current Build Status:**
- ✅ Windows: All tests passing
- ✅ Linux: All tests passing
- ✅ Test Coverage: 100% (asset loading)
---

## Documentation

- [Installation Guide (Linux)](docs/installation_linux.md)
- [Test Results](docs/test_results.md)
- [Bugs Found & Fixed](docs/bugs_found.md)
- [TDD Journey - Asset Loader](docs/tdd/asset_loader_journey.md)
- [Architecture Overview](docs/architecture.md)

---

## Project Goals

### Primary
Demonstrate professional QA engineering skills:
- ✅ Automated testing
- ✅ CI/CD pipeline setup
- ✅ Cross-platform development
- ✅ Test-Driven Development
- ✅ Bug documentation and root cause analysis

### Secondary
Learn modern graphics APIs and rendering techniques while maintaining QA focus.

---

## License

MIT License - See [LICENSE](LICENSE) file for details

---

## Contact

**Seeking QA Engineering opportunities in GPU/graphics software!**

- GitHub: [@Snosmth24](https://github.com/Snosmth24)
- LinkedIn: www.linkedin.com/in/tigran-amiragov-82aa62370
- Email: Tamiragov24@gmail.com

---

## Acknowledgments

- stb_image library for image loading
- Google Test framework
- Vulkan SDK and documentation
- GLFW for window management