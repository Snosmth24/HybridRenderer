#include <gtest/gtest.h>
#include "../../../src/assets/AssetLoader.h"

class AssetLoaderTest : public ::testing::Test {
protected:
    AssetLoader* loader;
    
    void SetUp() override {
        loader = new AssetLoader();
    }
    
    void TearDown() override {
        delete loader;
    }
};

TEST_F(AssetLoaderTest, LoaderCanBeCreated) {
    // ARRANGE & ACT
    // (Done in SetUp)
    
    // ASSERT
    EXPECT_NE(loader, nullptr);
}

TEST_F(AssetLoaderTest, HasLoadTextureMethod) {
    // ARRANGE
    std::string path = "test.png";
    
    // ACT
    Texture* texture = loader->loadTexture(path);
    
    // ASSERT
    // Just test that it returns something (we'll test contents later)
    EXPECT_NE(texture, nullptr);
    
    // Cleanup
    delete texture;
}