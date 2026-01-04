#pragma once

#include <iostream>
#include <string>
//
enum class LogLevel {
    Silent,   // No output
    Minimal,  // Errors only
    Normal,   // Errors + Important info
    Verbose,  // Everything
    Test      // Special mode for tests
};

class Logger {
public:
    static void setLevel(LogLevel level) {
        currentLevel = level;
    }
    
    static LogLevel getLevel() {
        return currentLevel;
    }
    
    static void info(const std::string& message) {
        if (currentLevel >= LogLevel::Normal) {
            std::cout << message << std::endl;
        }
    }
    
    static void verbose(const std::string& message) {
        if (currentLevel >= LogLevel::Verbose) {
            std::cout << message << std::endl;
        }
    }
    
    static void error(const std::string& message) {
        if (currentLevel >= LogLevel::Minimal) {
            std::cerr << "\033[91m[ERROR] " << message << "\033[0m" << std::endl;
        }
    }
    
    static void success(const std::string& message) {
        if (currentLevel >= LogLevel::Normal) {
            std::cout << "\033[92m✓ " << message << "\033[0m" << std::endl;
        }
    }
    
    static void testInfo(const std::string& message) {
        if (currentLevel == LogLevel::Test) {
            std::cout << "  " << message << std::endl;
        }
    }
    
private:
    static LogLevel currentLevel;
};

// Initialize in cpp file
inline LogLevel Logger::currentLevel = LogLevel::Normal;