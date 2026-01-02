#include <gtest/gtest.h>
#include "../src/platform/GLFWWindow.h"
#include "../src/vulkan/VulkanContext.h"

class VulkanContextTest : public ::testing::Test {
protected:
    void SetUp() override {
        window = new GLFWWindow(800, 600, "Test");
        context = new VulkanContext(window);
    }
    
    void TearDown() override {
        delete context;  // Destructor calls cleanup()
        delete window;
    }
    
    GLFWWindow* window;
    VulkanContext* context;
};

TEST_F(VulkanContextTest, InitializesSuccessfully) {
    EXPECT_NO_THROW({
        context->initialize();
    });
}

TEST_F(VulkanContextTest, CanRenderFrame) {
    context->initialize();
    
    EXPECT_NO_THROW({
        context->beginFrame();
        context->endFrame();
    });
}

TEST_F(VulkanContextTest, CanRenderMultipleFrames) {
    context->initialize();
    
    for (int i = 0; i < 10; i++) {
        EXPECT_NO_THROW({
            context->beginFrame();
            context->endFrame();
        }) << "Failed on frame " << i;
    }
}

TEST_F(VulkanContextTest, CleanupDoesntCrashAfterInit) {
    context->initialize();
    
    // Explicit cleanup for testing
    EXPECT_NO_THROW({
        context->cleanup();
    });
    
    // Mark that we already cleaned up so destructor doesn't try again
    // (or just let destructor handle the second cleanup safely)
}

TEST_F(VulkanContextTest, DoubleCleanupDoesntCrash) {
    context->initialize();
    
    context->cleanup();
    
    // Second cleanup should be safe (no-op)
    EXPECT_NO_THROW({
        context->cleanup();
    });
}