#pragma once

#include "../../src/interfaces/IWindow.h"

class MockWindow : public IWindow {
public:
    MockWindow(int width, int height, const char* title)
        : width(width), height(height), title(title), closed(false) {}
    
    ~MockWindow() override = default;
    
    bool shouldClose() const override {
        return closed;
    }
    
    void pollEvents() override {
        // No-op in mock
    }
    
    int getWidth() const override {
        return width;
    }
    
    int getHeight() const override {
        return height;
    }
    
    void* getNativeHandle() const override {
        return nullptr;  // Mock has no real window
    }
    
    bool isKeyPressed(Key key) const override {
        return keyStates.find(key) != keyStates.end() ? keyStates.at(key) : false;
    }
    
    bool isMouseButtonPressed(MouseButton button) const override {
        return false;
    }
    
    void getCursorPosition(double& x, double& y) const override {
        x = 0.0;
        y = 0.0;
    }
    
    // Test helpers
    void setKeyPressed(Key key, bool pressed) {
        keyStates[key] = pressed;
    }
    
    void setClosed(bool closed) {
        this->closed = closed;
    }
    
    void resize(int newWidth, int newHeight) {
        this->width = newWidth;
        this->height = newHeight;
    }
    
private:
    int width;
    int height;
    std::string title;
    bool closed;
    std::map<Key, bool> keyStates;
};