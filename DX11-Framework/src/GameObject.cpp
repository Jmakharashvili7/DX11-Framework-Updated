#include "GameObject.h"

GameObject::GameObject(string type, Geometry geometry, Material material) :
	m_Geometry(geometry),
	m_Type(type),
	m_Material(material),
	m_Transform(new Transform(XMFLOAT3(0,0,0), XMFLOAT3(0,0,0), XMFLOAT3(1,1,1)))
{
	m_Parent = nullptr;
	m_TextureRV = nullptr;
}

GameObject::~GameObject()
{
	delete m_Transform;
}

void GameObject::Update(float t)
{
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(m_Transform->GetScale()->x, m_Transform->GetScale()->y, m_Transform->GetScale()->z);
	XMMATRIX rotation = XMMatrixRotationX(m_Transform->GetRotation()->x) * XMMatrixRotationY(m_Transform->GetRotation()->y) * XMMatrixRotationZ(m_Transform->GetRotation()->z);
	XMMATRIX translation = XMMatrixTranslation(m_Transform->GetPosition()->x, m_Transform->GetPosition()->y, m_Transform->GetPosition()->z);

	XMStoreFloat4x4(&m_World, scale * rotation * translation);

	if (m_Parent != nullptr)
	{
		XMStoreFloat4x4(&m_World, this->GetWorldMatrix() * m_Parent->GetWorldMatrix());
	}
}

void GameObject::Draw(XMMATRIX& worldMatrix, ConstantBuffer& buffer, ID3D11Buffer* constBuffer, ID3D11DeviceContext* immediateContext)
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	worldMatrix = XMLoadFloat4x4(&m_World);
	buffer.mWorld = XMMatrixTranspose(worldMatrix);

	immediateContext->IASetVertexBuffers(0, 1, &m_Geometry.vertexBuffer, &m_Geometry.vertexBufferStride, &m_Geometry.vertexBufferOffset);
	immediateContext->IASetIndexBuffer(m_Geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	immediateContext->UpdateSubresource(constBuffer, 0, nullptr, &buffer, 0, 0);

	immediateContext->DrawIndexed(m_Geometry.indexCount, 0, 0);
}
