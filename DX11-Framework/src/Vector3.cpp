#include "Vector3.h"

Vector3 operator*(const Vector3& lhs, const float& rhs)
{
	return Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

Vector3 operator+(const Vector3& lhs, const float& rhs)
{
	return Vector3(lhs + rhs);
}

Vector3 operator-(const Vector3& lhs, const float& rhs)
{
	return Vector3(lhs - rhs);
}

Vector3 operator/(const Vector3& lhs, const float& rhs)
{
	return Vector3(lhs / rhs);
}
