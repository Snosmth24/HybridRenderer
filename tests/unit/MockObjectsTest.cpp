#include <gtest/gtest.h>
#include "../mocks/MockWindow.h"
#include "../mocks/MockGraphicsContext.h"

// ==================== MockWindow Tests ====================

TEST(MockWindowTest, CreatesWithCorrectDimensions) {
    MockWindow window(1920, 1080, "Test Window");
    
    EXPECT_EQ(window.getWidth(), 1920);
    EXPECT_EQ(window.getHeight(), 1080);
    EXPECT_EQ(window.getTitle(), "Test Window");
}

TEST(MockWindowTest, StartsNotClosed) {
    MockWindow window(800, 600, "Test");
    
    EXPECT_FALSE(window.shouldClose());
}

TEST(MockWindowTest, CanBeClosedProgrammatically) {
    MockWindow window(800, 600, "Test");
    
    window.setClosed(true);
    
    EXPECT_TRUE(window.shouldClose());
}

TEST(MockWindowTest, CanBeResized) {
    MockWindow window(800, 600, "Test");
    
    EXPECT_EQ(window.getWidth(), 800);
    EXPECT_EQ(window.getHeight(), 600);
    
    window.resize(1920, 1080);
    
    EXPECT_EQ(window.getWidth(), 1920);
    EXPECT_EQ(window.getHeight(), 1080);
}

TEST(MockWindowTest, KeyInputControlled) {
    MockWindow window(800, 600, "Test");
    
    // Initially no keys pressed
    EXPECT_FALSE(window.isKeyPressed(Key::W));
    EXPECT_FALSE(window.isKeyPressed(Key::A));
    
    // Programmatically press W
    window.setKeyPressed(Key::W, true);
    EXPECT_TRUE(window.isKeyPressed(Key::W));
    EXPECT_FALSE(window.isKeyPressed(Key::A));
    
    // Release W, press A
    window.setKeyPressed(Key::W, false);
    window.setKeyPressed(Key::A, true);
    EXPECT_FALSE(window.isKeyPressed(Key::W));
    EXPECT_TRUE(window.isKeyPressed(Key::A));
}

TEST(MockWindowTest, MouseInputControlled) {
    MockWindow window(800, 600, "Test");
    
    EXPECT_FALSE(window.isMouseButtonPressed(MouseButton::Left));
    
    window.setMouseButtonPressed(MouseButton::Left, true);
    EXPECT_TRUE(window.isMouseButtonPressed(MouseButton::Left));
    
    window.setMouseButtonPressed(MouseButton::Left, false);
    EXPECT_FALSE(window.isMouseButtonPressed(MouseButton::Left));
}

TEST(MockWindowTest, CursorPositionControlled) {
    MockWindow window(800, 600, "Test");
    
    double x, y;
    window.getCursorPosition(x, y);
    EXPECT_EQ(x, 0.0);
    EXPECT_EQ(y, 0.0);
    
    window.setCursorPosition(123.5, 456.7);
    window.getCursorPosition(x, y);
    EXPECT_EQ(x, 123.5);
    EXPECT_EQ(y, 456.7);
}

TEST(MockWindowTest, PollEventsDoesntCrash) {
    MockWindow window(800, 600, "Test");
    
    // Should be safe to call
    EXPECT_NO_THROW({
        window.pollEvents();
        window.pollEvents();
        window.pollEvents();
    });
}

TEST(MockWindowTest, NativeHandleIsNull) {
    MockWindow window(800, 600, "Test");
    
    // Mock window has no native handle
    EXPECT_EQ(window.getNativeHandle(), nullptr);
}

// ==================== MockGraphicsContext Tests ====================

TEST(MockGraphicsContextTest, StartsUninitialized) {
    MockGraphicsContext context;
    
    EXPECT_FALSE(context.isInitialized());
    EXPECT_FALSE(context.isCleanedUp());
}

TEST(MockGraphicsContextTest, CanBeInitialized) {
    MockGraphicsContext context;
    
    EXPECT_NO_THROW({
        context.initialize();
    });
    
    EXPECT_TRUE(context.isInitialized());
}

TEST(MockGraphicsContextTest, CannotInitializeTwice) {
    MockGraphicsContext context;
    
    context.initialize();
    
    EXPECT_THROW({
        context.initialize();
    }, std::runtime_error);
}

TEST(MockGraphicsContextTest, CannotBeginFrameBeforeInitialize) {
    MockGraphicsContext context;
    
    EXPECT_THROW({
        context.beginFrame();
    }, std::runtime_error);
}

TEST(MockGraphicsContextTest, CanBeginAndEndFrame) {
    MockGraphicsContext context;
    context.initialize();
    
    EXPECT_NO_THROW({
        context.beginFrame();
        context.endFrame();
    });
    
    EXPECT_EQ(context.getFramesBegun(), 1);
    EXPECT_EQ(context.getFramesEnded(), 1);
    EXPECT_TRUE(context.allFramesCompleted());
}

TEST(MockGraphicsContextTest, CannotEndFrameWithoutBegin) {
    MockGraphicsContext context;
    context.initialize();
    
    EXPECT_THROW({
        context.endFrame();  // No beginFrame called
    }, std::runtime_error);
}

TEST(MockGraphicsContextTest, TracksMultipleFrames) {
    MockGraphicsContext context;
    context.initialize();
    
    for (int i = 0; i < 10; i++) {
        context.beginFrame();
        context.endFrame();
    }
    
    EXPECT_EQ(context.getFramesBegun(), 10);
    EXPECT_EQ(context.getFramesEnded(), 10);
    EXPECT_TRUE(context.allFramesCompleted());
}

TEST(MockGraphicsContextTest, CleanupCanBeCalledMultipleTimes) {
    MockGraphicsContext context;
    context.initialize();
    
    context.cleanup();
    EXPECT_TRUE(context.isCleanedUp());
    
    // Second cleanup should not throw
    EXPECT_NO_THROW({
        context.cleanup();
    });
}

TEST(MockGraphicsContextTest, CannotRenderAfterCleanup) {
    MockGraphicsContext context;
    context.initialize();
    context.cleanup();
    
    EXPECT_THROW({
        context.beginFrame();
    }, std::runtime_error);
}

TEST(MockGraphicsContextTest, CanSimulateInitializeFailure) {
    MockGraphicsContext context;
    context.setFailOnInitialize(true);
    
    EXPECT_THROW({
        context.initialize();
    }, std::runtime_error);
    
    EXPECT_FALSE(context.isInitialized());
}

TEST(MockGraphicsContextTest, CanSimulateBeginFrameFailure) {
    MockGraphicsContext context;
    context.initialize();
    context.setFailOnBeginFrame(true);
    
    EXPECT_THROW({
        context.beginFrame();
    }, std::runtime_error);
}

TEST(MockGraphicsContextTest, CanSimulateEndFrameFailure) {
    MockGraphicsContext context;
    context.initialize();
    context.beginFrame();
    context.setFailOnEndFrame(true);
    
    EXPECT_THROW({
        context.endFrame();
    }, std::runtime_error);
}

TEST(MockGraphicsContextTest, CanBeReset) {
    MockGraphicsContext context;
    context.initialize();
    context.beginFrame();
    context.endFrame();
    
    context.reset();
    
    EXPECT_FALSE(context.isInitialized());
    EXPECT_FALSE(context.isCleanedUp());
    EXPECT_EQ(context.getFramesBegun(), 0);
    EXPECT_EQ(context.getFramesEnded(), 0);
}

TEST(MockGraphicsContextTest, StateStringIsInformative) {
    MockGraphicsContext context;
    context.initialize();
    context.beginFrame();
    context.endFrame();
    
    std::string state = context.getStateString();
    
    // Should contain key information
    EXPECT_NE(state.find("Initialized: true"), std::string::npos);
    EXPECT_NE(state.find("Frames begun: 1"), std::string::npos);
    EXPECT_NE(state.find("Frames ended: 1"), std::string::npos);
}