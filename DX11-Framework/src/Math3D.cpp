#include "Math3D.h"
#include "structs.h"

void Math3D::NormalAvarage(SimpleVertex vertices[], const WORD indices[], UINT vertexCount, UINT indexCount)
{
	// For each triangle in the mesh:
	for (UINT i = 0; i < (indexCount/3); ++i)
	{
		// indices of the ith triangle
		UINT i0 = indices[i * 3 + 0];
		UINT i1 = indices[i * 3 + 1];
		UINT i2 = indices[i * 3 + 2];

		// vertices of ith triangle
		XMVECTOR v0 = XMLoadFloat3(&vertices[i0].pos);
		XMVECTOR v1 = XMLoadFloat3(&vertices[i1].pos);
		XMVECTOR v2 = XMLoadFloat3(&vertices[i2].pos);

		// compute face normal
		XMVECTOR e0 = v1 - v0;
		XMVECTOR e1 = v2 - v0;
		XMVECTOR faceNormal = XMVector3Cross(e0, e1);

		// This triangle shares the following three vertices,
		// so add this face normal into the average of these
		// vertex normals.
		XMFLOAT3 normal;

		XMStoreFloat3(&normal, faceNormal);

		vertices[i0].normal.x += normal.x;
		vertices[i0].normal.y += normal.y;
		vertices[i0].normal.z += normal.z;
								 
		vertices[i1].normal.x += normal.x;
		vertices[i1].normal.y += normal.y;
		vertices[i1].normal.z += normal.z;
								 
		vertices[i2].normal.x += normal.x;
		vertices[i2].normal.y += normal.y;
		vertices[i2].normal.z += normal.z;
	}

	// For each vertex v, we have summed the face normals of all
	// the triangles that share v, so now we just need to normalize.
	for (UINT i = 0; i < vertexCount; ++i)
	{
		XMVECTOR temp = XMLoadFloat3(&vertices[i].normal);
		temp = XMVector3Normalize(temp);
		XMStoreFloat3(&vertices[i].normal, temp);
	}
}
