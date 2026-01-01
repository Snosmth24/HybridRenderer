#pragma once

enum class Key {
    Unknown = -1,
    W, A, S, D,
    Up, Down, Left, Right,
    Escape,
    Space,
    LeftShift, RightShift,
    LeftControl, RightControl
};

enum class MouseButton {
    Left,
    Right,
    Middle
};

class IWindow {
public:
    virtual ~IWindow() = default;
    
    virtual bool shouldClose() const = 0;
    virtual void pollEvents() = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual void* getNativeHandle() const = 0;


    virtual bool isKeyPressed(Key key) const = 0;
    virtual bool isMouseButtonPressed(MouseButton button) const = 0;
    virtual void getCursorPosition(double& x, double& y) const = 0;
};