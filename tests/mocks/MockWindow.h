#pragma once

#include "../../src/interfaces/IWindow.h"
#include <map>
#include <string>

/**
 * MockWindow - Fake window for testing without GPU/display
 * 
 * This is a test double that implements IWindow but doesn't create
 * a real window. It just stores values in memory.
 * 
 * Benefits:
 * - No GLFW dependency (works headless)
 * - No GPU required
 * - Fast (< 1ms to create)
 * - Controllable (can programmatically set state)
 * - Runs in CI/CD
 */
class MockWindow : public IWindow {
public:
    /**
     * Constructor - doesn't create real window, just stores parameters
     */
    MockWindow(int width, int height, const char* title)
        : width(width)
        , height(height)
        , title(title)
        , closed(false)
    {
        // No GLFW calls - just initialization
    }
    
    /**
     * Destructor - nothing to clean up (no real window)
     */
    ~MockWindow() override = default;
    
    // ==================== IWindow Interface Implementation ====================
    
    bool shouldClose() const override {
        return closed;
    }
    
    void pollEvents() override {
        // Mock implementation: do nothing
        // Real window would process OS events here
    }
    
    int getWidth() const override {
        return width;
    }
    
    int getHeight() const override {
        return height;
    }
    
    void* getNativeHandle() const override {
        // Mock window has no native handle
        return nullptr;
    }
    
    bool isKeyPressed(Key key) const override {
        auto it = keyStates.find(key);
        return (it != keyStates.end()) ? it->second : false;
    }
    
    bool isMouseButtonPressed(MouseButton button) const override {
        auto it = mouseButtonStates.find(button);
        return (it != mouseButtonStates.end()) ? it->second : false;
    }
    
    void getCursorPosition(double& x, double& y) const override {
        x = cursorX;
        y = cursorY;
    }
    
    // ==================== Test Control Methods ====================
    
    /**
     * Programmatically set key state (for testing)
     * Real window: key state changed by user input
     * Mock window: test controls the state
     */
    void setKeyPressed(Key key, bool pressed) {
        keyStates[key] = pressed;
    }
    
    /**
     * Programmatically set mouse button state
     */
    void setMouseButtonPressed(MouseButton button, bool pressed) {
        mouseButtonStates[button] = pressed;
    }
    
    /**
     * Programmatically set cursor position
     */
    void setCursorPosition(double x, double y) {
        cursorX = x;
        cursorY = y;
    }
    
    /**
     * Programmatically close the window
     */
    void setClosed(bool shouldClose) {
        closed = shouldClose;
    }
    
    /**
     * Simulate window resize
     * Real window: OS/user resizes window
     * Mock window: test triggers resize
     */
    void resize(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;
    }
    
    /**
     * Get window title (for verification in tests)
     */
    const std::string& getTitle() const {
        return title;
    }
    
    // ==================== Test Verification Methods ====================
    
    /**
     * Check how many times pollEvents was called
     * Useful for verifying main loop behavior
     */
    int getPollEventsCallCount() const {
        return pollEventsCallCount;
    }
    
    /**
     * Reset call counters (between tests)
     */
    void resetCallCounters() {
        pollEventsCallCount = 0;
    }

private:
    // Window properties
    int width;
    int height;
    std::string title;
    bool closed;
    
    // Input state
    std::map<Key, bool> keyStates;
    std::map<MouseButton, bool> mouseButtonStates;
    double cursorX = 0.0;
    double cursorY = 0.0;
    
    // Call tracking for verification
    mutable int pollEventsCallCount = 0;
};