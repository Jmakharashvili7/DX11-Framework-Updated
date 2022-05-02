#include "Vector3.h"

Vector3 operator*(const Vector3& lhs, const float& rhs)
{
	return Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

Vector3 operator*(const float lhs, const Vector3& rhs)
{
	return Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

Vector3 operator+(const Vector3& lhs, const float& rhs)
{
	return Vector3(lhs + rhs);
}

Vector3 operator-(const Vector3& lhs, const float& rhs)
{
	return Vector3(lhs - rhs);
}

Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Vector3 operator/(const Vector3& lhs, const float& rhs)
{
	return Vector3(lhs / rhs);
}
