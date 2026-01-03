#include "GLFWWindow.h"
#include "../utils/Logger.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

GLFWWindow::GLFWWindow(int width, int height, const char* title)
    : width(width), height(height), window(nullptr)
{
    // Initialize GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    Logger::verbose("GLFW initialized");

    // Tell GLFW not to create an OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create window
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window!");
    }

    glfwSetKeyCallback(window, keyCallback);

    Logger::verbose("Window created");

}

GLFWWindow::~GLFWWindow() {
    if (window) {
        glfwDestroyWindow(window);
        Logger::verbose("Window destroyed");
    }

    glfwTerminate();
    Logger::verbose("GLFW terminated");
}

bool GLFWWindow::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void GLFWWindow::pollEvents() {
    glfwPollEvents();
}

int GLFWWindow::getWidth() const {
    return width;
}

int GLFWWindow::getHeight() const {
    return height;
}

void* GLFWWindow::getNativeHandle() const {
    return window;
}

//////////////////////input functions///////////////////////////

bool GLFWWindow::isKeyPressed(Key key) const {
    int glfwKey = toGLFWKey(key);
    if (glfwKey == GLFW_KEY_UNKNOWN) return false;

    return glfwGetKey(window, glfwKey) == GLFW_PRESS;
}

bool GLFWWindow::isMouseButtonPressed(MouseButton button) const {
    int glfwButton = toGLFWMouseButton(button);
    return glfwGetMouseButton(window, glfwButton) == GLFW_PRESS;
}

void GLFWWindow::getCursorPosition(double& x, double& y) const {
    glfwGetCursorPos(window, &x, &y);
}

int GLFWWindow::toGLFWKey(Key key) const {
    switch (key) {
    case Key::W: return GLFW_KEY_W;
    case Key::A: return GLFW_KEY_A;
    case Key::S: return GLFW_KEY_S;
    case Key::D: return GLFW_KEY_D;
    case Key::Up: return GLFW_KEY_UP;
    case Key::Down: return GLFW_KEY_DOWN;
    case Key::Left: return GLFW_KEY_LEFT;
    case Key::Right: return GLFW_KEY_RIGHT;
    case Key::Escape: return GLFW_KEY_ESCAPE;
    case Key::Space: return GLFW_KEY_SPACE;
    case Key::LeftShift: return GLFW_KEY_LEFT_SHIFT;
    case Key::RightShift: return GLFW_KEY_RIGHT_SHIFT;
    case Key::LeftControl: return GLFW_KEY_LEFT_CONTROL;
    case Key::RightControl: return GLFW_KEY_RIGHT_CONTROL;
    default: return GLFW_KEY_UNKNOWN;
    }
}

int GLFWWindow::toGLFWMouseButton(MouseButton button) const {
    switch (button) {
    case MouseButton::Left: return GLFW_MOUSE_BUTTON_LEFT;
    case MouseButton::Right: return GLFW_MOUSE_BUTTON_RIGHT;
    case MouseButton::Middle: return GLFW_MOUSE_BUTTON_MIDDLE;
    default: return GLFW_MOUSE_BUTTON_LEFT;
    }
}