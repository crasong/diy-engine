#pragma once
#include "vec3.h"
#include <cmath>

// 4x4 matrix for 3D transformations
// Column-major order (OpenGL style) - columns are stored contiguously
class mat4 {
#pragma region DATA
public:
    // Data stored in column-major order: m[column][row]
    // Layout: m[0][0-3] = column 0, m[1][0-3] = column 1, etc.
    float m[4][4];
#pragma endregion

#pragma region CONSTRUCTORS
public:
    // Default constructor - creates identity matrix
    mat4() {
        // Initialize to identity
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                m[col][row] = (col == row) ? 1.0f : 0.0f;
            }
        }
    }

    // Construct from diagonal value
    explicit mat4(float diagonal) {
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                m[col][row] = (col == row) ? diagonal : 0.0f;
            }
        }
    }

    // Construct from 16 values (in column-major order)
    mat4(float m00, float m01, float m02, float m03,
         float m10, float m11, float m12, float m13,
         float m20, float m21, float m22, float m23,
         float m30, float m31, float m32, float m33) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
        m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
    }
#pragma endregion

#pragma region OPERATORS
public:
    // Matrix multiplication
    mat4 operator*(const mat4& other) const {
        mat4 result(0.0f);
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                result.m[col][row] =
                    m[0][row] * other.m[col][0] +
                    m[1][row] * other.m[col][1] +
                    m[2][row] * other.m[col][2] +
                    m[3][row] * other.m[col][3];
            }
        }
        return result;
    }

    // Matrix-vector multiplication (transform a 3D point, w=1)
    vec3 operator*(const vec3& v) const {
        float w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3];
        return vec3(
            (m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0]) / w,
            (m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1]) / w,
            (m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]) / w
        );
    }

    // Compound assignment
    mat4& operator*=(const mat4& other) {
        *this = *this * other;
        return *this;
    }
#pragma endregion

#pragma region STATIC_FACTORY_METHODS
public:
    // Create identity matrix
    static mat4 identity() {
        return mat4();
    }

    // Create translation matrix
    static mat4 translate(const vec3& v) {
        mat4 result;
        result.m[3][0] = v.x;
        result.m[3][1] = v.y;
        result.m[3][2] = v.z;
        return result;
    }

    static mat4 translate(float x, float y, float z) {
        return translate(vec3(x, y, z));
    }

    // Create scale matrix
    static mat4 scale(const vec3& v) {
        mat4 result(0.0f);
        result.m[0][0] = v.x;
        result.m[1][1] = v.y;
        result.m[2][2] = v.z;
        result.m[3][3] = 1.0f;
        return result;
    }

    static mat4 scale(float x, float y, float z) {
        return scale(vec3(x, y, z));
    }

    static mat4 scale(float s) {
        return scale(vec3(s, s, s));
    }

    // Create rotation matrix around X axis (angle in radians)
    static mat4 rotateX(float angle) {
        mat4 result;
        float c = std::cos(angle);
        float s = std::sin(angle);
        result.m[1][1] = c;
        result.m[1][2] = s;
        result.m[2][1] = -s;
        result.m[2][2] = c;
        return result;
    }

    // Create rotation matrix around Y axis (angle in radians)
    static mat4 rotateY(float angle) {
        mat4 result;
        float c = std::cos(angle);
        float s = std::sin(angle);
        result.m[0][0] = c;
        result.m[0][2] = -s;
        result.m[2][0] = s;
        result.m[2][2] = c;
        return result;
    }

    // Create rotation matrix around Z axis (angle in radians)
    static mat4 rotateZ(float angle) {
        mat4 result;
        float c = std::cos(angle);
        float s = std::sin(angle);
        result.m[0][0] = c;
        result.m[0][1] = s;
        result.m[1][0] = -s;
        result.m[1][1] = c;
        return result;
    }

    // Create rotation matrix around arbitrary axis (angle in radians)
    static mat4 rotate(float angle, const vec3& axis) {
        mat4 result;
        vec3 a = axis.normalized();
        float c = std::cos(angle);
        float s = std::sin(angle);
        float t = 1.0f - c;

        result.m[0][0] = c + a.x * a.x * t;
        result.m[1][1] = c + a.y * a.y * t;
        result.m[2][2] = c + a.z * a.z * t;

        float tmp1 = a.x * a.y * t;
        float tmp2 = a.z * s;
        result.m[1][0] = tmp1 + tmp2;
        result.m[0][1] = tmp1 - tmp2;

        tmp1 = a.x * a.z * t;
        tmp2 = a.y * s;
        result.m[2][0] = tmp1 - tmp2;
        result.m[0][2] = tmp1 + tmp2;

        tmp1 = a.y * a.z * t;
        tmp2 = a.x * s;
        result.m[2][1] = tmp1 + tmp2;
        result.m[1][2] = tmp1 - tmp2;

        return result;
    }

    // Create perspective projection matrix
    static mat4 perspective(float fovY, float aspect, float near, float far) {
        mat4 result(0.0f);
        float tanHalfFov = std::tan(fovY / 2.0f);

        result.m[0][0] = 1.0f / (aspect * tanHalfFov);
        result.m[1][1] = 1.0f / tanHalfFov;
        result.m[2][2] = -(far + near) / (far - near);
        result.m[2][3] = -1.0f;
        result.m[3][2] = -(2.0f * far * near) / (far - near);

        return result;
    }

    // Create orthographic projection matrix
    static mat4 ortho(float left, float right, float bottom, float top, float near, float far) {
        mat4 result;

        result.m[0][0] = 2.0f / (right - left);
        result.m[1][1] = 2.0f / (top - bottom);
        result.m[2][2] = -2.0f / (far - near);
        result.m[3][0] = -(right + left) / (right - left);
        result.m[3][1] = -(top + bottom) / (top - bottom);
        result.m[3][2] = -(far + near) / (far - near);

        return result;
    }

    // Create look-at view matrix
    static mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
        vec3 f = (center - eye).normalized();
        vec3 s = f.cross(up).normalized();
        vec3 u = s.cross(f);

        mat4 result;
        result.m[0][0] = s.x;
        result.m[1][0] = s.y;
        result.m[2][0] = s.z;
        result.m[0][1] = u.x;
        result.m[1][1] = u.y;
        result.m[2][1] = u.z;
        result.m[0][2] = -f.x;
        result.m[1][2] = -f.y;
        result.m[2][2] = -f.z;
        result.m[3][0] = -s.dot(eye);
        result.m[3][1] = -u.dot(eye);
        result.m[3][2] = f.dot(eye);

        return result;
    }
#pragma endregion

#pragma region FUNCTIONS
public:
    // Transpose the matrix
    mat4 transposed() const {
        mat4 result;
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                result.m[col][row] = m[row][col];
            }
        }
        return result;
    }

    // Get pointer to data (useful for OpenGL)
    const float* data() const {
        return &m[0][0];
    }

    float* data() {
        return &m[0][0];
    }
#pragma endregion
};
