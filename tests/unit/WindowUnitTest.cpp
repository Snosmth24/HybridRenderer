#include <gtest/gtest.h>
#include "../mocks/MockWindow.h"

TEST(MockWindowTest, CreatesWithCorrectDimensions) {
    MockWindow window(1920, 1080, "Test");
    
    EXPECT_EQ(window.getWidth(), 1920);
    EXPECT_EQ(window.getHeight(), 1080);
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
    
    window.resize(1920, 1080);
    
    EXPECT_EQ(window.getWidth(), 1920);
    EXPECT_EQ(window.getHeight(), 1080);
}

TEST(MockWindowTest, HandlesKeyInput) {
    MockWindow window(800, 600, "Test");
    
    EXPECT_FALSE(window.isKeyPressed(Key::W));
    
    window.setKeyPressed(Key::W, true);
    EXPECT_TRUE(window.isKeyPressed(Key::W));
    
    window.setKeyPressed(Key::W, false);
    EXPECT_FALSE(window.isKeyPressed(Key::W));
}

TEST(MockWindowTest, PollEventsDoesntCrash) {
    MockWindow window(800, 600, "Test");
    
    EXPECT_NO_THROW({
        window.pollEvents();
    });
}