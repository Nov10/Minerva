#include "Vector4.h"

Vector4::Vector4() : x(0), y(0), z(0), w(0) {}
Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

float Vector4::magnitude() const {
	return std::sqrt(sqrMagnitude());
}

float Vector4::sqrMagnitude() const {
	return x * x + y * y + z * z + w * w;
}

Vector4 Vector4::normalized() const {
	float mag = magnitude();
	return *this / mag;
}

float Vector4::dot(const Vector4& v1, const Vector4& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

std::string Vector4::ToString() const {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
		std::to_string(z) + ", " + std::to_string(w) + ")";
}

Vector4 operator+(const Vector4& l, const Vector4& r) { return Vector4(l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w); }
Vector4 operator-(const Vector4& l, const Vector4& r) { return Vector4(l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w); }
Vector4 operator*(const Vector4& v, float scalar) { return Vector4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar); }
Vector4 operator*(float scalar, const Vector4& v) { return v * scalar; }
Vector4 operator/(const Vector4& v, float scalar) { return v * (1.0f / scalar); }