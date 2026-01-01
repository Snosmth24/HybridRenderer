#pragma once

#include "../interfaces/IWindow.h"
#include <map>

// Forward declare to avoid including GLFW in header
struct GLFWwindow;

class GLFWWindow : public IWindow {
public:
    GLFWWindow(int width, int height, const char* title);
    ~GLFWWindow() override;

    bool shouldClose() const override;
    void pollEvents() override;
    int getWidth() const override;
    int getHeight() const override;
    void* getNativeHandle() const override;

    bool isKeyPressed(Key key) const override;
    bool isMouseButtonPressed(MouseButton button) const override;
    void getCursorPosition(double& x, double& y) const override;

private:
    GLFWwindow* window;
    int width;
    int height;

    int toGLFWKey(Key key) const;
    int toGLFWMouseButton(MouseButton button) const;
};