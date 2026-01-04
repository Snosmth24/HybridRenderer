# Cross-Platform Testing

## Test Matrix

| Platform | Unit Tests | Integration Tests | Notes |
|----------|-----------|------------------|-------|
| Windows 11 | ✅ | ✅ | Primary development platform |
| Ubuntu 22.04 | ✅ | ✅ | CI/CD via GitHub Actions |
| Ubuntu 20.04 | ✅ | ⚠️ | Older Vulkan drivers |
| macOS 13+ | ✅ | ⚠️ | Via MoltenVK (limited features) |
| macOS 12 | ✅ | ❌ | MoltenVK too old |

## Platform-Specific Issues

### Windows
- ✅ Full Vulkan support
- ✅ All validation layers work
- ✅ Best performance

### Linux
- ✅ Full Vulkan support
- ⚠️ Driver quality varies by GPU vendor
- ⚠️ X11 vs Wayland differences
- 💡 Works great on modern distros

### macOS
- ⚠️ Vulkan via MoltenVK (translation layer)
- ⚠️ Some Vulkan features unavailable
- ⚠️ Performance lower than native Metal
- ❌ Validation layers limited

## CI/CD Platform Coverage
```yaml
# .github/workflows/test.yml
matrix:
  os: [windows-latest, ubuntu-latest, macos-latest]
  
  include:
    - os: windows-latest
      run_integration: true    # Has GPU in some runners
    - os: ubuntu-latest
      run_integration: false   # Headless server
    - os: macos-latest
      run_integration: false   # CI has no GPU
```

## Recommendations

**For Development:**
- Primary: Windows (best tooling, full Vulkan)
- Secondary: Linux (production servers often Linux)
- Occasional: macOS (if targeting Mac users)

**For Testing:**
- Always: Unit tests on all platforms (fast, headless)
- Regularly: Integration tests on Windows (full Vulkan)
- Occasionally: Integration tests on Linux (CI/CD)
- Rarely: Integration tests on macOS (limited Vulkan)

**For Release:**
- Test on all three platforms
- Verify unit tests pass on all
- Verify integration tests pass on Windows + Linux
- Document any macOS limitations