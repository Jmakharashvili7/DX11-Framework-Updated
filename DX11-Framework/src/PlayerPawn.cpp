#include "PlayerPawn.h"

PlayerPawn::PlayerPawn(MeshData meshData, XMFLOAT3 position, UINT WindowHeight, UINT WindowWidth, FLOAT nearDepth, FLOAT farDepth) :
	BaseObjectOBJ(meshData), 
    m_Position(position), 
    m_RightVec{1.0f, 0.0f, 0.0f}, 
    m_LookVec{0.0f, 0.0f, 1.0f}, 
    m_UpVec{0.0f, 1.0f, 0.0f},
	m_FPCamera(new FP_Camera(position, WindowWidth, WindowHeight, nearDepth, farDepth)),
	m_TPCamera(new Camera(position, WindowWidth, WindowHeight, nearDepth, farDepth))
{
	m_CurrentCamera = m_FPCamera;
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
	
	XMStoreFloat4x4(&m_World, XMMatrixRotationX(m_RotationP));
}

void PlayerPawn::RotateYaw(float angle)
{ 
	m_RotationY += angle;

	XMStoreFloat4x4(&m_World, XMMatrixRotationY(m_RotationY));
}

void PlayerPawn::Render(XMMATRIX& worldMatrix, ConstantBuffer& buffer, ID3D11Buffer* constBuffer, ID3D11DeviceContext* deviceContext)
{
	// if we are using first person camera do not render the object
	if (!m_IsFPCamera)
	{
		BaseObjectOBJ::Render(worldMatrix, buffer, constBuffer, deviceContext);
	}
}

void PlayerPawn::Update()
{
	XMStoreFloat4x4(&m_World, XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z) * XMMatrixRotationY(m_RotationY) *
		XMMatrixRotationX(m_RotationP));

	m_FPCamera->SetPosition(m_Position);
	m_TPCamera->SetPosition(m_Position);
}
