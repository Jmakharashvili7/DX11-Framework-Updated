#include "Transform.h"

Transform::Transform(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale) :
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
