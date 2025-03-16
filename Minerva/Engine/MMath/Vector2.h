#pragma once

struct Vector2
{
	float x;
	float y;

	Vector2(float x, float y);
	Vector2();
	float magnitude() const;
	Vector2 normalized() const;

} typedef Vector2;