#pragma once
#include <cstdint>
#include <algorithm>

// Simple color class with floating point components [0.0 - 1.0]
class color {
#pragma region DATA
public:
    float r, g, b, a;
#pragma endregion

#pragma region CONSTRUCTORS
public:
    // Default constructor - black with full alpha
    color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

    // Construct from float components [0.0 - 1.0]
    color(float r, float g, float b, float a = 1.0f)
        : r(r), g(g), b(b), a(a) {}

    // Construct from uint8_t components [0 - 255]
    static color fromBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        return color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    // Construct from packed uint32_t (ARGB8888 format)
    static color fromUint32(uint32_t packed) {
        return color(
            ((packed >> 16) & 0xFF) / 255.0f,  // R
            ((packed >> 8) & 0xFF) / 255.0f,   // G
            (packed & 0xFF) / 255.0f,          // B
            ((packed >> 24) & 0xFF) / 255.0f   // A
        );
    }
#pragma endregion

#pragma region OPERATORS
public:
    // Color arithmetic
    color operator+(const color& other) const {
        return color(r + other.r, g + other.g, b + other.b, a + other.a);
    }

    color operator-(const color& other) const {
        return color(r - other.r, g - other.g, b - other.b, a - other.a);
    }

    color operator*(float scalar) const {
        return color(r * scalar, g * scalar, b * scalar, a * scalar);
    }

    color operator*(const color& other) const {
        return color(r * other.r, g * other.g, b * other.b, a * other.a);
    }

    color& operator+=(const color& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        return *this;
    }

    color& operator*=(float scalar) {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        a *= scalar;
        return *this;
    }
#pragma endregion

#pragma region CONVERSION
public:
    // Convert to packed uint32_t (ARGB8888 format) for framebuffer
    uint32_t toUint32() const {
        uint8_t ri = static_cast<uint8_t>(std::clamp(r, 0.0f, 1.0f) * 255.0f);
        uint8_t gi = static_cast<uint8_t>(std::clamp(g, 0.0f, 1.0f) * 255.0f);
        uint8_t bi = static_cast<uint8_t>(std::clamp(b, 0.0f, 1.0f) * 255.0f);
        uint8_t ai = static_cast<uint8_t>(std::clamp(a, 0.0f, 1.0f) * 255.0f);
        return (ai << 24) | (ri << 16) | (gi << 8) | bi;
    }

    // Clamp color components to [0.0 - 1.0]
    color clamped() const {
        return color(
            std::clamp(r, 0.0f, 1.0f),
            std::clamp(g, 0.0f, 1.0f),
            std::clamp(b, 0.0f, 1.0f),
            std::clamp(a, 0.0f, 1.0f)
        );
    }

    void clamp() {
        r = std::clamp(r, 0.0f, 1.0f);
        g = std::clamp(g, 0.0f, 1.0f);
        b = std::clamp(b, 0.0f, 1.0f);
        a = std::clamp(a, 0.0f, 1.0f);
    }
#pragma endregion

#pragma region STATIC_COLORS
public:
    // Common colors
    static color black()   { return color(0.0f, 0.0f, 0.0f, 1.0f); }
    static color white()   { return color(1.0f, 1.0f, 1.0f, 1.0f); }
    static color red()     { return color(1.0f, 0.0f, 0.0f, 1.0f); }
    static color green()   { return color(0.0f, 1.0f, 0.0f, 1.0f); }
    static color blue()    { return color(0.0f, 0.0f, 1.0f, 1.0f); }
    static color yellow()  { return color(1.0f, 1.0f, 0.0f, 1.0f); }
    static color cyan()    { return color(0.0f, 1.0f, 1.0f, 1.0f); }
    static color magenta() { return color(1.0f, 0.0f, 1.0f, 1.0f); }
    static color gray()    { return color(0.5f, 0.5f, 0.5f, 1.0f); }
#pragma endregion
};

// Non-member operator for scalar * color
inline color operator*(float scalar, const color& c) {
    return c * scalar;
}

// Linear interpolation between two colors
inline color lerp(const color& a, const color& b, float t) {
    return color(
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
        a.a + (b.a - a.a) * t
    );
}