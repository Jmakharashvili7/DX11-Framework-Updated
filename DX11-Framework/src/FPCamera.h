#pragma once
#include "Camera.h"
class FP_Camera : public Camera
{
public:
	FP_Camera(XMFLOAT3 position, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	~FP_Camera() {}

	// Movement functions
	void HandleInput(const float dt) override;

	void Strafe(const float force) override;
	void Walk(const float force) override;

	void RotateP(const float angle) override;
	void RotateY(const float angle) override;
};

