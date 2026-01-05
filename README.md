# Hybrid Renderer

[![CI](https://github.com/Snosmth24/HybridRenderer/actions/workflows/ci.yml/badge.svg)](https://github.com/Snosmth24/HybridRenderer/actions/workflows/ci.yml)
[![Tests](https://img.shields.io/badge/tests-66%20passing-brightgreen)](https://github.com/Snosmth24/HybridRenderer)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-blue)](https://github.com/Snosmth24/HybridRenderer)
<!-- Code coverage -->
[![Coverage](https://img.shields.io/badge/coverage-90%25-brightgreen)](coverage.html)

<!-- Last commit -->
[![Last Commit](https://img.shields.io/github/last-commit/YOUR_USERNAME/HybridRenderer)](https://github.com/YOUR_USERNAME/HybridRenderer/commits)

<!-- Issues -->
[![Issues](https://img.shields.io/github/issues/YOUR_USERNAME/HybridRenderer)](https://github.com/YOUR_USERNAME/HybridRenderer/issues)

<!-- Stars -->
[![Stars](https://img.shields.io/github/stars/YOUR_USERNAME/HybridRenderer?style=social)](https://github.com/YOUR_USERNAME/HybridRenderer)

> GPU rendering engine demonstrating **professional QA engineering practices** for NVIDIA application.

## QA Engineering Focus

This project showcases:
- ✅ **Automated Testing** - 66 tests, 100% automated
- ✅ **CI/CD Pipeline** - GitHub Actions, multi-platform
- ✅ **Headless Testing** - Unit tests run without GPU
- ✅ **Mock-Based Testing** - Fast iteration (< 20ms)
- ✅ **Cross-Platform** - Windows + Linux
- ✅ **Bug Documentation** - [6 bugs found and fixed](docs/bugs_found.md)

## Quick Start
```bash
# Clone
git clone https://github.com/Snosmth24/HybridRenderer.git
cd HybridRenderer

# Run tests (no GPU needed!)
test_quick.bat          # Windows
./test_quick.sh         # Linux

# Output: ✅ 49 tests passed in 15ms
```

## Test Architecture
```
Tests (66 total)
├── Unit Tests (49) - Headless, < 20ms
│   ├── Mock objects (no GPU)
│   ├── Fast iteration
│   └── CI/CD friendly
└── Integration Tests (17) - Real GPU, ~3500ms
    ├── Vulkan validation
    ├── Driver compatibility
    └── Visual verification
```

## CI/CD Pipeline
```
Git Push
  ↓
GitHub Actions
  ├─ Windows: Build + Test (49 tests)
  └─ Linux:   Build + Test (49 tests)
  ↓
Status: ✅ All Passed
Time: ~2 minutes
```

## Key Features (QA Perspective)

### 1. Test Coverage
- 49 unit tests (business logic)
- 17 integration tests (GPU functionality)
- Edge case testing (window resize, cleanup, errors)
- Performance testing (frame timing)

### 2. Automated Quality Checks
- Every commit tested automatically
- No manual testing for PRs
- Cross-platform validation
- Fast feedback (< 2 minutes)

### 3. Professional Practices
- Mock-based testing for speed
- Interface-based architecture for testability
- Dependency injection for flexibility
- RAII for resource safety

## Technologies

- **Graphics API:** Vulkan 1.3
- **Testing:** Google Test + custom mocks
- **CI/CD:** GitHub Actions
- **Platforms:** Windows, Linux
- **Build:** CMake 3.20+

## Documentation

- [Test Results](docs/test_results.md) - Detailed test metrics
- [Bugs Found](docs/bugs_found.md) - 6 bugs documented and fixed
- [Installation (Linux)](docs/installation_linux.md) - Linux setup guide
- [Contributing](CONTRIBUTING.md) - Development workflow

## Running Tests

### Unit Tests (No GPU Required)
```bash
# Windows
test_quick.bat

# Linux  
./test_quick.sh

# Output
✅ 49 tests passed in 15ms
```

### Full Suite (GPU Required)
```bash
# Windows
test_full.bat

# Linux
./test_full.sh

# Output
✅ 49 unit tests passed in 15ms
✅ 17 integration tests passed in 3421ms
```

## For NVIDIA QA Role

This project demonstrates:

**QA Skills:**
- Automated test creation and maintenance
- CI/CD pipeline configuration
- Cross-platform testing
- Bug documentation and root cause analysis
- Performance testing and profiling

**Technical Skills:**
- Vulkan API knowledge
- C++17 systems programming
- CMake build systems
- Git/GitHub workflows
- Linux and Windows development

**Mindset:**
- "How can this break?" thinking
- Test-first development
- Attention to edge cases
- Documentation of findings

## License

MIT License - See LICENSE file

## Author

Built to demonstrate QA engineering skills for NVIDIA GPU Software QA Engineer application.

**Contact:** [Tamiragov24@gmail.com]  