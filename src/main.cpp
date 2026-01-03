#include <iostream>
#include <stdexcept>
#include <chrono>

#include "platform/GLFWWindow.h"
#include "vulkan/VulkanContext.h"
#include "renderer/Renderer.h"

#include <iostream>
#include <stdexcept>

#include "platform/GLFWWindow.h"
#include "vulkan/VulkanContext.h"
#include "renderer/Renderer.h"
#include "utils/Logger.h"

int main() {
    try {
		std::cout << "=== Hybrid Renderer Starting ===" << std::endl;
        std::cout << std::endl;

        // Create window
        std::cout << "Creating window..." << std::endl;
        GLFWWindow window(1920, 1080, "Hybrid Renderer");
        std::cout << std::endl;

        // Create Vulkan context
        std::cout << "Creating Vulkan context..." << std::endl;
        VulkanContext vulkanContext(&window);
        std::cout << std::endl;

        // Create renderer
        std::cout << "Creating renderer..." << std::endl;
        Renderer renderer(&window, &vulkanContext);
        std::cout << std::endl;

        // Initialize
        std::cout << "Initializing renderer..." << std::endl;
        renderer.initialize();
        std::cout << std::endl;

        std::cout << "=== Initialization Complete ===" << std::endl;
        std::cout << "Window: " << window.getWidth() << "x" << window.getHeight() << std::endl;
        std::cout << "Controls: WASD to move, ESC to exit" << std::endl;
        std::cout << std::endl;

        // Main loop
        std::cout << "=== Entering Main Loop ===" << std::endl;

        int frameCount = 0;
        auto startTime = std::chrono::high_resolution_clock::now();
        auto lastPrintTime = startTime;

        while (!window.shouldClose()) {
            window.pollEvents();

            // Test input
            if (window.isKeyPressed(Key::W)) {
                std::cout << "[Input] W pressed" << std::endl;
            }
            if (window.isKeyPressed(Key::Space)) {
                std::cout << "[Input] Space pressed" << std::endl;
            }

            renderer.renderFrame();
            frameCount++;

            // Print FPS every second
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPrintTime);

            if (elapsed.count() >= 1000) {
                float fps = frameCount / (elapsed.count() / 1000.0f);
                std::cout << "FPS: " << fps << " | Frames: " << frameCount << std::endl;

                frameCount = 0;
                lastPrintTime = now;
            }
        }

        std::cout << std::endl;
        std::cout << "=== Exiting Main Loop ===" << std::endl;
        std::cout << std::endl;

        // Cleanup
        std::cout << "=== Shutting Down ===" << std::endl;
        vulkanContext.waitIdle();
        renderer.cleanup();

        std::cout << std::endl;
        std::cout << "=== Success! ===" << std::endl;
        return 0;

    }
    catch (const std::exception& e) {
        std::cerr << std::endl;
        std::cerr << "=== FATAL ERROR ===" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << std::endl;
        std::cerr << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }
}