#include "framebuffer.h"
#include <algorithm>

Framebuffer::Framebuffer(int width, int height)
    : width(width), height(height) {
    pixels.resize(width * height, 0xFF000000);  // Default: black
}

void Framebuffer::setPixel(int x, int y, uint32_t color) {
    if (isInBounds(x, y)) {
        pixels[y * width + x] = color;
    }
}

uint32_t Framebuffer::getPixel(int x, int y) const {
    if (isInBounds(x, y)) {
        return pixels[y * width + x];
    }
    return 0;
}

void Framebuffer::clear(uint32_t color) {
    std::fill(pixels.begin(), pixels.end(), color);
}

bool Framebuffer::isInBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}
