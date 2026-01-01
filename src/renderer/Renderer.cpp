#include "Renderer.h"
#include <iostream>

Renderer::Renderer(IWindow* window, IGraphicsContext* context)
    : window(window), context(context)
{
}

void Renderer::initialize() {
    std::cout << "Renderer initializing..." << std::endl;
    context->initialize();
    std::cout << "Renderer initialized successfully!" << std::endl;
}

void Renderer::renderFrame() {
    context->beginFrame();
    
    // TODO: Actual rendering will go here
    
    context->endFrame();
}

void Renderer::cleanup() {
    std::cout << "Renderer cleaning up..." << std::endl;
    context->cleanup();
}