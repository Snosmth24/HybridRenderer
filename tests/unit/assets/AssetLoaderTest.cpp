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

// RED: This test will fail because AssetLoader doesn't exist yet
TEST_F(AssetLoaderTest, LoaderCanBeCreated) {
    // ARRANGE & ACT
    // (Done in SetUp)
    
    // ASSERT
    EXPECT_NE(loader, nullptr);
}