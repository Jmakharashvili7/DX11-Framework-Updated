#include "PlayerPawn.h"

PlayerPawn::PlayerPawn(MeshData meshData, XMFLOAT3 position, UINT WindowHeight, UINT WindowWidth, FLOAT nearDepth, FLOAT farDepth) :
	BaseObjectOBJ(meshData), 
    m_Position(position), 
    m_RightVec{1.0f, 0.0f, 0.0f}, 
    m_LookVec{0.0f, 0.0f, 1.0f}, 
    m_UpVec{0.0f, 1.0f, 0.0f},
	m_FPCamera(new FP_Camera(position, WindowWidth, WindowHeight, nearDepth, farDepth)),
	m_TPCamera(new TPCamera(position, WindowWidth, WindowHeight, nearDepth, farDepth))
{
	m_CurrentCamera = m_TPCamera;
}

PlayerPawn::~PlayerPawn()
{
	// delete third person camera
	delete m_TPCamera;
	m_TPCamera = nullptr;

	// delete first person camera
	delete m_FPCamera;
	m_FPCamera = nullptr;
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
	m_RotationP += angle;
}

void PlayerPawn::RotateYaw(float angle)
{ 
	m_RotationY += angle;
}

void PlayerPawn::Update()
{
	XMStoreFloat4x4(&m_World, XMMatrixRotationY(m_RotationY) * XMMatrixRotationX(m_RotationP) * 
		XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z));

	m_FPCamera->SetPosition(m_Position);
	m_TPCamera->UpdatePos(m_Position);
}
