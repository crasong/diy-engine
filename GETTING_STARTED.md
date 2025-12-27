# Getting Started

Quick setup guide to start building your software renderer.

## Project Setup

### Option 1: C++ with SDL3 (Recommended for beginners)

#### Install Dependencies

**Linux (Ubuntu/Debian)**
```bash
sudo apt-get install build-essential cmake libsdl3-dev
```

**macOS**
```bash
brew install cmake sdl3
```

**Windows**
- Download SDL3 development libraries from https://www.libsdl.org/
- Or use vcpkg: `vcpkg install sdl3`

#### Project Structure
```
diy-engine/
├── src/
│   ├── main.cpp           # Entry point
│   ├── framebuffer.h/cpp  # Pixel buffer
│   ├── window.h/cpp       # SDL window wrapper
│   └── math/
│       ├── vector.h       # Vec2, Vec3, Vec4
│       └── matrix.h       # Mat4
├── assets/
│   ├── textures/          # Texture images
│   └── models/            # 3D models (.obj)
├── tests/                 # Unit tests
├── CMakeLists.txt         # Build configuration
└── README.md
```

#### Minimal CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.10)
project(DIYEngine)

set(CMAKE_CXX_STANDARD 17)

# Find SDL3
find_package(SDL3 REQUIRED)
include_directories(${SDL3_INCLUDE_DIRS})

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Executable
add_executable(renderer ${SOURCES})
target_link_libraries(renderer SDL3::SDL3)
```

#### Build and Run
```bash
mkdir build
cd build
cmake ..
make
./renderer
```

---

### Option 2: C++ with GLFW

Replace SDL3 with GLFW for a lighter alternative:
```bash
# Install GLFW
sudo apt-get install libglfw3-dev  # Linux
brew install glfw                   # macOS

# Update CMakeLists.txt
find_package(glfw3 REQUIRED)
target_link_libraries(renderer glfw)
```

---

### Option 3: Python with Pygame (Rapid Prototyping)

```bash
pip install pygame numpy pillow
```

```python
# main.py
import pygame
import numpy as np

WIDTH, HEIGHT = 800, 600

pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("DIY Renderer")

# Framebuffer as numpy array
framebuffer = np.zeros((HEIGHT, WIDTH, 3), dtype=np.uint8)

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Clear framebuffer
    framebuffer.fill(0)

    # TODO: Render here
    # Example: framebuffer[y, x] = [r, g, b]

    # Display framebuffer
    surf = pygame.surfarray.make_surface(framebuffer.swapaxes(0, 1))
    screen.blit(surf, (0, 0))
    pygame.display.flip()

pygame.quit()
```

---

## Phase 1: First Program (Hello Pixel!)

### Goal
Create a window, draw pixels, display a gradient.

### C++ Implementation (SDL3)

**main.cpp**
```cpp
#include <SDL3/SDL.h>
#include <vector>
#include <cstdint>

const int WIDTH = 800;
const int HEIGHT = 600;

class Framebuffer {
public:
    Framebuffer(int w, int h) : width(w), height(h) {
        pixels.resize(w * h, 0xFF000000);  // ARGB format
    }

    void setPixel(int x, int y, uint32_t color) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixels[y * width + x] = color;
        }
    }

    void clear(uint32_t color = 0xFF000000) {
        std::fill(pixels.begin(), pixels.end(), color);
    }

    uint32_t* data() { return pixels.data(); }

private:
    int width, height;
    std::vector<uint32_t> pixels;
};

uint32_t RGB(uint8_t r, uint8_t g, uint8_t b) {
    return 0xFF000000 | (r << 16) | (g << 8) | b;
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "DIY Renderer",
        WIDTH, HEIGHT,
        0  // default flags
    );

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    // Create framebuffer
    Framebuffer fb(WIDTH, HEIGHT);

    // Main loop
    bool running = true;
    SDL_Event event;

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear framebuffer
        fb.clear(RGB(0, 0, 0));

        // Draw gradient
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                uint8_t r = (x * 255) / WIDTH;
                uint8_t g = (y * 255) / HEIGHT;
                uint8_t b = 128;
                fb.setPixel(x, y, RGB(r, g, b));
            }
        }

        // Update texture and display
        SDL_UpdateTexture(texture, nullptr, fb.data(), WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // ~60 FPS
    }

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
```

### Compile and Run
```bash
g++ main.cpp -o renderer -lSDL3
./renderer
```

You should see a gradient window!

---

## Phase 2: Draw Your First Line

Add this to your code after the gradient:

```cpp
// Bresenham's line algorithm
void drawLine(Framebuffer& fb, int x0, int y0, int x1, int y1, uint32_t color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        fb.setPixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// In main loop, replace gradient with:
fb.clear(RGB(0, 0, 0));
drawLine(fb, 100, 100, 700, 500, RGB(255, 0, 0));
drawLine(fb, 700, 100, 100, 500, RGB(0, 255, 0));
```

You should see two colored lines crossing!

---

## Development Workflow

### 1. Incremental Development
- Implement one feature at a time
- Test after each addition
- Don't move forward until current phase works

### 2. Version Control
```bash
git init
git add .
git commit -m "Initial commit - Phase 1 complete"

# Create branch for each phase
git checkout -b phase-2-primitives
```

### 3. Testing
- Create simple test scenes
- Save screenshots for regression testing
- Use assertions for math operations

### 4. Debugging
```cpp
// Add debug output
void saveAsPPM(const Framebuffer& fb, const char* filename) {
    // Save framebuffer as .ppm image for debugging
}

// Add visualization helpers
void drawPoint(Framebuffer& fb, int x, int y, int size, uint32_t color) {
    for (int dy = -size; dy <= size; dy++) {
        for (int dx = -size; dx <= size; dx++) {
            fb.setPixel(x + dx, y + dy, color);
        }
    }
}
```

---

## Common First Issues

### Issue: Window appears but is black
- Check framebuffer initialization
- Verify setPixel() is being called
- Ensure texture update is happening

### Issue: Pixels appear in wrong locations
- Check coordinate system (y-down vs y-up)
- Verify width/height in calculations
- Check array indexing (y * width + x)

### Issue: Colors are wrong
- Verify pixel format (ARGB vs RGBA vs RGB)
- Check bit shifting in RGB() function
- Ensure values are 0-255 range

### Issue: Slow performance
- Optimize framebuffer access
- Use profiler to find bottlenecks
- Consider smaller resolution for testing

---

## Learning Resources

### Beginner-Friendly
- [Scratchapixel](https://www.scratchapixel.com/) - Excellent tutorials
- [Tiny Renderer](https://github.com/ssloy/tinyrenderer) - Similar project
- [3D Graphics in 500 lines](https://github.com/ssloy/tinyrenderer/wiki)

### Math
- [3Blue1Brown - Linear Algebra](https://www.youtube.com/playlist?list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab)
- [Essence of Calculus](https://www.youtube.com/playlist?list=PLZHQObOWTQDMsr9K-rj53DwVRMYO3t5Yr)

### Reference
- [Learn OpenGL](https://learnopengl.com/) - Concepts apply to software rendering
- [Real-Time Rendering](http://www.realtimerendering.com/)

---

## Next Steps

1. **Complete Phase 1** - Get pixels rendering
2. **Read ROADMAP.md** - Understand the full journey
3. **Read ACTION_PLAN.md** - Deep dive into algorithms
4. **Join communities** - r/GraphicsProgramming, Discord servers
5. **Share progress** - Blog, GitHub, social media

---

**Remember**: Every expert started exactly where you are now. Take it one pixel at a time!

Good luck! 🚀
