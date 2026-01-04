#include <gtest/gtest.h>
#include "../../src/platform/GLFWWindow.h"
#include "../../src/utils/Logger.h"

/**
 * Integration tests for GLFWWindow
 * 
 * These tests require:
 * - GLFW library
 * - Window system (Windows, X11, Wayland)
 * - Display/monitor
 * 
 * These tests are SLOW because they create real windows.
 * Run unit tests first for fast feedback.
 */
class WindowIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Keep logging for integration tests (helpful for debugging)
        Logger::setLevel(LogLevel::Minimal);
    }
    
    void TearDown() override {
        Logger::setLevel(LogLevel::Normal);
    }
};

TEST_F(WindowIntegrationTest, CreatesRealWindow) {
    EXPECT_NO_THROW({
        GLFWWindow window(800, 600, "Integration Test Window");
        
        // Window should exist
        EXPECT_NE(window.getNativeHandle(), nullptr);
    });
}

TEST_F(WindowIntegrationTest, HasCorrectDimensions) {
    GLFWWindow window(1920, 1080, "Test Window");
    
    EXPECT_EQ(window.getWidth(), 1920);
    EXPECT_EQ(window.getHeight(), 1080);
}

TEST_F(WindowIntegrationTest, StartsNotClosed) {
    GLFWWindow window(800, 600, "Test Window");
    
    EXPECT_FALSE(window.shouldClose());
}

TEST_F(WindowIntegrationTest, InputQueryDoesntCrash) {
    GLFWWindow window(800, 600, "Test Window");
    
    // Should not crash even if key not pressed
    EXPECT_NO_THROW({
        bool pressed = window.isKeyPressed(Key::W);
        (void)pressed; // Suppress unused warning
    });
}

TEST_F(WindowIntegrationTest, PollEventsWorks) {
    GLFWWindow window(800, 600, "Test Window");
    
    // Should not crash
    EXPECT_NO_THROW({
        window.pollEvents();
        window.pollEvents();
        window.pollEvents();
    });
}

TEST_F(WindowIntegrationTest, MultipleWindowsCanExist) {
    GLFWWindow window1(800, 600, "Window 1");
    GLFWWindow window2(1024, 768, "Window 2");
    
    EXPECT_NE(window1.getNativeHandle(), nullptr);
    EXPECT_NE(window2.getNativeHandle(), nullptr);
    EXPECT_NE(window1.getNativeHandle(), window2.getNativeHandle());
}

TEST_F(WindowIntegrationTest, SequentialWindowCreation) {
    // Create and destroy multiple windows in sequence
    for (int i = 0; i < 5; i++) {
        EXPECT_NO_THROW({
            GLFWWindow window(640 + i * 100, 480 + i * 100, "Test");
        }) << "Failed on iteration " << i;
    }
}

TEST_F(WindowIntegrationTest, DifferentWindowSizes) {
    std::vector<std::pair<int, int>> sizes = {
        {640, 480},
        {800, 600},
        {1024, 768},
        {1280, 720},
        {1920, 1080}
    };
    
    for (const auto& [width, height] : sizes) {
        EXPECT_NO_THROW({
            GLFWWindow window(width, height, "Test");
            EXPECT_EQ(window.getWidth(), width);
            EXPECT_EQ(window.getHeight(), height);
        }) << "Failed with size " << width << "x" << height;
    }
}