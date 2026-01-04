# Hybrid Renderer

[![CI](https://github.com/Snosmth24/HybridRenderer/actions/workflows/ci.yml/badge.svg)](https://github.com/Snosmth24/HybridRenderer/actions/workflows/ci.yml)
[![Unit Tests](https://img.shields.io/badge/unit%20tests-passing-brightgreen)](https://github.com/Snosmth24/HybridRenderer/actions)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

GPU rendering engine combining rasterization and ray tracing with comprehensive test coverage.

## Features

- ✅ Vulkan 1.3 rendering pipeline
- ✅ 49 unit tests (headless, < 20ms)
- ✅ 17 integration tests (GPU required)
- ✅ Automated CI/CD pipeline
- ✅ Cross-platform (Windows, Linux)
- ✅ Mock-based testing for rapid iteration

## Quick Start

### Windows
```cmd
git clone https://github.com/Snosmth24/HybridRenderer.git
cd HybridRenderer
git submodule update --init --recursive
build.bat
```

### Linux
```bash
git clone https://github.com/Snosmth24/HybridRenderer.git
cd HybridRenderer
git submodule update --init --recursive
./build.sh
```

## Testing

### Quick Test (No GPU Required)
```cmd
# Windows
test_quick.bat

# Linux
./test_quick.sh
```

**Output:**
```
✅ 49 unit tests passed in 15ms
```

### Full Test Suite (GPU Required)
```cmd
# Windows
test_full.bat

# Linux
./test_full.sh
```

**Output:**
```
✅ 49 unit tests passed in 15ms
✅ 17 integration tests passed in 3421ms
```

## Architecture
```
HybridRenderer/
├── src/
│   ├── interfaces/      # Abstract contracts (IWindow, IGraphicsContext)
│   ├── renderer/        # Core rendering logic
│   ├── vulkan/          # Vulkan implementation
│   └── platform/        # GLFW windowing
├── tests/
│   ├── unit/            # Fast tests (49 tests, ~15ms, no GPU)
│   ├── integration/     # Slow tests (17 tests, ~3500ms, needs GPU)
│   └── mocks/           # Test doubles for headless testing
└── .github/
    └── workflows/       # CI/CD automation
```

## Test Coverage

| Type | Count | Time | Requirements |
|------|-------|------|--------------|
| Unit | 49 | 15ms | None (headless) |
| Integration | 17 | 3500ms | GPU + drivers |
| **Total** | **66** | **3515ms** | - |

## CI/CD

- ✅ Automated testing on every commit
- ✅ Runs on Windows and Linux
- ✅ Unit tests always run (headless)
- ⚠️ Integration tests optional (need GPU)

## Requirements

### Development
- CMake 3.20+
- C++17 compiler
- Vulkan SDK 1.3+
- GPU with Vulkan support

### Testing (Unit Tests Only)
- CMake 3.20+
- C++17 compiler
- **No GPU required!**

## Technologies

- **Graphics API:** Vulkan 1.3
- **Windowing:** GLFW 3.x
- **Math:** GLM
- **Testing:** Google Test
- **Build:** CMake
- **CI/CD:** GitHub Actions

## Project Goals

This project demonstrates:
- Modern C++ graphics programming
- Professional testing practices (unit + integration)
- Headless testing for CI/CD
- Mock-based testing for rapid iteration
- Interface-based architecture for testability
- Cross-platform development

## License

MIT License - see LICENSE file for details

## Author

Built for NVIDIA QA Engineer application, demonstrating:
- Vulkan API expertise
- Automated testing
- CI/CD integration
- Mock-based testing
- Cross-platform development