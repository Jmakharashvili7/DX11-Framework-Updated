#pragma once
#include "Camera.h"
class FP_Camera : public Camera
{
public:
	FP_Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, XMFLOAT3 right, FLOAT windowWidth, FLOAT windowHeight, 
		FLOAT nearDepth, FLOAT farDepth);
	~FP_Camera() {}

	// Movement functions
	void Strafe(float force);
	void Walk(float force);
};

