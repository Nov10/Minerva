#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <stdexcept>

struct Vector4 {
    float x, y, z, w;

    Vector4();
    Vector4(float x, float y, float z, float w);

    static Vector4 zero() {
        return Vector4(0, 0, 0, 0);
    }
    static Vector4 one() {
        return Vector4(1, 1, 1, 1);
    }

    float magnitude() const;
    float sqrMagnitude() const;
    Vector4 normalized() const;

    static float dot(const Vector4& v1, const Vector4& v2);

    std::string ToString() const;
} typedef Vector4;

Vector4 operator+(const Vector4& l, const Vector4& r);
Vector4 operator-(const Vector4& l, const Vector4& r);
Vector4 operator*(const Vector4& v, float scalar);
Vector4 operator*(float scalar, const Vector4& v);
Vector4 operator/(const Vector4& v, float scalar);