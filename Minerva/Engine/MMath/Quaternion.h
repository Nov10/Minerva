#pragma once
#include "Vector3.h"
#include <iostream>

struct Quaternion {
    float w, x, y, z;

    Quaternion();
    Quaternion(float x, float y, float z, float w);

    Quaternion operator*(const Quaternion& q) const;
    Quaternion conjugate() const;
    float magnitude() const;
    Quaternion normalized() const;

    Vector3 RotateVector(const Vector3& v) const;
    Vector3 ToEulerAngles() const;
	Quaternion Inverse() const;
    std::string ToString();

    static Quaternion FromEulerAngles(float pitch, float yaw, float roll);
    static Quaternion FromEulerAngles(const Vector3& v);
    static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);
} typedef Quaternion;
