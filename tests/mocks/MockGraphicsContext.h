#pragma once

#include "../../src/interfaces/IGraphicsContext.h"
#include <iostream>

class MockGraphicsContext : public IGraphicsContext {
public:
    MockGraphicsContext()
        : initialized(false)
        , framesRendered(0)
        , cleanedUp(false) {}
    
    ~MockGraphicsContext() override = default;
    
    void initialize() override {
        if (initialized) {
            throw std::runtime_error("Already initialized!");
        }
        initialized = true;
        std::cout << "[MOCK] Graphics context initialized" << std::endl;
    }
    
    void beginFrame() override {
        if (!initialized) {
            throw std::runtime_error("Not initialized!");
        }
        if (cleanedUp) {
            throw std::runtime_error("Already cleaned up!");
        }
        // Simulate frame start
    }
    
    void endFrame() override {
        if (!initialized) {
            throw std::runtime_error("Not initialized!");
        }
        if (cleanedUp) {
            throw std::runtime_error("Already cleaned up!");
        }
        framesRendered++;
    }
    
    void cleanup() override {
        if (cleanedUp) {
            return;  // Already cleaned up
        }
        cleanedUp = true;
        std::cout << "[MOCK] Graphics context cleaned up" << std::endl;
    }
    
    // Test helpers
    bool isInitialized() const { return initialized; }
    int getFramesRendered() const { return framesRendered; }
    bool isCleanedUp() const { return cleanedUp; }
    
private:
    bool initialized;
    int framesRendered;
    bool cleanedUp;
};