# ==============================================================================
# Modern CMake Build Configuration for C++ Ping-Pong Game
# ==============================================================================
# Tutorial Note:
# CMake is an open-source, cross-platform build system generator.
# `cmake_minimum_required` specifies the lowest version of CMake that can be used.
# ==============================================================================
cmake_minimum_required(VERSION 3.16)

# Define project name, version, and language
project(CppPingPong
    VERSION 1.0.0
    DESCRIPTION "Terminal-based Google Ping-Pong game with AI written in C++"
    LANGUAGES CXX
)

# Enforce C++17 Standard for modern features (std::optional, constexpr, structured bindings)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compiler warnings for industry-standard code safety
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Include directory for header files (.hpp)
include_directories(include)

# Collect source files
set(SOURCES
    src/Config.cpp
    src/Ball.cpp
    src/Paddle.cpp
    src/Renderer.cpp
    src/Game.cpp
)

# Main Executable Target
add_executable(pingpong src/main.cpp ${SOURCES})

# Target include directories for modular encapsulation
target_include_directories(pingpong PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Enable Testing
enable_testing()
add_executable(test_physics tests/test_physics.cpp ${SOURCES})
target_include_directories(test_physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_test(NAME PhysicsAndCollisionTest COMMAND test_physics)
