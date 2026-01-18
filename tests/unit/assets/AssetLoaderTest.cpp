#include <gtest/gtest.h>
#include "../../../src/assets/AssetLoader.h"

class AssetLoaderTest : public ::testing::Test {
protected:
    AssetLoader* loader = nullptr;
    
    void SetUp() override {
        loader = new AssetLoader();
    }
    
    void TearDown() override {
        if (loader) {
            delete loader;  // This will call clearCache() automatically
            loader = nullptr;
        }
    }
};

// ==================== Cycle 1 ====================

TEST_F(AssetLoaderTest, LoaderCanBeCreated) {
    EXPECT_NE(loader, nullptr);
}

// ==================== Cycle 2 ====================

TEST_F(AssetLoaderTest, HasLoadTextureMethod) {
    std::string path = "test_assets/test_256x256.png";
    Texture* texture = loader->loadTexture(path);
    EXPECT_NE(texture, nullptr);
    // Note: Don't delete - loader owns it
}

// ==================== Cycle 3 ====================

TEST_F(AssetLoaderTest, LoadsCorrectDimensions) {
    std::string path = "test_assets/test_256x256.png";
    Texture* texture = loader->loadTexture(path);
    ASSERT_NE(texture, nullptr);
    EXPECT_EQ(texture->width, 256);
    EXPECT_EQ(texture->height, 256);
    // Note: Don't delete - loader owns it
}

// ==================== Cycle 4 ====================

TEST_F(AssetLoaderTest, ThrowsExceptionForMissingFile) {
    std::string invalidPath = "nonexistent_file.png";
    EXPECT_THROW(
        loader->loadTexture(invalidPath),
        std::runtime_error
    );
}

// ==================== Cycle 7 ====================

TEST_F(AssetLoaderTest, CachesLoadedTextures) {
    std::string path = "test_assets/test_256x256.png";
    
    Texture* tex1 = loader->loadTexture(path);
    Texture* tex2 = loader->loadTexture(path);
    
    // Should return same pointer (cached)
    EXPECT_EQ(tex1, tex2);
    EXPECT_EQ(loader->getCacheSize(), 1);
}

TEST_F(AssetLoaderTest, CachesDifferentTextures) {
    std::string path1 = "test_assets/test_256x256.png";
    std::string path2 = "test_assets/test_512x512.png";
    
    Texture* tex1 = loader->loadTexture(path1);
    Texture* tex2 = loader->loadTexture(path2);
    
    // Different files = different pointers
    EXPECT_NE(tex1, tex2);
    EXPECT_EQ(loader->getCacheSize(), 2);
}

TEST_F(AssetLoaderTest, ClearCacheRemovesAllTextures) {
    loader->loadTexture("test_assets/test_256x256.png");
    EXPECT_EQ(loader->getCacheSize(), 1);
    
    loader->clearCache();
    EXPECT_EQ(loader->getCacheSize(), 0);
    
    // Load again - should create new entry
    Texture* tex = loader->loadTexture("test_assets/test_256x256.png");
    EXPECT_NE(tex, nullptr);
    EXPECT_EQ(loader->getCacheSize(), 1);
}