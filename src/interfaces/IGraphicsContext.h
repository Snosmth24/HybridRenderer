#pragma once

class IGraphicsContext {
public:
    virtual ~IGraphicsContext() = default;
    
    virtual void initialize() = 0;
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
    virtual void cleanup() = 0;
};