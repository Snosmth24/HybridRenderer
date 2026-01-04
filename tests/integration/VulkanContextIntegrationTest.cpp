#include <gtest/gtest.h>
#include "../../src/platform/GLFWWindow.h"
#include "../../src/vulkan/VulkanContext.h"
#include "../../src/utils/Logger.h"

/**
 * Integration tests for VulkanContext
 * 
 * These tests require:
 * - Vulkan SDK installed
 * - GPU with Vulkan drivers
 * - Display/monitor
 * 
 * These tests are VERY SLOW because they initialize real GPU.
 * Run unit tests first for fast feedback.
 */
class VulkanContextIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Silence most output, but keep errors
        Logger::setLevel(LogLevel::Minimal);
        
        window = new GLFWWindow(800, 600, "Integration Test");
        context = new VulkanContext(window);
    }
    
    void TearDown() override {
        delete context;
        delete window;
        
        Logger::setLevel(LogLevel::Normal);
    }
    
    GLFWWindow* window;
    VulkanContext* context;
};

TEST_F(VulkanContextIntegrationTest, InitializesWithRealGPU) {
    EXPECT_NO_THROW({
        context->initialize();
    });
}

TEST_F(VulkanContextIntegrationTest, CanRenderFrameOnRealGPU) {
    context->initialize();
    
    EXPECT_NO_THROW({
        context->beginFrame();
        context->endFrame();
    });
}

TEST_F(VulkanContextIntegrationTest, CanRenderMultipleFrames) {
    context->initialize();
    
    for (int i = 0; i < 10; i++) {
        EXPECT_NO_THROW({
            context->beginFrame();
            context->endFrame();
        }) << "Failed on frame " << i;
    }
}

TEST_F(VulkanContextIntegrationTest, CleanupDoesntCrash) {
    context->initialize();
    context->beginFrame();
    context->endFrame();
    
    EXPECT_NO_THROW({
        context->cleanup();
    });
}

TEST_F(VulkanContextIntegrationTest, DoubleCleanupIsSafe) {
    context->initialize();
    
    context->cleanup();
    
    // Second cleanup should be safe
    EXPECT_NO_THROW({
        context->cleanup();
    });
}

TEST_F(VulkanContextIntegrationTest, WaitIdleWorks) {
    context->initialize();
    
    // Render some frames
    for (int i = 0; i < 5; i++) {
        context->beginFrame();
        context->endFrame();
    }
    
    // Wait for GPU to finish
    EXPECT_NO_THROW({
        context->waitIdle();
    });
}

TEST_F(VulkanContextIntegrationTest, ValidationLayersWork) {
    // This test verifies validation layers are working in debug builds
    #ifndef NDEBUG
        context->initialize();
        
        // If validation is working, we should be able to render without errors
        EXPECT_NO_THROW({
            for (int i = 0; i < 3; i++) {
                context->beginFrame();
                context->endFrame();
            }
        });
    #else
        GTEST_SKIP() << "Validation layers only in Debug builds";
    #endif
}

TEST_F(VulkanContextIntegrationTest, StressTest) {
    context->initialize();
    
    // Render many frames to stress test
    const int frameCount = 100;
    
    for (int i = 0; i < frameCount; i++) {
        EXPECT_NO_THROW({
            context->beginFrame();
            context->endFrame();
        }) << "Failed on frame " << i << " of " << frameCount;
    }
}

TEST_F(VulkanContextIntegrationTest, SequentialInitAndCleanup) {
    // Test multiple init/cleanup cycles
    for (int i = 0; i < 3; i++) {
        EXPECT_NO_THROW({
            context->initialize();
            context->beginFrame();
            context->endFrame();
            context->cleanup();
        }) << "Failed on cycle " << i;
        
        // Reset context for next iteration
        delete context;
        context = new VulkanContext(window);
    }
}