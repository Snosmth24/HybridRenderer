# Installation - Linux

## Prerequisites
```bash
# Update package list
sudo apt update

# Install build tools
sudo apt install -y build-essential cmake git

# Install Vulkan SDK
sudo apt install -y vulkan-tools vulkan-validationlayers-dev

# Install X11 development headers (for GLFW)
sudo apt install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

# Verify Vulkan
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

## Troubleshooting

**Issue: "vulkan: Cannot find ICD"**

Solution:
```bash
sudo apt install mesa-vulkan-drivers  # For Intel/AMD
# OR
sudo apt install nvidia-vulkan-icd     # For NVIDIA
```

**Issue: "GLFW: X11 not available"**

Solution:
```bash
sudo apt install xorg-dev
```