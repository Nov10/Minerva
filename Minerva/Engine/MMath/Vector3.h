#pragma once
#include <string>

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3(float x, float y, float z);
	Vector3();
	float sqrMagnitude() const;
	float magnitude() const;
	Vector3 normalized() const;
	std::string ToString() const;


	static float Dot(Vector3 l, Vector3 r);
	static Vector3 Cross(Vector3 v1, Vector3 v2);
	static Vector3 ElemetProduct(Vector3 l, Vector3 r);
	static Vector3 ElemetDivision(Vector3 l, Vector3 r);

} typedef Vector3;
Vector3 operator*(Vector3 l, float r);
Vector3 operator*(float l, Vector3 r);
Vector3 operator+(Vector3 l, Vector3 r);
Vector3 operator-(Vector3 l, Vector3 r);
Vector3 operator/(Vector3 l, float r);