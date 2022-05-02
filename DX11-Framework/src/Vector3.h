#pragma once
#include "DirectXMath.h"
#include "Log.h"
#include <math.h>

using namespace DirectX;

struct Vector3
{
	float x, y, z;

	Vector3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3(XMVECTOR vector)
	{
		XMFLOAT3 temp;
		XMStoreFloat3(&temp, vector);

		x = temp.x;
		y = temp.y;
		z = temp.z;
	}

	Vector3(XMFLOAT3 vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
	}

	float GetMagnitude()
	{
		return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
	}

	Vector3 GetUnitVec()
	{
		float mag = GetMagnitude();
		return Vector3(x / mag, y / mag, z / mag);
	}

	XMFLOAT3 GetXMFLOAT3()
	{
		return XMFLOAT3(x, y, z);
	}
	
	XMVECTOR GetXMVECTOR()
	{
		XMFLOAT3 temp(x, y, z);
		return XMLoadFloat3(&temp);
	}

	void Zero()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vector3 GetDirection()
	{
		assert(magnitude != 0);
		return Vector3(x / magnitude, y / magnitude, z / magnitude);
	}

	void Normalize()
	{
		assert(magnitude != 0);

		this->x = x / magnitude;
		this->y = y / magnitude;
		this->z = z / magnitude;
	}

	float Dot(Vector3 other)
	{
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}

	float DistanceSquared(Vector3 other)
	{
		return std::pow(x - other.x, 2) + std::pow(y - other.y, 2) + std::pow(z - other.z, 2);

	}

	float Distance(Vector3 other)
	{
		float distanceSqrd = DistanceSquared(other);
		return std::sqrt(distanceSqrd);
	}

	Vector3 operator+(const Vector3& other)
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator+(const float& other)
	{
		return Vector3(x + other, y + other, z + other);
	}
	
	void operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}

	void operator+=(const float& other)
	{
		x += other;
		y += other;
		z += other;
	}

	Vector3 operator-(const Vector3& other)
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 operator-(const float& scalar)
	{
		return Vector3(x - scalar, y - scalar, z - scalar);
	}

	void operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	void operator-=(const float& scalar)
	{
		x -= scalar;
		y -= scalar;
		z -= scalar;
	}

	Vector3 operator*(const Vector3& other)
	{
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	Vector3 operator*(const float& scalar)
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	void operator*=(const Vector3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
	}

	void operator*=(const float& scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	Vector3 operator/(const Vector3& other)
	{
		assert(other.x != 0 && other.y != 0 && other.z != 0);
		return Vector3(x / other.x, y / other.y, z / other.z);
	}

	Vector3 operator/(const float& scalar)
	{
		assert(scalar != 0);
		return Vector3(x / scalar, y / scalar, z / scalar);
	}

	void operator/=(const Vector3& other)
	{
		assert(other.x != 0 && other.y != 0 && other.z != 0);

		x /= other.x;
		y /= other.y;
		z /= other.z;
	}
	void operator/=(const float& scalar)
	{
		assert(scalar != 0);

		x /= scalar;
		y /= scalar;
		z /= scalar;
	}
};

Vector3 operator*(const Vector3& lhs, const float& rhs);
Vector3 operator*(const float lhs, const Vector3& rhs);
Vector3 operator+(const Vector3& lhs, const float& rhs);
Vector3 operator-(const Vector3& lhs, const float& rhs);
Vector3 operator/(const Vector3& lhs, const float& rhs);