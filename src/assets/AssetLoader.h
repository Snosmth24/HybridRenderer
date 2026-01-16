#pragma once
#include <string>

// Forward declaration
struct Texture {
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* data = nullptr;
    
    ~Texture() {
        // Don't delete data yet - we'll manage that properly later
    }
};

class AssetLoader {
public:
    AssetLoader() = default;
    ~AssetLoader() = default;
    
    // Minimal implementation - just return something
    Texture* loadTexture(const std::string& path) {
        return new Texture();
    }
};