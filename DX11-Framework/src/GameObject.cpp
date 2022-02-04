#include "GameObject.h"

GameObject::GameObject(string type, Geometry geometry, Material material) : m_Geometry(geometry), m_Type(type), m_Material(material)
{
	m_Parent = nullptr;
	m_Position = XMFLOAT3();
	m_Rotation = XMFLOAT3();
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_TextureRV = nullptr;
}

GameObject::~GameObject()
{
}

void GameObject::Update(float t)
{
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	XMMATRIX rotation = XMMatrixRotationX(m_Rotation.x) * XMMatrixRotationY(m_Rotation.y) * XMMatrixRotationZ(m_Rotation.z);
	XMMATRIX translation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

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
