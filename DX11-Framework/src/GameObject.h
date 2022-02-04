#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>
#include "structs.h"

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
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	XMFLOAT3 m_Scale;

	string m_Type;

	XMFLOAT4X4 m_World;

	Geometry m_Geometry;
	Material m_Material;

	ID3D11ShaderResourceView* m_TextureRV;

	GameObject* m_Parent;
public:
	GameObject(string type, Geometry geometry, Material material);
	~GameObject();

	// Setters and Getters for position
	void SetPosition(XMFLOAT3 position) { m_Position = position; }
	void SetPosition(float x, float y, float z) { m_Position.x = x; m_Position.y = y; m_Position.z = z; }

	XMFLOAT3 GetPosition() const { return m_Position; }

	// Setters and getters for scale
	void SetScale(XMFLOAT3 scale) { m_Scale = scale; }
	void SetScale(float x, float y, float z) { m_Scale.x = x; m_Scale.y = y; m_Scale.z = z; }

	XMFLOAT3 GetScale() const { return m_Scale; }

	// Setters and getters for rotation
	void SetRotation(XMFLOAT3 rotation) { m_Rotation = rotation; }
	void SetRotation(float x, float y, float z) { m_Rotation.x = x; m_Rotation.y = y; m_Rotation.z = z; }

	XMFLOAT3 GetRotation() const { return m_Rotation; }

	// returns a string of object type
	string GetType() const { return m_Type; }

	Geometry GetGeometryData() const { return m_Geometry; }

	Material GetMaterial() const { return m_Material; }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&m_World); }

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
