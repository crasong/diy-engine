cmake_minimum_required(VERSION 3.14)  # Minimum for FetchContent
project(DIYEngine VERSION 1.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Enable optimizations in release mode
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Option to use system SDL3 or fetch from source
option(USE_SYSTEM_SDL3 "Use system-installed SDL3 instead of fetching" OFF)

if(USE_SYSTEM_SDL3)
    # Try to find system SDL3
    find_package(SDL3 REQUIRED)
else()
    # Fetch SDL3 from GitHub
    include(FetchContent)

    message(STATUS "Fetching SDL3 from source...")

    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG main  # Or use a specific release tag like "release-3.1.2"
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )

    # Configure SDL3 options (disable things we don't need)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
    set(SDL_TEST OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(SDL3)
endif()

# Include directories
include_directories(src)

# Source files
set(SOURCES
    src/main.cpp
    src/core/framebuffer.cpp
    src/core/window.cpp
    # Note: vec3.h, mat4.h, and color.h are header-only
    # Add .cpp files here only if you create them later
)

# Create executable
add_executable(renderer ${SOURCES})

# Link libraries (only SDL3)
if(USE_SYSTEM_SDL3)
    target_link_libraries(renderer SDL3::SDL3)
else()
    target_link_libraries(renderer SDL3::SDL3-shared)
endif()

# Optional: Add debug/release configurations
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_MODE)
    message(STATUS "Building in Debug mode")
else()
    message(STATUS "Building in Release mode")
endif()
