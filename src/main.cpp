#include "core/window.h"
#include "core/framebuffer.h"
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

            // Example: Draw a simple gradient
            for (int y = 0; y < WINDOW_HEIGHT; y++) {
                for (int x = 0; x < WINDOW_WIDTH; x++) {
                    uint8_t r = (x * 255) / WINDOW_WIDTH;
                    uint8_t g = (y * 255) / WINDOW_HEIGHT;
                    uint8_t b = 128;
                    framebuffer.setPixel(x, y, makeColor(r, g, b));
                }
            }

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
