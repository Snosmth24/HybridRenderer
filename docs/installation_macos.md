# Installation - macOS

## Prerequisites
```bash
# Install Homebrew (if not installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install build tools
brew install cmake git

# Install Vulkan SDK (includes MoltenVK)
brew install --cask vulkan-sdk

# Verify installation
vulkaninfo | head -20
```

## Build
```bash
# Clone repository
git clone [your-repo-url]
cd HybridRenderer

# Initialize submodules
git submodule update --init --recursive

# Build
./build.sh

# Run
./build/bin/Debug/HybridRenderer
```

## Run Tests
```bash
# Quick test (no GPU)
./test_quick.sh

# Full test (requires GPU)
./test_full.sh
```

## Notes

- macOS uses **MoltenVK** which translates Vulkan to Metal
- Some Vulkan features may not be available on macOS
- Requires macOS 10.15+ for full Vulkan support

## Troubleshooting

**Issue: "dyld: Library not loaded: libvulkan"**

Solution:
```bash
export DYLD_LIBRARY_PATH=/usr/local/lib:$DYLD_LIBRARY_PATH
```

**Issue: "MoltenVK not found"**

Solution:
```bash
brew reinstall --cask vulkan-sdk
```