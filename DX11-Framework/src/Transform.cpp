#include "Transform.h"

Transform::Transform()
	:m_Parent(nullptr),
	m_Position(new Vector3(0,0,0)),
	m_Rotation(new Vector3(0,0,0)),
	m_Scale(new Vector3(0,0,0))
{
}

Transform::Transform(GameObject* parent, Vector3 position, Vector3 rotation, Vector3 scale) :
	m_Parent(nullptr),
	m_Position(new Vector3(position)),
	m_Rotation(new Vector3(rotation)),
	m_Scale(new Vector3(scale))
{
}

Transform::~Transform()
{
	delete m_Position;
	delete m_Rotation;
	delete m_Scale;
}

void Transform::Update()
{
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(m_Scale->x, m_Scale->y, m_Scale->z);
	XMMATRIX rotation = XMMatrixRotationX(m_Rotation->x) * XMMatrixRotationY(m_Rotation->y) * XMMatrixRotationZ(m_Rotation->z);
	XMMATRIX translation = XMMatrixTranslation(m_Position->x, m_Position->y, m_Position->z);

	XMStoreFloat4x4(&m_World, scale * rotation * translation);

	if (m_Parent != nullptr)
	{
		XMMATRIX matrix = XMLoadFloat4x4(m_Parent->GetTransform()->GetWorldConst());
		XMMATRIX matrix2 = XMLoadFloat4x4(this->GetWorldConst());
		XMStoreFloat4x4(&m_World, matrix * matrix2);
	}
}

void Transform::Draw(ConstantBuffer& buffer)
{
	XMMATRIX worldMatrix = XMLoadFloat4x4(&m_World);
	buffer.mWorld = XMMatrixTranspose(worldMatrix);
}
