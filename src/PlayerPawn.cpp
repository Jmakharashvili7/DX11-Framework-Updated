#include "PlayerPawn.h"

PlayerPawn::PlayerPawn(MeshData meshData) : BaseObjectOBJ(meshData), m_Position{0.0f, 0.0f, 0.0f}
{
}

PlayerPawn::~PlayerPawn()
{
}

void PlayerPawn::Walk(float deltaTime)
{
	m_Position.z += deltaTime * m_Speed;
}

void PlayerPawn::Strafe(float deltaTime)
{
	m_Position.x += deltaTime * m_Speed;
}

void PlayerPawn::RotatePitch(float angle)
{
}

void PlayerPawn::RotateYaw(float angle)
{ 
}

void PlayerPawn::Update()
{
	XMStoreFloat4x4(&m_World, XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z) * XMMatrixRotationY(m_RotationY) *
		XMMatrixRotationX(m_RotationP));
}
