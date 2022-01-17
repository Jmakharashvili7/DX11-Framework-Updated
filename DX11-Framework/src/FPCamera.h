#pragma once
#include "Camera.h"
class FP_Camera : public Camera
{
public:
	FP_Camera(XMFLOAT3 position, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	~FP_Camera() {}

	// Movement functions
	void Strafe(float force) override;
	void Walk(float force) override;

	void RotateP(float angle) override;
	void RotateY(float angle) override;
};

