#include "AssetLoader.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb_image.h"

Texture::~Texture() {
    std::cout << "[DESTRUCTOR] Called" << std::endl;
    if (data) {
        std::cout << "[DESTRUCTOR] Freeing data at: " << (void*)data << std::endl;
        stbi_image_free(data);
        data = nullptr;
        std::cout << "[DESTRUCTOR] Data freed" << std::endl;
    }
    else {
        std::cout << "[DESTRUCTOR] data is nullptr, nothing to free" << std::endl;
    }
    std::cout << "[DESTRUCTOR] Done" << std::endl;
}

Texture* AssetLoader::loadTexture(const std::string& path) {
    std::cout << "[LOAD] Starting loadTexture()" << std::endl;
    std::cout << "[LOAD] Path: " << path << std::endl;

    std::cout << "[LOAD] About to create Texture..." << std::endl;
    Texture* texture = new Texture();
    std::cout << "[LOAD] Texture created at: " << (void*)texture << std::endl;

    std::cout << "[LOAD] Calling stbi_load()..." << std::endl;
    texture->data = stbi_load(
        path.c_str(),
        &texture->width,
        &texture->height,
        &texture->channels,
        0
    );
    std::cout << "[LOAD] stbi_load() returned" << std::endl;
    std::cout << "[LOAD] data pointer: " << (void*)texture->data << std::endl;

    if (!texture->data) {
        std::cout << "[LOAD] Loading failed!" << std::endl;
        const char* error = stbi_failure_reason();
        std::cout << "[LOAD] Error: " << error << std::endl;

        std::cout << "[LOAD] About to delete texture..." << std::endl;
        delete texture;
        std::cout << "[LOAD] Texture deleted" << std::endl;

        std::cout << "[LOAD] About to throw exception..." << std::endl;
        throw std::runtime_error("Failed to load texture: " + path + " (" + error + ")");
    }

    std::cout << "[LOAD] Success! " << texture->width << "x" << texture->height << std::endl;
    return texture;
}