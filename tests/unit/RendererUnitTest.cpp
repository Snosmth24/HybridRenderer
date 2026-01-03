#include <gtest/gtest.h>
#include "../../src/renderer/Renderer.h"
#include "../mocks/MockWindow.h"
#include "../mocks/MockGraphicsContext.h"
#include "../../src/utils/Logger.h"

class RendererUnitTest : public ::testing::Test {
protected:
    void SetUp() override {
        Logger::setLevel(LogLevel::Silent);
        
        window = new MockWindow(1920, 1080, "Mock Window");
        context = new MockGraphicsContext();
        renderer = new Renderer(window, context);
    }
    
    void TearDown() override {
        delete renderer;
        delete context;
        delete window;
        
        Logger::setLevel(LogLevel::Normal);
    }
    
    MockWindow* window;
    MockGraphicsContext* context;
    Renderer* renderer;
};

TEST_F(RendererUnitTest, InitializesSuccessfully) {
    EXPECT_NO_THROW({
        renderer->initialize();
    });
    
    EXPECT_TRUE(context->isInitialized());
}

TEST_F(RendererUnitTest, RendersFrames) {
    renderer->initialize();
    
    EXPECT_EQ(context->getFramesRendered(), 0);
    
    renderer->renderFrame();
    EXPECT_EQ(context->getFramesRendered(), 1);
    
    renderer->renderFrame();
    EXPECT_EQ(context->getFramesRendered(), 2);
}

TEST_F(RendererUnitTest, CleansUpProperly) {
    renderer->initialize();
    renderer->renderFrame();
    
    EXPECT_NO_THROW({
        renderer->cleanup();
    });
    
    EXPECT_TRUE(context->isCleanedUp());
}

TEST_F(RendererUnitTest, CannotRenderBeforeInitialize) {
    // Should handle gracefully or throw
    EXPECT_THROW({
        renderer->renderFrame();
    }, std::runtime_error);
}

TEST_F(RendererUnitTest, CannotInitializeTwice) {
    renderer->initialize();
    
    EXPECT_THROW({
        renderer->initialize();
    }, std::runtime_error);
}

TEST_F(RendererUnitTest, HandlesMockWindowResize) {
    renderer->initialize();
    
    window->resize(1280, 720);
    
    EXPECT_EQ(window->getWidth(), 1280);
    EXPECT_EQ(window->getHeight(), 720);
    
    // Should still render after resize
    EXPECT_NO_THROW({
        renderer->renderFrame();
    });
}

TEST_F(RendererUnitTest, HandlesKeyInput) {
    renderer->initialize();
    
    window->setKeyPressed(Key::W, true);
    EXPECT_TRUE(window->isKeyPressed(Key::W));
    
    window->setKeyPressed(Key::W, false);
    EXPECT_FALSE(window->isKeyPressed(Key::W));
}