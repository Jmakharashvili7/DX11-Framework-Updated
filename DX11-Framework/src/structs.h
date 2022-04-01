#pragma once
#include "DirectXMath.h"
#include "Vector3.h"

using namespace DirectX;

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

struct MeshData
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;
};

struct SimpleVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 texCord;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	};
};

struct BoundingSphere
{
	Vector3 center;
	float radius;

	BoundingSphere(Vector3 center, float radius)
	{
		this->center = center;
		this->radius = radius;
	}

	BoundingSphere()
	{
		center = { 0.0f, 0.0f, 0.0f };
		radius = 0.0f;
	}
	
	~BoundingSphere() {}
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 SpecularMaterial;
	XMFLOAT4 AmbientLight;
	XMFLOAT4 DiffuseLight;
	XMFLOAT4 SpecularLight;
	float	 SpecularPower;
	XMFLOAT3 LightVecW;
	XMFLOAT3 EyePosW; // camera position in world space
};