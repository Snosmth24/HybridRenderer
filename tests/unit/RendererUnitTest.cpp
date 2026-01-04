#include <gtest/gtest.h>
#include "../../src/renderer/Renderer.h"
#include "../mocks/MockWindow.h"
#include "../mocks/MockGraphicsContext.h"
#include "../../src/utils/Logger.h"

/**
 * Test fixture for Renderer unit tests
 * Uses mock objects instead of real GPU
 */
class RendererUnitTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Silence output during tests
        Logger::setLevel(LogLevel::Silent);
        
        // Create mock objects
        window = new MockWindow(1920, 1080, "Test Window");
        context = new MockGraphicsContext();
        renderer = new Renderer(window, context);
    }
    
    void TearDown() override {
        // Cleanup
        delete renderer;
        delete context;
        delete window;
        
        // Restore logging
        Logger::setLevel(LogLevel::Normal);
    }
    
    // Test objects (accessible to all tests)
    MockWindow* window;
    MockGraphicsContext* context;
    Renderer* renderer;
};

// ==================== Basic Lifecycle Tests ====================

TEST_F(RendererUnitTest, CreatesSuccessfully) {
    // Just creating the renderer should not crash
    // Constructor should not initialize yet
    EXPECT_FALSE(context->isInitialized());
}

TEST_F(RendererUnitTest, InitializesSuccessfully) {
    EXPECT_NO_THROW({
        renderer->initialize();
    });
    
    // Should initialize the graphics context
    EXPECT_TRUE(context->isInitialized());
}

TEST_F(RendererUnitTest, CannotInitializeTwice) {
    renderer->initialize();
    
    // Second initialization should fail
    EXPECT_THROW({
        renderer->initialize();
    }, std::runtime_error);
}

TEST_F(RendererUnitTest, CleansUpSuccessfully) {
    renderer->initialize();
    
    EXPECT_NO_THROW({
        renderer->cleanup();
    });
    
    EXPECT_TRUE(context->isCleanedUp());
}

TEST_F(RendererUnitTest, CleanupIsIdempotent) {
    renderer->initialize();
    
    // Multiple cleanups should be safe
    EXPECT_NO_THROW({
        renderer->cleanup();
        renderer->cleanup();
        renderer->cleanup();
    });
}

TEST_F(RendererUnitTest, CleanupWithoutInitializeIsSafe) {
    // Should not crash if cleanup called before initialize
    EXPECT_NO_THROW({
        renderer->cleanup();
    });
}

// ==================== Frame Rendering Tests ====================

TEST_F(RendererUnitTest, CannotRenderBeforeInitialize) {
    // Attempting to render without initialization should fail
    EXPECT_THROW({
        renderer->renderFrame();
    }, std::runtime_error);
}

TEST_F(RendererUnitTest, CanRenderSingleFrame) {
    renderer->initialize();
    
    EXPECT_NO_THROW({
        renderer->renderFrame();
    });
    
    // Should have called beginFrame and endFrame
    EXPECT_EQ(context->getFramesBegun(), 1);
    EXPECT_EQ(context->getFramesEnded(), 1);
}

TEST_F(RendererUnitTest, CanRenderMultipleFrames) {
    renderer->initialize();
    
    const int frameCount = 100;
    
    for (int i = 0; i < frameCount; i++) {
        EXPECT_NO_THROW({
            renderer->renderFrame();
        }) << "Failed on frame " << i;
    }
    
    EXPECT_EQ(context->getFramesBegun(), frameCount);
    EXPECT_EQ(context->getFramesEnded(), frameCount);
    EXPECT_TRUE(context->allFramesCompleted());
}

TEST_F(RendererUnitTest, CannotRenderAfterCleanup) {
    renderer->initialize();
    renderer->renderFrame();
    renderer->cleanup();
    
    // Rendering after cleanup should fail
    EXPECT_THROW({
        renderer->renderFrame();
    }, std::runtime_error);
}

// ==================== Window Interaction Tests ====================

TEST_F(RendererUnitTest, UsesCorrectWindowDimensions) {
    renderer->initialize();
    
    // Renderer should use window dimensions
    EXPECT_EQ(window->getWidth(), 1920);
    EXPECT_EQ(window->getHeight(), 1080);
}

TEST_F(RendererUnitTest, HandlesWindowResize) {
    renderer->initialize();
    renderer->renderFrame();
    
    // Simulate window resize
    window->resize(1280, 720);
    
    // Should still be able to render
    EXPECT_NO_THROW({
        renderer->renderFrame();
    });
    
    EXPECT_EQ(window->getWidth(), 1280);
    EXPECT_EQ(window->getHeight(), 720);
}

TEST_F(RendererUnitTest, HandlesDifferentWindowSizes) {
    // Test various window sizes
    std::vector<std::pair<int, int>> sizes = {
        {640, 480},
        {800, 600},
        {1024, 768},
        {1280, 720},
        {1920, 1080},
        {2560, 1440},
        {3840, 2160}
    };
    
    for (const auto& [width, height] : sizes) {
        window->resize(width, height);
        
        EXPECT_NO_THROW({
            renderer->initialize();
            renderer->renderFrame();
            renderer->cleanup();
        }) << "Failed with size " << width << "x" << height;
        
        // Reset for next iteration
        delete renderer;
        delete context;
        context = new MockGraphicsContext();
        renderer = new Renderer(window, context);
    }
}

// ==================== Error Handling Tests ====================

TEST_F(RendererUnitTest, HandlesGraphicsContextInitializationFailure) {
    // Simulate graphics context initialization failure
    context->setFailOnInitialize(true);
    
    EXPECT_THROW({
        renderer->initialize();
    }, std::runtime_error);
    
    EXPECT_FALSE(context->isInitialized());
}

TEST_F(RendererUnitTest, HandlesBeginFrameFailure) {
    renderer->initialize();
    
    // Simulate begin frame failure (e.g., device lost)
    context->setFailOnBeginFrame(true);
    
    EXPECT_THROW({
        renderer->renderFrame();
    }, std::runtime_error);
}

TEST_F(RendererUnitTest, HandlesEndFrameFailure) {
    renderer->initialize();
    
    // Simulate end frame failure (e.g., swapchain out of date)
    context->setFailOnEndFrame(true);
    
    EXPECT_THROW({
        renderer->renderFrame();
    }, std::runtime_error);
}

// ==================== State Management Tests ====================

TEST_F(RendererUnitTest, MaintainsCorrectStateAfterInitialize) {
    EXPECT_FALSE(context->isInitialized());
    EXPECT_FALSE(context->isCleanedUp());
    
    renderer->initialize();
    
    EXPECT_TRUE(context->isInitialized());
    EXPECT_FALSE(context->isCleanedUp());
}

TEST_F(RendererUnitTest, MaintainsCorrectStateAfterCleanup) {
    renderer->initialize();
    
    EXPECT_TRUE(context->isInitialized());
    EXPECT_FALSE(context->isCleanedUp());
    
    renderer->cleanup();
    
    EXPECT_TRUE(context->isCleanedUp());
}

TEST_F(RendererUnitTest, FrameCountIncrementsCorrectly) {
    renderer->initialize();
    
    EXPECT_EQ(context->getFramesEnded(), 0);
    
    renderer->renderFrame();
    EXPECT_EQ(context->getFramesEnded(), 1);
    
    renderer->renderFrame();
    EXPECT_EQ(context->getFramesEnded(), 2);
    
    renderer->renderFrame();
    EXPECT_EQ(context->getFramesEnded(), 3);
}

// ==================== Performance Tests ====================

TEST_F(RendererUnitTest, RenderingIsFast) {
    renderer->initialize();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Render 1000 frames
    for (int i = 0; i < 1000; i++) {
        renderer->renderFrame();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Mock rendering should be very fast (< 100ms for 1000 frames)
    EXPECT_LT(duration.count(), 100) 
        << "1000 mock frames took " << duration.count() << "ms (too slow!)";
}

// ==================== Integration-Style Tests (But Still Mocked) ====================

TEST_F(RendererUnitTest, FullRenderingCycle) {
    // Test a complete rendering cycle
    
    // 1. Initialize
    EXPECT_NO_THROW({
        renderer->initialize();
    });
    EXPECT_TRUE(context->isInitialized());
    
    // 2. Render some frames
    for (int i = 0; i < 10; i++) {
        EXPECT_NO_THROW({
            renderer->renderFrame();
        });
    }
    EXPECT_EQ(context->getFramesEnded(), 10);
    
    // 3. Cleanup
    EXPECT_NO_THROW({
        renderer->cleanup();
    });
    EXPECT_TRUE(context->isCleanedUp());
}

TEST_F(RendererUnitTest, SimulateApplicationLifecycle) {
    // Simulate a full application lifecycle
    
    // Startup
    renderer->initialize();
    
    // Main loop (simulate 60 frames = 1 second at 60fps)
    for (int frame = 0; frame < 60; frame++) {
        window->pollEvents();
        renderer->renderFrame();
    }
    
    // Shutdown
    renderer->cleanup();
    
    // Verify
    EXPECT_EQ(context->getFramesEnded(), 60);
    EXPECT_TRUE(context->allFramesCompleted());
    EXPECT_TRUE(context->isCleanedUp());
}

TEST_F(RendererUnitTest, SimulateResizeDuringRendering) {
    renderer->initialize();
    
    // Render a few frames
    renderer->renderFrame();
    renderer->renderFrame();
    
    // User resizes window
    window->resize(1280, 720);
    
    // Continue rendering
    renderer->renderFrame();
    renderer->renderFrame();
    
    // Should have rendered 4 frames total
    EXPECT_EQ(context->getFramesEnded(), 4);
}

// ==================== Edge Case Tests ====================

TEST_F(RendererUnitTest, HandlesZeroSizeWindow) {
    // Some systems might report 0x0 when minimized
    window->resize(0, 0);
    
    // Should handle gracefully (implementation dependent)
    // At minimum, shouldn't crash
    EXPECT_NO_THROW({
        renderer->initialize();
    });
}

TEST_F(RendererUnitTest, HandlesVeryLargeWindow) {
    // Test with 8K resolution
    window->resize(7680, 4320);
    
    EXPECT_NO_THROW({
        renderer->initialize();
        renderer->renderFrame();
        renderer->cleanup();
    });
}

TEST_F(RendererUnitTest, HandlesMultipleResizesDuringRendering) {
    renderer->initialize();
    
    // Simulate rapid resizing (like user dragging window edge)
    std::vector<std::pair<int, int>> sizes = {
        {1920, 1080},
        {1850, 1000},
        {1700, 950},
        {1600, 900},
        {1400, 800},
        {1280, 720}
    };
    
    for (const auto& [width, height] : sizes) {
        window->resize(width, height);
        
        EXPECT_NO_THROW({
            renderer->renderFrame();
        }) << "Failed after resize to " << width << "x" << height;
    }
}