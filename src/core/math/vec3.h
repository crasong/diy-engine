#pragma once
#include <cmath>

class vec3 {
#pragma region DATA
public:
    float x, y, z;
#pragma endregion

#pragma region CONSTRUCTORS
public:
    vec3() : vec3(0.f) {}
    explicit vec3(float x) : vec3(x, x, x) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
#pragma endregion

#pragma region OPERATORS
public:
    vec3& operator=(const vec3& other) = default;

    vec3 operator+(vec3 other) const {
        return vec3(x + other.x, y + other.y, z + other.z);
    }
    vec3 operator-(vec3 other) const {
        return vec3(x - other.x, y - other.y, z - other.z);
    }
    vec3 operator*(float s) const {
        return vec3(x * s, y * s, z * s);
    }
    vec3 operator/(float s) const {
        return vec3(x / s, y / s, z / s);
    }
    vec3 operator-() const {
        return vec3(-x, -y, -z);
    }

    vec3& operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    vec3& operator/=(float s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }
    vec3& operator+=(const vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    vec3& operator-=(const vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
#pragma endregion

#pragma region FUNCTIONS
public:
    inline float lengthsquared() const {
        return x * x + y * y + z * z;
    }

    inline float length() const {
        return std::sqrt(lengthsquared());
    }

    vec3 normalized() const {
        const float len = length();
        if (len > 0.f) {
            return vec3(x/len, y/len, z/len);
        }
        return vec3();
    }

    vec3& normalize() {
        const float len = length();
        if (len > 0.f) {
            x /= len;
            y /= len;
            z /= len;
        }
        return *this;
    }

    vec3 cross(const vec3& other) const {
        return vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    float dot(const vec3& other) const {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }
#pragma endregion
};

// Non-member operators for scalar multiplication (reversed order)
inline vec3 operator*(float s, vec3 v) {
    return v * s;
}