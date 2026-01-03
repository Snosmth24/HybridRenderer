cmake_minimum_required(VERSION 3.20)
project(HybridRenderer VERSION 1.0.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find Vulkan
find_package(Vulkan REQUIRED)

if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan not found. Install Vulkan SDK from https://vulkan.lunarg.com/")
endif()

message(STATUS "Vulkan found: ${Vulkan_VERSION}")

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

# Find shader compiler
find_program(GLSL_VALIDATOR glslc HINTS 
    $ENV{VULKAN_SDK}/Bin
    $ENV{VULKAN_SDK}/bin
)

if(NOT GLSL_VALIDATOR)
    message(FATAL_ERROR "glslc not found! Make sure Vulkan SDK is installed.")
endif()

message(STATUS "glslc found: ${GLSL_VALIDATOR}")

# Shader compilation
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
    
    # Unit tests (headless)
    tests/unit/WindowUnitTest.cpp
    tests/unit/RendererUnitTest.cpp
    
    # Mocks (no implementation needed)
    tests/mocks/MockWindow.h
    tests/mocks/MockGraphicsContext.h
    
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






# Main executable
add_executable(HybridRenderer
    src/main.cpp
    ${INTERFACE_HEADERS}
    ${RENDERER_SOURCES}
    ${VULKAN_SOURCES}
    ${PLATFORM_SOURCES}
)

# Include directories
target_include_directories(HybridRenderer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(HybridRenderer PRIVATE
    Vulkan::Vulkan
    glfw
)

# Add shader dependency
add_dependencies(HybridRenderer Shaders)

# Windows-specific settings
if(WIN32)
    target_compile_definitions(HybridRenderer PRIVATE
        VK_USE_PLATFORM_WIN32_KHR
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
endif()
# ==================== INTEGRATION TESTS (Require GPU) ====================

add_executable(HybridRenderer_integration_tests
    # Test main
    tests/main_test.cpp
    
    # Integration tests (need GPU)
    tests/WindowTest.cpp
    tests/VulkanContextTest.cpp
    
    # Real implementations
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

# Register integration tests
gtest_discover_tests(HybridRenderer_integration_tests)

add_executable(HybridRenderer_tests
    # Test main
    tests/main_test.cpp  # ← ADD THIS
    
    # Test files
    tests/WindowTest.cpp
    tests/VulkanContextTest.cpp
    
    # Source files
    ${VULKAN_SOURCES}
    ${PLATFORM_SOURCES}
    ${RENDERER_SOURCES}
)

target_include_directories(HybridRenderer_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(HybridRenderer_tests PRIVATE
    gtest
    # gtest_main  # ← REMOVE THIS (we have our own main now)
    Vulkan::Vulkan
    glfw
)

# Windows-specific settings for tests
if(WIN32)
    target_compile_definitions(HybridRenderer_tests PRIVATE
        VK_USE_PLATFORM_WIN32_KHR
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
endif()

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(HybridRenderer_tests)