#pragma once

#include <iostream>
#include <string>

/**
 * Utilities for testing
 */
namespace TestUtils {
    
    /**
     * Print a test section header
     */
    inline void printTestHeader(const std::string& testName) {
        std::cout << "\n";
        std::cout << "========================================\n";
        std::cout << "  " << testName << "\n";
        std::cout << "========================================\n";
    }
    
    /**
     * Print test step
     */
    inline void printTestStep(const std::string& step) {
        std::cout << "  → " << step << std::endl;
    }
    
    /**
     * Print test result
     */
    inline void printTestResult(bool passed, const std::string& message = "") {
        if (passed) {
            std::cout << "  ✓ PASS";
        } else {
            std::cout << "  ✗ FAIL";
        }
        
        if (!message.empty()) {
            std::cout << ": " << message;
        }
        
        std::cout << std::endl;
    }
}