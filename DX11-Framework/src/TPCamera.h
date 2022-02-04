#pragma once
#include "Camera.h"

class PlayerPawn;

class TPCamera : public Camera
{
private:
	float m_Distance = 3.0f;
public:
	TPCamera(XMFLOAT3 position, FLOAT windowWidth, FLOAT windowHeight, 
	FLOAT nearDepth, FLOAT farDepth);
	~TPCamera() {}

	void UpdatePos(XMFLOAT3 pos);
};

