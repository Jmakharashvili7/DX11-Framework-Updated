#include "TPCamera.h"
#include "PlayerPawn.h"

TPCamera::TPCamera(XMFLOAT3 position, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth) :
	Camera( position, windowWidth, windowHeight, nearDepth, farDepth)
{
}

void TPCamera::UpdatePos(XMFLOAT3 pos)
{
	Camera::Update();

	m_Position.x = pos.x;
	m_Position.y = pos.y;
	m_Position.z = pos.z - m_Distance;

	// Set the look vector to point at the player pawn
	XMVECTOR Pos = XMLoadFloat3(&m_Position);
	XMVECTOR Pawn = XMLoadFloat3(&pos);
	XMVECTOR LookAt = Pawn - Pos;
	
	XMStoreFloat3(&m_LookVec, XMVector3Normalize(LookAt));
	UpdateViewMatrix();
}
