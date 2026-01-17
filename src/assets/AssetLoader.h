#pragma once
#include <string>
#include <stdexcept>

struct Texture {
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* data = nullptr;

    // Destructor - DECLARED here, IMPLEMENTED in .cpp
    ~Texture();
};

class AssetLoader {
public:
    AssetLoader() = default;
    ~AssetLoader() = default;

    Texture* loadTexture(const std::string& path);
};