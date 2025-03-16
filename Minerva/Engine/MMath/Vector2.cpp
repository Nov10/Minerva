#include "Vector2.h"
#include <math.h>

Vector2::Vector2(float x, float y) : x(x), y(y)
{

}
Vector2::Vector2() : x(0), y(0)
{

}
float Vector2::magnitude() const
{
	return sqrt(x * x + y * y);
}
Vector2 Vector2::normalized() const
{
	float invM = 1.0f / magnitude();
	return Vector2(x * invM, y* invM);
}