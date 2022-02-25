#pragma once
#include <d3d11_1.h>
#include "structs.h"

class Appearance
{
private:
	Material* m_material;
	ID3D11ShaderResourceView* m_textureRV;
	Geometry* m_geometry;
public:
	Appearance(Material* material, Geometry geometry);
	~Appearance();

	void Draw(ConstantBuffer& buffer, ID3D11Buffer* constBuffer, ID3D11DeviceContext* immediateContext);
	inline ID3D11ShaderResourceView* GetTextureRV() { return m_textureRV; }
	inline void SetTextureRV(ID3D11ShaderResourceView* textureRV) { m_textureRV = textureRV; }
	inline bool HasTexture() const { return m_textureRV ? true : false; }
	inline const Geometry* GetGeometryData() const { return m_geometry; }
	inline const Material* GetMaterial() const { return m_material; }
};

