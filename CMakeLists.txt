cmake_minimum_required(VERSION 3.10)
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

# Find SDL2 (only external dependency for windowing)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Include directories
include_directories(src)

# Source files
set(SOURCES
    src/main.cpp
    src/core/framebuffer.cpp
    src/core/window.cpp
    # Math files will go here as you create them
    # src/math/vec3.cpp
    # src/math/mat4.cpp
)

# Create executable
add_executable(renderer ${SOURCES})

# Link libraries (only SDL2)
target_link_libraries(renderer ${SDL2_LIBRARIES})

# Optional: Add debug/release configurations
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_MODE)
    message(STATUS "Building in Debug mode")
else()
    message(STATUS "Building in Release mode")
endif()
