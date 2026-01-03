#pragma once

#include "../../src/interfaces/IGraphicsContext.h"
#include <stdexcept>
#include <string>

/**
 * MockGraphicsContext - Fake graphics context for testing without GPU
 * 
 * This is a test double that implements IGraphicsContext but doesn't
 * talk to the GPU. It just tracks state in memory.
 * 
 * Benefits:
 * - No Vulkan/DirectX dependency
 * - No GPU/drivers required
 * - Fast (< 1ms per operation)
 * - State verification (can check what was called)
 * - Error injection (can simulate failures)
 * - Runs in CI/CD
 */
class MockGraphicsContext : public IGraphicsContext {
public:
    /**
     * Constructor - no GPU initialization needed
     */
    MockGraphicsContext()
        : initialized(false)
        , framesBegun(0)
        , framesEnded(0)
        , cleanedUp(false)
        , failOnInitialize(false)
        , failOnBeginFrame(false)
        , failOnEndFrame(false)
    {
    }
    
    /**
     * Destructor - verifies cleanup was called
     */
    ~MockGraphicsContext() override {
        // In tests, we want to verify cleanup was called explicitly
        // Destructor cleanup is a backup safety mechanism
        if (initialized && !cleanedUp) {
            // Don't throw in destructor, just track
            cleanedUp = true;
        }
    }
    
    // ==================== IGraphicsContext Interface ====================
    
    void initialize() override {
        // Simulate initialization failure if requested
        if (failOnInitialize) {
            throw std::runtime_error("MockGraphicsContext: Simulated initialization failure");
        }
        
        if (initialized) {
            throw std::runtime_error("MockGraphicsContext: Already initialized!");
        }
        
        initialized = true;
    }
    
    void beginFrame() override {
        // Simulate frame begin failure if requested
        if (failOnBeginFrame) {
            throw std::runtime_error("MockGraphicsContext: Simulated begin frame failure");
        }
        
        if (!initialized) {
            throw std::runtime_error("MockGraphicsContext: Cannot begin frame - not initialized!");
        }
        
        if (cleanedUp) {
            throw std::runtime_error("MockGraphicsContext: Cannot begin frame - already cleaned up!");
        }
        
        framesBegun++;
    }
    
    void endFrame() override {
        // Simulate frame end failure if requested
        if (failOnEndFrame) {
            throw std::runtime_error("MockGraphicsContext: Simulated end frame failure");
        }
        
        if (!initialized) {
            throw std::runtime_error("MockGraphicsContext: Cannot end frame - not initialized!");
        }
        
        if (cleanedUp) {
            throw std::runtime_error("MockGraphicsContext: Cannot end frame - already cleaned up!");
        }
        
        if (framesBegun <= framesEnded) {
            throw std::runtime_error("MockGraphicsContext: endFrame called without beginFrame!");
        }
        
        framesEnded++;
    }
    
    void cleanup() override {
        if (cleanedUp) {
            // Cleanup is idempotent - safe to call multiple times
            return;
        }
        
        cleanedUp = true;
    }
    
    // ==================== Test Verification Methods ====================
    
    /**
     * Check if context was initialized
     */
    bool isInitialized() const {
        return initialized;
    }
    
    /**
     * Check if context was cleaned up
     */
    bool isCleanedUp() const {
        return cleanedUp;
    }
    
    /**
     * Get number of frames that were begun
     */
    int getFramesBegun() const {
        return framesBegun;
    }
    
    /**
     * Get number of frames that were ended (completed)
     */
    int getFramesEnded() const {
        return framesEnded;
    }
    
    /**
     * Check if all begun frames were ended
     * Useful for detecting incomplete frames
     */
    bool allFramesCompleted() const {
        return framesBegun == framesEnded;
    }
    
    // ==================== Test Control Methods ====================
    
    /**
     * Simulate initialization failure
     * Next call to initialize() will throw
     */
    void setFailOnInitialize(bool fail) {
        failOnInitialize = fail;
    }
    
    /**
     * Simulate frame begin failure (e.g., device lost)
     * Next call to beginFrame() will throw
     */
    void setFailOnBeginFrame(bool fail) {
        failOnBeginFrame = fail;
    }
    
    /**
     * Simulate frame end failure (e.g., swapchain out of date)
     * Next call to endFrame() will throw
     */
    void setFailOnEndFrame(bool fail) {
        failOnEndFrame = fail;
    }
    
    /**
     * Reset all state (for running multiple tests)
     */
    void reset() {
        initialized = false;
        framesBegun = 0;
        framesEnded = 0;
        cleanedUp = false;
        failOnInitialize = false;
        failOnBeginFrame = false;
        failOnEndFrame = false;
    }
    
    /**
     * Get detailed state as string (for debugging failed tests)
     */
    std::string getStateString() const {
        std::string state = "MockGraphicsContext State:\n";
        state += "  Initialized: " + std::string(initialized ? "true" : "false") + "\n";
        state += "  Cleaned up: " + std::string(cleanedUp ? "true" : "false") + "\n";
        state += "  Frames begun: " + std::to_string(framesBegun) + "\n";
        state += "  Frames ended: " + std::to_string(framesEnded) + "\n";
        state += "  All frames completed: " + std::string(allFramesCompleted() ? "true" : "false") + "\n";
        return state;
    }

private:
    // State tracking
    bool initialized;
    int framesBegun;
    int framesEnded;
    bool cleanedUp;
    
    // Error injection flags
    bool failOnInitialize;
    bool failOnBeginFrame;
    bool failOnEndFrame;
};