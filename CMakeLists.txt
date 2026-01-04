cmake_minimum_required(VERSION 3.20)
project(HybridRenderer VERSION 1.0.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Find Vulkan (optional for CI - only needed for main app and integration tests)
find_package(Vulkan)

if(Vulkan_FOUND)
    message(STATUS "✓ Vulkan SDK found: ${Vulkan_VERSION}")
    set(BUILD_VULKAN_TARGETS ON)
else()
    message(WARNING "Vulkan SDK not found - building unit tests only")
    set(BUILD_VULKAN_TARGETS OFF)
endif()

# Add GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

message(STATUS "GLFW added successfully")

# Add Google Test
add_subdirectory(external/googletest)

message(STATUS "Google Test added successfully")

# Enable testing
enable_testing()

# Find shader compiler (only needed if building Vulkan targets)
find_program(GLSL_VALIDATOR glslc HINTS 
    $ENV{VULKAN_SDK}/Bin
    $ENV{VULKAN_SDK}/bin
)

if(NOT GLSL_VALIDATOR)
    message(WARNING "glslc not found - shader compilation disabled")
    set(BUILD_SHADERS OFF)
else()
    message(STATUS "glslc found: ${GLSL_VALIDATOR}")
    set(BUILD_SHADERS ON)
endif()


# Shader compilation (only if glslc available)
if(BUILD_SHADERS AND BUILD_VULKAN_TARGETS)
    set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
    set(SHADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

    file(MAKE_DIRECTORY ${SHADER_BINARY_DIR})

    # Compile vertex shader
    add_custom_command(
        OUTPUT ${SHADER_BINARY_DIR}/triangle.vert.spv
        COMMAND ${GLSL_VALIDATOR} ${SHADER_SOURCE_DIR}/triangle.vert -o ${SHADER_BINARY_DIR}/triangle.vert.spv
        DEPENDS ${SHADER_SOURCE_DIR}/triangle.vert
        COMMENT "Compiling vertex shader"
    )

    # Compile fragment shader
    add_custom_command(
        OUTPUT ${SHADER_BINARY_DIR}/triangle.frag.spv
        COMMAND ${GLSL_VALIDATOR} ${SHADER_SOURCE_DIR}/triangle.frag -o ${SHADER_BINARY_DIR}/triangle.frag.spv
        DEPENDS ${SHADER_SOURCE_DIR}/triangle.frag
        COMMENT "Compiling fragment shader"
    )

    # Create shader target
    add_custom_target(Shaders ALL
        DEPENDS 
            ${SHADER_BINARY_DIR}/triangle.vert.spv
            ${SHADER_BINARY_DIR}/triangle.frag.spv
    )
    
    message(STATUS "✓ Shader compilation enabled")
else()
    message(STATUS "⚠ Shader compilation disabled (glslc not available)")
endif()

# Source files organized by component
set(INTERFACE_HEADERS
    src/interfaces/IWindow.h
    src/interfaces/IGraphicsContext.h
)

set(RENDERER_SOURCES
    src/renderer/Renderer.h
    src/renderer/Renderer.cpp
)

set(VULKAN_SOURCES
    src/vulkan/VulkanContext.h
    src/vulkan/VulkanContext.cpp
)

set(PLATFORM_SOURCES
    src/platform/GLFWWindow.h
    src/platform/GLFWWindow.cpp
)
# ==================== UNIT TESTS (Headless - No GPU) ====================

add_executable(HybridRenderer_unit_tests
    # Test main
    tests/main_test.cpp
	
	# Unit tests
    tests/unit/MockObjectsTest.cpp
	tests/unit/RendererUnitTest.cpp
	
	# Mocks (header-only, but list for IDE)
    tests/mocks/MockWindow.h
    tests/mocks/MockGraphicsContext.h
    tests/mocks/TestUtils.h
    
    
  
    
    # Only include renderer source (no Vulkan/GLFW)
    ${RENDERER_SOURCES}
)

target_include_directories(HybridRenderer_unit_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(HybridRenderer_unit_tests PRIVATE
    gtest
    gtest_main
)

# Register unit tests
include(GoogleTest)
gtest_discover_tests(HybridRenderer_unit_tests)

message(STATUS "Unit tests configured (headless, no GPU required)")

# ==================== INTEGRATION TESTS (GPU Required) ====================

# Integration tests (require Vulkan)
if(BUILD_VULKAN_TARGETS)
    add_executable(HybridRenderer_integration_tests
        tests/main_test.cpp
        tests/integration/WindowIntegrationTest.cpp
        tests/integration/VulkanContextIntegrationTest.cpp
        ${VULKAN_SOURCES}
        ${PLATFORM_SOURCES}
        ${RENDERER_SOURCES}
    )

    target_include_directories(HybridRenderer_integration_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${Vulkan_INCLUDE_DIRS}
    )

    target_link_libraries(HybridRenderer_integration_tests PRIVATE
        gtest
        gtest_main
        Vulkan::Vulkan
        glfw
    )

    if(WIN32)
        target_compile_definitions(HybridRenderer_integration_tests PRIVATE
            VK_USE_PLATFORM_WIN32_KHR
            NOMINMAX
            WIN32_LEAN_AND_MEAN
        )
    endif()

    gtest_discover_tests(HybridRenderer_integration_tests)
    
    message(STATUS "✓ Integration tests target configured")
else()
    message(STATUS "⚠ Skipping integration tests (Vulkan not available)")
endif()

target_include_directories(HybridRenderer_integration_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(HybridRenderer_integration_tests PRIVATE
    gtest
    gtest_main
    Vulkan::Vulkan
    glfw
)

# Register with CTest
gtest_discover_tests(HybridRenderer_integration_tests)

message(STATUS "✓ Integration tests configured (requires GPU)")

# ==================== TEST SUMMARY ====================

message(STATUS "")
message(STATUS "=== Test Configuration ===")
message(STATUS "Unit tests:        Fast (<20ms), headless, no GPU")
message(STATUS "Integration tests: Slow (3-5s), requires GPU and drivers")
message(STATUS "")
message(STATUS "Quick test: bin/Debug/HybridRenderer_unit_tests.exe")
message(STATUS "Full test:  bin/Debug/HybridRenderer_integration_tests.exe")
message(STATUS "==========================")




# Main executable (requires Vulkan)
if(BUILD_VULKAN_TARGETS)
    add_executable(HybridRenderer
        src/main.cpp
        ${INTERFACE_HEADERS}
        ${RENDERER_SOURCES}
        ${VULKAN_SOURCES}
        ${PLATFORM_SOURCES}
    )

    target_include_directories(HybridRenderer PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${Vulkan_INCLUDE_DIRS}
    )

    target_link_libraries(HybridRenderer PRIVATE
        Vulkan::Vulkan
        glfw
    )

    add_dependencies(HybridRenderer Shaders)

    if(WIN32)
        target_compile_definitions(HybridRenderer PRIVATE
            VK_USE_PLATFORM_WIN32_KHR
            NOMINMAX
            WIN32_LEAN_AND_MEAN
        )
    endif()
    
    message(STATUS "✓ Main application target configured")
else()
    message(STATUS "⚠ Skipping main application (Vulkan not available)")
endif()

# ==================== PLATFORM-SPECIFIC SETTINGS ====================

if(WIN32)
    message(STATUS "Platform: Windows")
    
    target_compile_definitions(HybridRenderer PRIVATE
        VK_USE_PLATFORM_WIN32_KHR
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
    
    target_compile_definitions(HybridRenderer_integration_tests PRIVATE
        VK_USE_PLATFORM_WIN32_KHR
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
    
elseif(APPLE)
    message(STATUS "Platform: macOS")
    
    # macOS uses MoltenVK (Vulkan over Metal)
    target_compile_definitions(HybridRenderer PRIVATE
        VK_USE_PLATFORM_MACOS_MVK
        VK_USE_PLATFORM_METAL_EXT
    )
    
    target_compile_definitions(HybridRenderer_integration_tests PRIVATE
        VK_USE_PLATFORM_MACOS_MVK
        VK_USE_PLATFORM_METAL_EXT
    )
    
    # macOS-specific settings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    
elseif(UNIX)
    message(STATUS "Platform: Linux")
    
    # Linux can use X11 or Wayland
    # Check which is available
    find_package(X11)
    
    if(X11_FOUND)
        message(STATUS "  Using X11 for window management")
        target_compile_definitions(HybridRenderer PRIVATE
            VK_USE_PLATFORM_XLIB_KHR
        )
        target_compile_definitions(HybridRenderer_integration_tests PRIVATE
            VK_USE_PLATFORM_XLIB_KHR
        )
        target_link_libraries(HybridRenderer PRIVATE X11)
        target_link_libraries(HybridRenderer_integration_tests PRIVATE X11)
    else()
        message(STATUS "  Using Wayland for window management")
        target_compile_definitions(HybridRenderer PRIVATE
            VK_USE_PLATFORM_WAYLAND_KHR
        )
        target_compile_definitions(HybridRenderer_integration_tests PRIVATE
            VK_USE_PLATFORM_WAYLAND_KHR
        )
    endif()
    
    # Linux-specific settings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
    
endif()

message(STATUS "")
