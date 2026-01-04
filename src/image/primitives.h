#pragma once
#include "image/color.h"
#include "core/framebuffer.h"
#include <cmath>

static void DrawLine(int x0, int y0, int x1, int y1, color color, Framebuffer& framebuffer) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int err = dx - dy;
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    while (true) {
        framebuffer.setPixel(x0, y0, color.toUint32());
        
        // Have we reached the destination?
        if ((x0 == x1) && (y0 == y1)) break;

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

static void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color color, Framebuffer& framebuffer) {
    DrawLine(x0, y0, x1, y1, color, framebuffer);
    DrawLine(x1, y1, x2, y2, color, framebuffer);
    DrawLine(x2, y2, x0, y0, color, framebuffer);
}

static void FillWithGradient(Framebuffer& framebuffer) {
    const int windowHeight = framebuffer.getHeight();
    const int windowWidth = framebuffer.getWidth();
    for (int y = 0; y < windowHeight; y++) {
        for (int x = 0; x < windowWidth; x++) {
            uint8_t r = (x * 255) / windowWidth;
            uint8_t g = (y * 255) / windowHeight;
            uint8_t b = 128;
            framebuffer.setPixel(x, y, makeColor(r, g, b));
        }
    }
}