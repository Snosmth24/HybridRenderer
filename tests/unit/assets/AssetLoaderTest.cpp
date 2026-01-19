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

// ==================== Cycle 8 ====================

TEST_F(AssetLoaderTest, CacheReducesLoadTime) {
    // NOTE: This test verifies cache improves performance,
    // but OS-level file caching affects exact speedup.
    // We test that cache IS faster, not HOW MUCH faster.
    
    std::string path = "test_assets/test_256x256.png";
    
    // Warm up OS cache to stabilize measurements
    Texture* warmup = loader->loadTexture(path);
    loader->clearCache();  // Clear app cache, but OS cache remains warm
    
    // Measure first load (cache miss, but OS cache warm)
    auto start1 = std::chrono::high_resolution_clock::now();
    Texture* tex1 = loader->loadTexture(path);
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    
    // Measure second load (cache hit)
    auto start2 = std::chrono::high_resolution_clock::now();
    Texture* tex2 = loader->loadTexture(path);
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    
    // Test: Cache hit is faster than cache miss
    EXPECT_LT(duration2.count(), duration1.count()) 
        << "Cache hit (" << duration2.count() << "µs) should be faster than "
        << "cache miss (" << duration1.count() << "µs)";
    
    // Informational output (not asserted)
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
    // Test that loading the same texture multiple times
    // doesn't cause performance degradation
    
    std::string path = "test_assets/test_256x256.png";
    
    // First load
    auto start1 = std::chrono::high_resolution_clock::now();
    Texture* tex1 = loader->loadTexture(path);
    auto end1 = std::chrono::high_resolution_clock::now();
    auto first_load = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
    
    // Load same texture 100 times
    std::vector<long long> subsequent_loads;
    for (int i = 0; i < 100; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        Texture* tex = loader->loadTexture(path);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        subsequent_loads.push_back(duration.count());
        
        // Should return same pointer
        EXPECT_EQ(tex, tex1);
    }
    
    // Calculate average time for cached loads
    long long sum = 0;
    for (auto duration : subsequent_loads) {
        sum += duration;
    }
    double avg_cached = sum / 100.0;
    
    // Cached loads should be faster than first load
    EXPECT_LT(avg_cached, first_load.count())
        << "Average cached load (" << avg_cached << "µs) should be faster than "
        << "first load (" << first_load.count() << "µs)";
    
    // Cache should still only have 1 entry
    EXPECT_EQ(loader->getCacheSize(), 1);
    
    std::cout << "\n=== Multiple Load Performance ===" << std::endl;
    std::cout << "First load:         " << first_load.count() << "µs" << std::endl;
    std::cout << "Average cached load: " << avg_cached << "µs" << std::endl;
    std::cout << "Total loads:        100" << std::endl;
    std::cout << "Cache entries:      " << loader->getCacheSize() << std::endl;
}

TEST_F(AssetLoaderTest, CacheSizeRemainsConstant) {
    // Verify that loading the same texture repeatedly
    // doesn't increase cache size (no duplicates)
    
    std::string path = "test_assets/test_256x256.png";
    
    // Load same texture 50 times
    for (int i = 0; i < 50; i++) {
        loader->loadTexture(path);
    }
    
    // Cache should have exactly 1 entry (not 50)
    EXPECT_EQ(loader->getCacheSize(), 1);
}

TEST_F(AssetLoaderTest, DifferentPathsCreateSeparateCacheEntries) {
    // Verify that different file paths are cached separately
    
    std::string path1 = "test_assets/test_256x256.png";
    std::string path2 = "test_assets/test_512x512.png";
    
    // Load both textures
    Texture* tex1 = loader->loadTexture(path1);
    Texture* tex2 = loader->loadTexture(path2);
    
    // Should be different pointers
    EXPECT_NE(tex1, tex2);
    
    // Cache should have 2 entries
    EXPECT_EQ(loader->getCacheSize(), 2);
    
    // Loading again should return same pointers
    Texture* tex1_again = loader->loadTexture(path1);
    Texture* tex2_again = loader->loadTexture(path2);
    
    EXPECT_EQ(tex1, tex1_again);
    EXPECT_EQ(tex2, tex2_again);
    
    // Cache size should still be 2
    EXPECT_EQ(loader->getCacheSize(), 2);
}