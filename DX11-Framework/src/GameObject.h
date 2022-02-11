#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>
#include "structs.h"
#include "Transform.h"

using namespace DirectX;
using namespace std;

struct Geometry
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	
	UINT indexCount;
	UINT vertexBufferStride;
	UINT vertexBufferOffset;
};

struct Material
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
	float specularPower;
};

class GameObject
{

protected:
	string m_Type;

	XMFLOAT4X4 m_World;
	Transform* m_Transform;

	Geometry m_Geometry;
	Material m_Material;

	ID3D11ShaderResourceView* m_TextureRV;

	GameObject* m_Parent;
public:
	GameObject(string type, Geometry geometry, Material material);
	~GameObject();

	// returns a string of object type
	inline string GetType() const { return m_Type; }

	inline Transform* GetTransform() { return m_Transform; }

	inline Geometry GetGeometryData() const { return m_Geometry; }

	inline Material GetMaterial() const { return m_Material; }

	inline XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&m_World); }

	void SetTextureRV(ID3D11ShaderResourceView* textureRV) { m_TextureRV = textureRV; }
	ID3D11ShaderResourceView* GetTextureRV() const { return m_TextureRV; }
	bool HasTexture() const { return m_TextureRV ? true : false; }

	// Sets the parent of the object
	void SetParent(GameObject* parent) { m_Parent = parent; }

	// Update function which sets the world matrix and takes in deltaTime
	void Update(float deltaTime);

	// Function for drawing the object
	void Draw(XMMATRIX& worldMatrix, ConstantBuffer& buffer, ID3D11Buffer* constBuffer, ID3D11DeviceContext* immediateContext);
};
