#pragma once

#include <SDL3/SDL.h>
#include <string>

// Minimal window wrapper around SDL3
// This is just for display - all rendering is done by YOU in the framebuffer!
class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    // Prevent copying
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Display framebuffer to screen
    void present(const uint32_t* pixelData);

    // Event handling
    bool pollEvent(SDL_Event& event);

    // Properties
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isOpen() const { return window != nullptr; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width;
    int height;
};
