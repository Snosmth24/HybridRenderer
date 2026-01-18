#pragma once
#include <string>
#include <stdexcept>
#include <unordered_map>

struct Texture {
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* data = nullptr;
    
    // Destructor declaration (implemented in .cpp)
    ~Texture();
};

class AssetLoader {
private:
    // Cache: maps file path to loaded texture
    std::unordered_map<std::string, Texture*> cache;
    
public:
    AssetLoader() = default;
    ~AssetLoader();  // Now needs custom destructor to clean up cache
    
    // Load a texture from file (returns cached version if already loaded)
    // Throws std::runtime_error if file cannot be loaded
    Texture* loadTexture(const std::string& path);
    
    // Clear all cached textures
    void clearCache();
    
    // Get number of cached textures
    size_t getCacheSize() const;
};