#pragma once
#include "DirectXMath.h"

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

	XMFLOAT3 GetXMFLOAT3()
	{
		return XMFLOAT3(x, y, z);
	}
	
	XMVECTOR GetXMVECTOR()
	{
		XMFLOAT3 temp(x, y, z);
		return XMLoadFloat3(&temp);
	}

	Vector3 operator+(const Vector3& other)
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	
	void operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}

	Vector3 operator*(const float& scalar)
	{
		return Vector3(x - scalar, y - scalar, z - scalar);
	}

	Vector3 operator-(const Vector3& other)
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	void operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
};
