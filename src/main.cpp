#pragma once
#include "core/window.h"
#include "core/framebuffer.h"
#include "image/primitives.h"
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {
    try {
        // Create window and framebuffer
        Window window("DIY Software Renderer", WINDOW_WIDTH, WINDOW_HEIGHT);
        Framebuffer framebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

        std::cout << "DIY Renderer started!" << std::endl;
        std::cout << "Resolution: " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
        std::cout << "Press ESC to quit" << std::endl;

        bool running = true;
        SDL_Event event;

        // Main loop
        while (running) {
            // Handle events
            while (window.pollEvent(event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    running = false;
                } else if (event.type == SDL_EVENT_KEY_DOWN) {
                    if (event.key.key == SDLK_ESCAPE) {
                        running = false;
                    }
                }
            }

            // Clear framebuffer
            framebuffer.clear(makeColor(0, 0, 0));  // Black background

            // ========================================
            // YOUR RENDERING CODE GOES HERE!
            // ========================================

            // Draw a simple gradient
            FillWithGradient (framebuffer);
            // Then draw a red line over that
            DrawLine(0, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT/2, color::red(), framebuffer); // X-axis
            DrawLine(WINDOW_WIDTH/2, 0, WINDOW_WIDTH/2, WINDOW_HEIGHT, color::green(), framebuffer); // Y-axis
            DrawTriangle(WINDOW_WIDTH/2      , WINDOW_HEIGHT/4
                        , WINDOW_WIDTH*3/4, WINDOW_HEIGHT*3/4
                        , WINDOW_WIDTH/4, WINDOW_HEIGHT*3/4
                        , color::cyan(), framebuffer);

            // Display framebuffer
            window.present(framebuffer.data());

            // Cap framerate (~60 FPS)
            SDL_Delay(16);
        }

        std::cout << "Renderer closed cleanly" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
