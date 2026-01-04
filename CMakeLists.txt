cmake_minimum_required(VERSION 3.20)
project(HybridRenderer VERSION 1.0.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ==================== Find Vulkan (Optional) ====================

find_package(Vulkan)

if(Vulkan_FOUND)
    message(STATUS "✓ Vulkan found: ${Vulkan_VERSION}")
    set(BUILD_VULKAN_TARGETS ON)
else()
    message(WARNING "⚠ Vulkan SDK not found - building unit tests only")
    set(BUILD_VULKAN_TARGETS OFF)
endif()

# ==================== Find Shader Compiler (Optional) ====================

find_program(GLSL_VALIDATOR glslc HINTS 
    $ENV{VULKAN_SDK}/Bin
    $ENV{VULKAN_SDK}/bin
)

if(NOT GLSL_VALIDATOR)
    message(WARNING "⚠ glslc not found - shader compilation disabled")
    set(BUILD_SHADERS OFF)
else()
    message(STATUS "✓ glslc found: ${GLSL_VALIDATOR}")
    set(BUILD_SHADERS ON)
endif()

# ==================== Add GLFW (Always needed) ====================

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

if(UNIX AND NOT APPLE)
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
    message(STATUS "Linux: Configuring GLFW for X11")
endif()

add_subdirectory(external/glfw)
message(STATUS "✓ GLFW added")

# ==================== Add Google Test (Always needed) ====================

add_subdirectory(external/googletest)
message(STATUS "✓ Google Test added")

enable_testing()

# ==================== Shader Compilation (Optional) ====================

if(BUILD_SHADERS AND BUILD_VULKAN_TARGETS)
    set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
    set(SHADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

    file(MAKE_DIRECTORY ${SHADER_BINARY_DIR})

    add_custom_command(
        OUTPUT ${SHADER_BINARY_DIR}/triangle.vert.spv
        COMMAND ${GLSL_VALIDATOR} ${SHADER_SOURCE_DIR}/triangle.vert -o ${SHADER_BINARY_DIR}/triangle.vert.spv
        DEPENDS ${SHADER_SOURCE_DIR}/triangle.vert
        COMMENT "Compiling vertex shader"
    )

    add_custom_command(
        OUTPUT ${SHADER_BINARY_DIR}/triangle.frag.spv
        COMMAND ${GLSL_VALIDATOR} ${SHADER_SOURCE_DIR}/triangle.frag -o ${SHADER_BINARY_DIR}/triangle.frag.spv
        DEPENDS ${SHADER_SOURCE_DIR}/triangle.frag
        COMMENT "Compiling fragment shader"
    )

    add_custom_target(Shaders ALL
        DEPENDS 
            ${SHADER_BINARY_DIR}/triangle.vert.spv
            ${SHADER_BINARY_DIR}/triangle.frag.spv
    )
    
    message(STATUS "✓ Shader compilation enabled")
else()
    message(STATUS "⚠ Shader compilation disabled")
endif()

# ==================== Source Files ====================

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

# ==================== Main Application (Requires Vulkan) ====================

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

    # Only add shader dependency if shaders are being built
    if(BUILD_SHADERS)
        add_dependencies(HybridRenderer Shaders)
    endif()

    if(WIN32)
        target_compile_definitions(HybridRenderer PRIVATE
            VK_USE_PLATFORM_WIN32_KHR
            NOMINMAX
            WIN32_LEAN_AND_MEAN
        )
    endif()
    
    message(STATUS "✓ Main application configured")
else()
    message(STATUS "⚠ Skipping main application (Vulkan not available)")
endif()

# ==================== Unit Tests (No Vulkan Required) ====================

add_executable(HybridRenderer_unit_tests
    tests/main_test.cpp
    tests/unit/MockObjectsTest.cpp
    tests/unit/RendererUnitTest.cpp
    ${RENDERER_SOURCES}
)

target_include_directories(HybridRenderer_unit_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(HybridRenderer_unit_tests PRIVATE
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(HybridRenderer_unit_tests)

message(STATUS "✓ Unit tests configured (headless, no GPU)")

# ==================== Integration Tests (Requires Vulkan) ====================

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
    
    message(STATUS "✓ Integration tests configured")
else()
    message(STATUS "⚠ Skipping integration tests (Vulkan not available)")
endif()

# ==================== Platform-Specific Settings ====================

if(WIN32)
    message(STATUS "Platform: Windows")
elseif(APPLE)
    message(STATUS "Platform: macOS")
elseif(UNIX)
    message(STATUS "Platform: Linux")
endif()

# ==================== Summary ====================

message(STATUS "")
message(STATUS "=== Build Configuration ===")
message(STATUS "Vulkan targets:      ${BUILD_VULKAN_TARGETS}")
message(STATUS "Shader compilation:  ${BUILD_SHADERS}")
message(STATUS "Unit tests:          ON (always)")
message(STATUS "===========================")
message(STATUS "")