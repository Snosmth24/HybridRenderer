#pragma once

#include "../interfaces/IWindow.h"
#include "../interfaces/IGraphicsContext.h"

class Renderer {
public:
    Renderer(IWindow* window, IGraphicsContext* context);
    
    void initialize();
    void renderFrame();
    void cleanup();
    
private:
    IWindow* window;
    IGraphicsContext* context;
};