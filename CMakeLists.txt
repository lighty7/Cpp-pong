# ==============================================================================
# Modern CMake Build Configuration for C++ Ping-Pong Game
# ==============================================================================
# Tutorial Note:
# CMake is an open-source, cross-platform build system generator.
# `cmake_minimum_required` specifies the lowest version of CMake that can be used.
# ==============================================================================
cmake_minimum_required(VERSION 3.16)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Define project name, version, and language
project(CppPingPong
    VERSION 1.0.0
    DESCRIPTION "2D Graphical Ping-Pong game written in C++"
    LANGUAGES CXX C
)

# Enforce C++17 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compiler warnings for safety
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Fetch SDL2 2D GUI library
include(FetchContent)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_X11 OFF CACHE BOOL "" FORCE)
set(VIDEO_X11 OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL2
    URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.30.2.tar.gz
)
FetchContent_MakeAvailable(SDL2)

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
target_include_directories(pingpong PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(pingpong PRIVATE SDL2-static)

# Enable Testing
enable_testing()
add_executable(test_physics tests/test_physics.cpp ${SOURCES})
target_include_directories(test_physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test_physics PRIVATE SDL2-static)
add_test(NAME PhysicsAndCollisionTest COMMAND test_physics)
