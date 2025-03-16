#pragma once
#include "Vector3.h"
#include <iostream>

struct Quaternion {
    float w, x, y, z;

    Quaternion();
    Quaternion(float x, float y, float z, float w);

    // Quaternion operations
    Quaternion operator*(const Quaternion& q) const;
    Quaternion conjugate() const;
    float magnitude() const;
    Quaternion normalized() const;

    // Euler angles to Quaternion
    static Quaternion FromEulerAngles(float pitch, float yaw, float roll);
    static Quaternion FromEulerAngles(const Vector3& v);

    // Quaternion to Euler angles
    Vector3 ToEulerAngles() const;

    // Linear interpolation (LERP)
    static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);

    std::string ToString();
    Vector3 RotateVector(const Vector3& v) const;
} typedef Quaternion;
