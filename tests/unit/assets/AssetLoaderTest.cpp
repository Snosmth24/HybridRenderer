#include <gtest/gtest.h>
#include "../../../src/assets/AssetLoader.h"

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

TEST_F(AssetLoaderTest, ShowWorkingDirectory) {
    std::cout << "Working directory: "
        << std::filesystem::current_path()
        << std::endl;

    std::cout << "Executable location: "
        << std::filesystem::current_path() / "HybridRenderer_unit_tests.exe"
        << std::endl;
}
// ==================== Cycle 3 ====================
// This test should ALWAYS pass - it doesn't load any files
TEST_F(AssetLoaderTest, LoaderCanBeCreated) {
    EXPECT_NE(loader, nullptr);
}


TEST_F(AssetLoaderTest, HasLoadTextureMethod) {
    // Print current working directory
    std::cout << "Working directory: " << std::filesystem::current_path() << std::endl;

    // Check if file exists
    std::string path = "test_assets/test_256x256.png";
    bool exists = std::filesystem::exists(path);
    std::cout << "File exists: " << (exists ? "YES" : "NO") << std::endl;

    if (!exists) {
        std::cout << "Looking for: " << std::filesystem::absolute(path) << std::endl;
    }

    Texture* texture = loader->loadTexture(path);
    EXPECT_NE(texture, nullptr);
    delete texture;
}

TEST_F(AssetLoaderTest, LoadsCorrectDimensions) {
    std::string path = "test_assets/test_256x256.png";
    Texture* texture = loader->loadTexture(path);
    ASSERT_NE(texture, nullptr);
    EXPECT_EQ(texture->width, 256);
    EXPECT_EQ(texture->height, 256);
    delete texture;
}

// ==================== Cycle 4 ====================

TEST_F(AssetLoaderTest, ThrowsExceptionForMissingFile) {
    // ARRANGE
    std::string invalidPath = "nonexistent_file.png";
    
    // ACT & ASSERT
    try {
        loader->loadTexture(invalidPath);
        FAIL() << "Expected exception to be thrown";
    } catch (const std::runtime_error& e) {
        // Verify error message contains the filename
        std::string errorMsg = e.what();
        EXPECT_TRUE(errorMsg.find("nonexistent_file.png") != std::string::npos)
            << "Error message should contain filename. Got: " << errorMsg;
    }
}
