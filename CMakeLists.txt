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

# Source files
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

# Find glslc (shader compiler from Vulkan SDK)
find_program(GLSL_VALIDATOR glslc HINTS 
    $ENV{VULKAN_SDK}/Bin
    $ENV{VULKAN_SDK}/bin
)

if(NOT GLSL_VALIDATOR)
    message(FATAL_ERROR "glslc not found!")
endif()

# Compile shaders
set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

file(MAKE_DIRECTORY ${SHADER_BINARY_DIR})

# Vertex shader
add_custom_command(
    OUTPUT ${SHADER_BINARY_DIR}/triangle.vert.spv
    COMMAND ${GLSL_VALIDATOR} ${SHADER_SOURCE_DIR}/triangle.vert -o ${SHADER_BINARY_DIR}/triangle.vert.spv
    DEPENDS ${SHADER_SOURCE_DIR}/triangle.vert
    COMMENT "Compiling vertex shader"
)

# Fragment shader
add_custom_command(
    OUTPUT ${SHADER_BINARY_DIR}/triangle.frag.spv
    COMMAND ${GLSL_VALIDATOR} ${SHADER_SOURCE_DIR}/triangle.frag -o ${SHADER_BINARY_DIR}/triangle.frag.spv
    DEPENDS ${SHADER_SOURCE_DIR}/triangle.frag
    COMMENT "Compiling fragment shader"
)

# Add shaders as dependency
add_custom_target(Shaders ALL
    DEPENDS 
        ${SHADER_BINARY_DIR}/triangle.vert.spv
        ${SHADER_BINARY_DIR}/triangle.frag.spv
)

add_dependencies(HybridRenderer Shaders)

# Windows-specific settings
if(WIN32)
    target_compile_definitions(HybridRenderer PRIVATE
        VK_USE_PLATFORM_WIN32_KHR
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
endif()