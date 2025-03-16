#pragma once

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <string>
#include "Vector4.h"
#include "Vector3.h"

struct Matrix4x4 {
    float e11, e12, e13, e14;
    float e21, e22, e23, e24;
    float e31, e32, e33, e34;
    float e41, e42, e43, e44;

    // 생성자
    Matrix4x4(
        float e11 = 0, float e12 = 0, float e13 = 0, float e14 = 0,
        float e21 = 0, float e22 = 0, float e23 = 0, float e24 = 0,
        float e31 = 0, float e32 = 0, float e33 = 0, float e34 = 0,
        float e41 = 0, float e42 = 0, float e43 = 0, float e44 = 0)
        : e11(e11), e12(e12), e13(e13), e14(e14),
        e21(e21), e22(e22), e23(e23), e24(e24),
        e31(e31), e32(e32), e33(e33), e34(e34),
        e41(e41), e42(e42), e43(e43), e44(e44) {}

    // 문자열 변환
    std::string ToString() const {
        return std::to_string(e11) + ", " + std::to_string(e12) + ", " + std::to_string(e13) + ", " + std::to_string(e14) + "\n" +
            std::to_string(e21) + ", " + std::to_string(e22) + ", " + std::to_string(e23) + ", " + std::to_string(e24) + "\n" +
            std::to_string(e31) + ", " + std::to_string(e32) + ", " + std::to_string(e33) + ", " + std::to_string(e34) + "\n" +
            std::to_string(e41) + ", " + std::to_string(e42) + ", " + std::to_string(e43) + ", " + std::to_string(e44);
    }

    // 비교 연산자
    bool operator==(const Matrix4x4& other) const {
        return e11 == other.e11 && e12 == other.e12 && e13 == other.e13 && e14 == other.e14 &&
            e21 == other.e21 && e22 == other.e22 && e23 == other.e23 && e24 == other.e24 &&
            e31 == other.e31 && e32 == other.e32 && e33 == other.e33 && e34 == other.e34 &&
            e41 == other.e41 && e42 == other.e42 && e43 == other.e43 && e44 == other.e44;
    }

    bool operator!=(const Matrix4x4& other) const {
        return !(*this == other);
    }

    // 단항 음수 연산자
    Matrix4x4 operator-() const {
        return Matrix4x4(
            -e11, -e12, -e13, -e14,
            -e21, -e22, -e23, -e24,
            -e31, -e32, -e33, -e34,
            -e41, -e42, -e43, -e44
        );
    }

    // 이항 연산자
    Matrix4x4 operator+(const Matrix4x4& other) const {
        return Matrix4x4(
            e11 + other.e11, e12 + other.e12, e13 + other.e13, e14 + other.e14,
            e21 + other.e21, e22 + other.e22, e23 + other.e23, e24 + other.e24,
            e31 + other.e31, e32 + other.e32, e33 + other.e33, e34 + other.e34,
            e41 + other.e41, e42 + other.e42, e43 + other.e43, e44 + other.e44
        );
    }

    Matrix4x4 operator-(const Matrix4x4& other) const {
        return Matrix4x4(
            e11 - other.e11, e12 - other.e12, e13 - other.e13, e14 - other.e14,
            e21 - other.e21, e22 - other.e22, e23 - other.e23, e24 - other.e24,
            e31 - other.e31, e32 - other.e32, e33 - other.e33, e34 - other.e34,
            e41 - other.e41, e42 - other.e42, e43 - other.e43, e44 - other.e44
        );
    }

    Matrix4x4 operator*(float scalar) const {
        return Matrix4x4(
            e11 * scalar, e12 * scalar, e13 * scalar, e14 * scalar,
            e21 * scalar, e22 * scalar, e23 * scalar, e24 * scalar,
            e31 * scalar, e32 * scalar, e33 * scalar, e34 * scalar,
            e41 * scalar, e42 * scalar, e43 * scalar, e44 * scalar
        );
    }

    friend Matrix4x4 operator*(float scalar, const Matrix4x4& matrix) {
        return matrix * scalar;
    }

    Matrix4x4 operator*(const Matrix4x4& other) const {
        return Matrix4x4(
            e11 * other.e11 + e12 * other.e21 + e13 * other.e31 + e14 * other.e41,
            e11 * other.e12 + e12 * other.e22 + e13 * other.e32 + e14 * other.e42,
            e11 * other.e13 + e12 * other.e23 + e13 * other.e33 + e14 * other.e43,
            e11 * other.e14 + e12 * other.e24 + e13 * other.e34 + e14 * other.e44,

            e21 * other.e11 + e22 * other.e21 + e23 * other.e31 + e24 * other.e41,
            e21 * other.e12 + e22 * other.e22 + e23 * other.e32 + e24 * other.e42,
            e21 * other.e13 + e22 * other.e23 + e23 * other.e33 + e24 * other.e43,
            e21 * other.e14 + e22 * other.e24 + e23 * other.e34 + e24 * other.e44,

            e31 * other.e11 + e32 * other.e21 + e33 * other.e31 + e34 * other.e41,
            e31 * other.e12 + e32 * other.e22 + e33 * other.e32 + e34 * other.e42,
            e31 * other.e13 + e32 * other.e23 + e33 * other.e33 + e34 * other.e43,
            e31 * other.e14 + e32 * other.e24 + e33 * other.e34 + e34 * other.e44,

            e41 * other.e11 + e42 * other.e21 + e43 * other.e31 + e44 * other.e41,
            e41 * other.e12 + e42 * other.e22 + e43 * other.e32 + e44 * other.e42,
            e41 * other.e13 + e42 * other.e23 + e43 * other.e33 + e44 * other.e43,
            e41 * other.e14 + e42 * other.e24 + e43 * other.e34 + e44 * other.e44
        );
    }

    Vector4 operator*(const Vector4& vector) const {
        return Vector4(
            vector.x * e11 + vector.y * e12 + vector.z * e13 + vector.w * e14,
            vector.x * e21 + vector.y * e22 + vector.z * e23 + vector.w * e24,
            vector.x * e31 + vector.y * e32 + vector.z * e33 + vector.w * e34,
            vector.x * e41 + vector.y * e42 + vector.z * e43 + vector.w * e44
        );
    }

    // Perspective Transform (Transform)
    Vector3 transform(const Vector3& v) const {
        float w = v.x * e41 + v.y * e42 + v.z * e43 + e44;
        if (w == 0) {
            throw std::invalid_argument("Division by zero during perspective transformation");
        }
        return Vector3(
            v.x * e11 + v.y * e12 + v.z * e13 + e14,
            v.x * e21 + v.y * e22 + v.z * e23 + e24,
            v.x * e31 + v.y * e32 + v.z * e33 + e34
        ) / w;
    }

    // 전치 행렬
    Matrix4x4 Transpose() const {
        return Matrix4x4(
            e11, e21, e31, e41,
            e12, e22, e32, e42,
            e13, e23, e33, e43,
            e14, e24, e34, e44
        );
    }

    // 행렬식
    float Determinant() const {
        return e11 * (e22 * (e33 * e44 - e34 * e43) - e32 * (e23 * e44 - e24 * e43) + e42 * (e23 * e34 - e24 * e33)) -
            e12 * (e21 * (e33 * e44 - e34 * e43) - e31 * (e23 * e44 - e24 * e43) + e41 * (e23 * e34 - e24 * e33)) +
            e13 * (e21 * (e32 * e44 - e34 * e42) - e31 * (e22 * e44 - e24 * e42) + e41 * (e22 * e34 - e24 * e32)) -
            e14 * (e21 * (e32 * e43 - e33 * e42) - e31 * (e22 * e43 - e23 * e42) + e41 * (e22 * e33 - e23 * e32));
    }

    // 역행렬
    Matrix4x4 Inverse() const {
        float det = Determinant();
        if (std::abs(det) < std::numeric_limits<float>::epsilon())
            throw std::invalid_argument("This matrix is non-invertible.");

        float invDet = 1.0f / det;
        // 역행렬 계산 구현 (생략 가능)
        return *this; // 임시 반환
    }
} typedef Matrix4x4;