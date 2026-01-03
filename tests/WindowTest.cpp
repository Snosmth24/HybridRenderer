#include <gtest/gtest.h>
#include "../src/platform/GLFWWindow.h"
#include "../src/utils/Logger.h"

// Test fixture with setup/teardown
class WindowTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Silence output during tests
        Logger::setLevel(LogLevel::Silent);
    }
    
    void TearDown() override {
        // Restore normal logging
        Logger::setLevel(LogLevel::Normal);
    }
};

TEST(WindowTest, CreatesSuccessfully) {
    // Test that window can be created without crashing
    EXPECT_NO_THROW({
        GLFWWindow window(800, 600, "Test Window");
    });
}

TEST(WindowTest, HasCorrectDimensions) {
    GLFWWindow window(1920, 1080, "Test Window");
    
    EXPECT_EQ(window.getWidth(), 1920);
    EXPECT_EQ(window.getHeight(), 1080);
}

TEST(WindowTest, StartsNotClosed) {
    GLFWWindow window(800, 600, "Test Window");
    
    EXPECT_FALSE(window.shouldClose());
}

TEST(WindowTest, InputQueryDoesntCrash) {
    GLFWWindow window(800, 600, "Test Window");
    
    // Should not crash even if key not pressed
    EXPECT_NO_THROW({
        bool pressed = window.isKeyPressed(Key::W);
        (void)pressed; // Suppress unused warning
    });
}