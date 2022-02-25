#include "GameObject.h"
#include "Transform.h"

GameObject::GameObject(string type, Geometry geometry, Material* material) :
	m_Type(type),
	m_Appearance(new Appearance(material, geometry)),
	m_Transform(new Transform(nullptr, XMFLOAT3(0,0,0), XMFLOAT3(0,0,0), XMFLOAT3(1,1,1)))
{
}

GameObject::~GameObject()
{
	if (m_Transform) delete m_Transform;
	//if (m_Appearance) delete m_Appearance;
}

void GameObject::Update(float t)
{
	m_Transform->Update();
}

void GameObject::Draw(XMMATRIX& worldMatrix, ConstantBuffer& buffer, ID3D11Buffer* constBuffer, ID3D11DeviceContext* immediateContext)
{
	worldMatrix = XMLoadFloat4x4(m_Transform->GetWorld());
	buffer.mWorld = XMMatrixTranspose(worldMatrix);

	m_Appearance->Draw(buffer, constBuffer, immediateContext);
}
