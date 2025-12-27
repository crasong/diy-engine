#pragma once

#include <cstdint>
#include <vector>

// Simple framebuffer - just a 2D array of pixels
// You'll implement all the rendering logic yourself!
class Framebuffer {
public:
    Framebuffer(int width, int height);

    // Basic pixel operations
    void setPixel(int x, int y, uint32_t color);
    uint32_t getPixel(int x, int y) const;
    void clear(uint32_t color = 0xFF000000);

    // Direct access to pixel data (for SDL)
    uint32_t* data() { return pixels.data(); }
    const uint32_t* data() const { return pixels.data(); }

    // Dimensions
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int width;
    int height;
    std::vector<uint32_t> pixels;  // ARGB8888 format

    bool isInBounds(int x, int y) const;
};

// Helper function to create ARGB color
inline uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
    return (a << 24) | (r << 16) | (g << 8) | b;
}

// Extract color components
inline uint8_t getRed(uint32_t color)   { return (color >> 16) & 0xFF; }
inline uint8_t getGreen(uint32_t color) { return (color >> 8) & 0xFF; }
inline uint8_t getBlue(uint32_t color)  { return color & 0xFF; }
inline uint8_t getAlpha(uint32_t color) { return (color >> 24) & 0xFF; }
