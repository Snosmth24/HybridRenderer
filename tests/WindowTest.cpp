#include <gtest/gtest.h>
#include "../src/platform/GLFWWindow.h"

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