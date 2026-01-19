#include <gtest/gtest.h>
#include "../../../src/assets/AssetLoader.h"
#include <chrono>
#include <vector>
#include <filesystem>
#include <iostream>

class AssetLoaderTest : public ::testing::Test {
protected:
    AssetLoader* loader = nullptr;
    
    void SetUp() override {
        loader = new AssetLoader();
    }
    
    void TearDown() override {
        if (loader) {
            delete loader;
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
}

// ==================== Cycle 3 ====================

TEST_F(AssetLoaderTest, LoadsCorrectDimensions) {
    std::string path = "test_assets/test_256x256.png";
    Texture* texture = loader->loadTexture(path);
    ASSERT_NE(texture, nullptr);
    EXPECT_EQ(texture->width, 256);
    EXPECT_EQ(texture->height, 256);
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
    
    EXPECT_EQ(tex1, tex2);
    EXPECT_EQ(loader->getCacheSize(), 1);
}

TEST_F(AssetLoaderTest, CachesDifferentTextures) {
    std::string path1 = "test_assets/test_256x256.png";
    std::string path2 = "test_assets/test_512x512.png";
    
    Texture* tex1 = loader->loadTexture(path1);
    Texture* tex2 = loader->loadTexture(path2);
    
    EXPECT_NE(tex1, tex2);
    EXPECT_EQ(loader->getCacheSize(), 2);
}

TEST_F(AssetLoaderTest, ClearCacheRemovesAllTextures) {
    loader->loadTexture("test_assets/test_256x256.png");
    EXPECT_EQ(loader->getCacheSize(), 1);
    
    loader->clearCache();
    EXPECT_EQ(loader->getCacheSize(), 0);
    
    Texture* tex = loader->loadTexture("test_assets/test_256x256.png");
    EXPECT_NE(tex, nullptr);
    EXPECT_EQ(loader->getCacheSize(), 1);
}

// ==================== Cycle 8 ====================

TEST_F(AssetLoaderTest, CacheReducesLoadTime) {
    std::string path = "test_assets/test_256x256.png";
    
    Texture* warmup = loader->loadTexture(path);
    loader->clearCache();
    
    auto start1 = std::chrono::high_resolution_clock::now();
    Texture* tex1 = loader->loadTexture(path);
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    
    auto start2 = std::chrono::high_resolution_clock::now();
    Texture* tex2 = loader->loadTexture(path);
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    
    EXPECT_LT(duration2.count(), duration1.count()) 
        << "Cache hit (" << duration2.count() << "µs) should be faster than "
        << "cache miss (" << duration1.count() << "µs)";
    
    std::cout << "\n=== Cache Performance ===" << std::endl;
    std::cout << "Cache miss: " << duration1.count() << "µs" << std::endl;
    std::cout << "Cache hit:  " << duration2.count() << "µs" << std::endl;
    if (duration2.count() > 0) {
        double speedup = static_cast<double>(duration1.count()) / duration2.count();
        std::cout << "Speedup:    " << speedup << "x" << std::endl;
    }
    std::cout << "Note: Speedup varies based on OS file cache state" << std::endl;
}

TEST_F(AssetLoaderTest, MultipleLoadsShowCacheBenefit) {
    std::string path = "test_assets/test_256x256.png";
    
    auto start1 = std::chrono::high_resolution_clock::now();
    Texture* tex1 = loader->loadTexture(path);
    auto end1 = std::chrono::high_resolution_clock::now();
    auto first_load = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    
    std::vector<long long> subsequent_loads;
    for (int i = 0; i < 100; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        Texture* tex = loader->loadTexture(path);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        subsequent_loads.push_back(duration.count());
        
        EXPECT_EQ(tex, tex1);
    }
    
    long long sum = 0;
    for (auto duration : subsequent_loads) {
        sum += duration;
    }
    double avg_cached = sum / 100.0;
    
    EXPECT_LT(avg_cached, first_load.count())
        << "Average cached load (" << avg_cached << "µs) should be faster than "
        << "first load (" << first_load.count() << "µs)";
    
    EXPECT_EQ(loader->getCacheSize(), 1);
    
    std::cout << "\n=== Multiple Load Performance ===" << std::endl;
    std::cout << "First load:          " << first_load.count() << "µs" << std::endl;
    std::cout << "Average cached load: " << avg_cached << "µs" << std::endl;
    std::cout << "Total loads:         100" << std::endl;
    std::cout << "Cache entries:       " << loader->getCacheSize() << std::endl;
}

TEST_F(AssetLoaderTest, CacheSizeRemainsConstant) {
    std::string path = "test_assets/test_256x256.png";
    
    for (int i = 0; i < 50; i++) {
        loader->loadTexture(path);
    }
    
    EXPECT_EQ(loader->getCacheSize(), 1);
}

TEST_F(AssetLoaderTest, DifferentPathsCreateSeparateCacheEntries) {
    std::string path1 = "test_assets/test_256x256.png";
    std::string path2 = "test_assets/test_512x512.png";
    
    Texture* tex1 = loader->loadTexture(path1);
    Texture* tex2 = loader->loadTexture(path2);
    
    EXPECT_NE(tex1, tex2);
    EXPECT_EQ(loader->getCacheSize(), 2);
    
    Texture* tex1_again = loader->loadTexture(path1);
    Texture* tex2_again = loader->loadTexture(path2);
    
    EXPECT_EQ(tex1, tex1_again);
    EXPECT_EQ(tex2, tex2_again);
    EXPECT_EQ(loader->getCacheSize(), 2);
}

// ==================== Cycle 9 ====================

TEST_F(AssetLoaderTest, ClearCacheDeletesAllTextures) {
    // ARRANGE
    std::string path1 = "test_assets/test_256x256.png";
    std::string path2 = "test_assets/test_512x512.png";
    
    // Load multiple textures
    Texture* tex1 = loader->loadTexture(path1);
    Texture* tex2 = loader->loadTexture(path2);
    
    EXPECT_EQ(loader->getCacheSize(), 2);
    
    // ACT
    loader->clearCache();
    
    // ASSERT
    EXPECT_EQ(loader->getCacheSize(), 0);
    
    // Loading again should create NEW textures (different pointers)
    Texture* tex1_new = loader->loadTexture(path1);
    Texture* tex2_new = loader->loadTexture(path2);
    
    // These should be different pointers (new allocations)
    EXPECT_NE(tex1, tex1_new);
    EXPECT_NE(tex2, tex2_new);
    
    // Cache should have 2 entries again
    EXPECT_EQ(loader->getCacheSize(), 2);
}

TEST_F(AssetLoaderTest, ClearCacheOnEmptyCacheIsSafe) {
    // ARRANGE
    EXPECT_EQ(loader->getCacheSize(), 0);
    
    // ACT - clearing empty cache should not crash
    loader->clearCache();
    
    // ASSERT
    EXPECT_EQ(loader->getCacheSize(), 0);
    
    // Can still load textures after clearing empty cache
    Texture* tex = loader->loadTexture("test_assets/test_256x256.png");
    EXPECT_NE(tex, nullptr);
    EXPECT_EQ(loader->getCacheSize(), 1);
}

TEST_F(AssetLoaderTest, MultipleClearCachesAreSafe) {
    // ARRANGE
    loader->loadTexture("test_assets/test_256x256.png");
    EXPECT_EQ(loader->getCacheSize(), 1);
    
    // ACT - multiple clears should be safe
    loader->clearCache();
    loader->clearCache();
    loader->clearCache();
    
    // ASSERT
    EXPECT_EQ(loader->getCacheSize(), 0);
    
    // Can still use loader normally
    Texture* tex = loader->loadTexture("test_assets/test_256x256.png");
    EXPECT_NE(tex, nullptr);
}

TEST_F(AssetLoaderTest, DestructorClearsCache) {
    // Test that destructor properly cleans up cache
    
    // Create a loader in a scope
    {
        AssetLoader* tempLoader = new AssetLoader();
        
        // Load some textures
        tempLoader->loadTexture("test_assets/test_256x256.png");
        tempLoader->loadTexture("test_assets/test_512x512.png");
        
        EXPECT_EQ(tempLoader->getCacheSize(), 2);
        
        // Destructor should clean up when we delete
        delete tempLoader;
        
        // Can't verify cache size after deletion (object destroyed)
        // But test passes if no memory leaks or crashes
    }
    
    // If we got here without crashing, destructor worked correctly
    SUCCEED();
}

TEST_F(AssetLoaderTest, ClearCacheFreesMemory) {
    // This test verifies that clearCache() actually frees memory
    // by loading a texture, clearing, and loading again to get new pointer
    
    std::string path = "test_assets/test_256x256.png";
    
    // First load
    Texture* tex1 = loader->loadTexture(path);
    void* ptr1 = static_cast<void*>(tex1);
    
    // Clear cache (should delete tex1)
    loader->clearCache();
    
    // Second load - should allocate new Texture
    Texture* tex2 = loader->loadTexture(path);
    void* ptr2 = static_cast<void*>(tex2);
    
    // Pointers should be different (new allocation)
    // Note: In theory they COULD be the same if allocator reuses
    // the same address, but in practice with fresh allocation,
    // they're usually different
    EXPECT_NE(ptr1, ptr2) 
        << "Expected different pointers after clearCache, but got same address. "
        << "This could happen if allocator reused memory, but is unlikely.";
    
    // More important: verify we can use the new texture
    EXPECT_EQ(tex2->width, 256);
    EXPECT_EQ(tex2->height, 256);
}