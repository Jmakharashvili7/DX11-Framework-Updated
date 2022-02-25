#include "Transform.h"

Transform::Transform(GameObject* parent, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale) :
	m_Parent(nullptr),
	m_Position(new XMFLOAT3(position.x, position.y, position.z)),
	m_Rotation(new XMFLOAT3(rotation.x, rotation.y, rotation.z)),
	m_Scale(new XMFLOAT3(scale.x, scale.y, scale.z))
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
		XMMATRIX matrix = XMLoadFloat4x4(m_Parent->GetTransform()->GetWorld());
		XMMATRIX matrix2 = XMLoadFloat4x4(this->GetWorld());
		XMStoreFloat4x4(&m_World, matrix * matrix2);
	}
}
