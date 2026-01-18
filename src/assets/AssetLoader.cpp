#include "AssetLoader.h"

// Include stb_image (header-only library)
// This must be in exactly ONE .cpp file
#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb_image.h"

#include <iostream>

// Texture destructor implementation
Texture::~Texture() {
    if (data) {
        stbi_image_free(data);
        data = nullptr;
    }
}

// AssetLoader destructor - clean up all cached textures
AssetLoader::~AssetLoader() {
    clearCache();
}

// Clear all cached textures
void AssetLoader::clearCache() {
    for (auto& pair : cache) {
        delete pair.second;
    }
    cache.clear();
}

// Get number of cached textures
size_t AssetLoader::getCacheSize() const {
    return cache.size();
}

// Load texture from file (with caching)
Texture* AssetLoader::loadTexture(const std::string& path) {
    // Check if texture is already in cache
    auto it = cache.find(path);
    if (it != cache.end()) {
        // Cache hit - return cached texture
        std::cout << "[CACHE HIT] Returning cached texture: " << path << std::endl;
        return it->second;
    }
    
    // Cache miss - load the texture
    std::cout << "[CACHE MISS] Loading texture: " << path << std::endl;
    
    Texture* texture = new Texture();
    
    // Load image using stb_image
    texture->data = stbi_load(
        path.c_str(),
        &texture->width,
        &texture->height,
        &texture->channels,
        0  // desired_channels (0 = keep original format)
    );
    
    // Check if loading failed
    if (!texture->data) {
        // Get error message from stb_image
        const char* error = stbi_failure_reason();
        
        // Clean up and throw exception
        delete texture;
        throw std::runtime_error("Failed to load texture: " + path + " (" + error + ")");
    }
    
    // Add to cache
    cache[path] = texture;
    std::cout << "[CACHED] Texture added to cache: " << path << std::endl;
    
    return texture;
}