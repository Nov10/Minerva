#include "Vector3.h"
#include <math.h>
#include <string>
 
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
Vector3::Vector3() : x(0), y(0), z(0) {}

float Vector3::sqrMagnitude() const
{
	return x * x + y * y + z * z;
}

float Vector3::magnitude() const
{
	return sqrt(sqrMagnitude());
}
Vector3 Vector3::normalized() const
{
	return Vector3(x, y, z) / magnitude();
}

std::string Vector3::ToString() const
{
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
		std::to_string(z) + ")";
}

float Vector3::Dot(Vector3 l, Vector3 r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
}
Vector3 Vector3::Cross(Vector3 v1, Vector3 v2)
{
	return Vector3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

Vector3 Vector3::ElemetProduct(Vector3 l, Vector3 r)
{
	return Vector3(l.x * r.x, l.y * r.y, l.z * r.z);
}

Vector3 Vector3::ElemetDivision(Vector3 l, Vector3 r)
{
	return Vector3(l.x / r.x, l.y / r.y, l.z / r.z);
}

Vector3 operator*(Vector3 l, float r) { return Vector3(l.x * r, l.y * r, l.z * r); }
Vector3 operator*(float l, Vector3 r) { return r * l; }
Vector3 operator+(Vector3 l, Vector3 r) { return Vector3(l.x + r.x, l.y + r.y, l.z + r.z); }
Vector3 operator-(Vector3 l, Vector3 r) { return Vector3(l.x - r.x, l.y - r.y, l.z - r.z); }
Vector3 operator/(Vector3 l, float r) { return l * (1.0f / r); }