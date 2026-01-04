# Contributing to Hybrid Renderer

## Development Workflow

1. **Fork the repository**
2. **Create a feature branch** (`git checkout -b feature/amazing-feature`)
3. **Make your changes**
4. **Run tests** (`test_quick.bat` or `./test_quick.sh`)
5. **Commit** (`git commit -m 'Add amazing feature'`)
6. **Push** (`git push origin feature/amazing-feature`)
7. **Open Pull Request**

## Before Submitting PR

### ✅ Required

- [ ] All unit tests pass
- [ ] Code compiles without warnings
- [ ] No validation layer errors (Debug build)
- [ ] Documentation updated if needed

### ⚠️ Recommended

- [ ] Integration tests pass (if you have GPU)
- [ ] Tested on Windows
- [ ] No performance regressions

## Testing Requirements

### Unit Tests (Required)
```cmd
# Windows
test_quick.bat

# Linux
./test_quick.sh
```

**Must pass before submitting PR.**

### Integration Tests (Recommended)
```cmd
# Windows
test_full.bat

# Linux
./test_full.sh
```

**Should pass if you have GPU.**

## Code Style

- Use modern C++17 features
- Follow existing code structure
- Add comments for complex logic
- Use RAII for resource management
- Prefer interfaces over concrete types

## Adding Tests

### For New Features

1. **Add unit tests** (tests/unit/)
   - Test with mock objects
   - Fast execution (< 1ms per test)
   - No GPU required

2. **Add integration tests** (tests/integration/)
   - Test with real GPU
   - Slower execution
   - Verify actual functionality

### Test Naming
```cpp
TEST(ComponentTest, BehaviorBeingTested) {
    // Arrange
    // Act
    // Assert
}
```

## Questions?

Open an issue or discussion on GitHub.