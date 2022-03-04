#include "Appearance.h"

Appearance::Appearance(Material* material, Geometry geometry) : 
	m_material(material),
	m_geometry(new Geometry(geometry)),
	m_textureRV(nullptr)
{
}

Appearance::~Appearance()
{
	if (m_material) m_material;
	if (m_textureRV) m_textureRV->Release();
}

void Appearance::Draw(ConstantBuffer& buffer, ID3D11Buffer* constBuffer, ID3D11DeviceContext* immediateContext)
{
	ID3D11ShaderResourceView* texture = m_textureRV;
	immediateContext->PSSetShaderResources(0, 1, &texture);

	immediateContext->IASetVertexBuffers(0, 1, &m_geometry->vertexBuffer, &m_geometry->vertexBufferStride, &m_geometry->vertexBufferOffset);

	immediateContext->IASetIndexBuffer(m_geometry->indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	immediateContext->UpdateSubresource(constBuffer, 0, nullptr, &buffer, 0, 0);
	immediateContext->DrawIndexed(m_geometry->indexCount, 0, 0);
}
