#include "PlanetObj.h"

PlanetObj::PlanetObj(MeshData meshData, Camera TPCamera)
	: BaseObjectOBJ(meshData), m_TPCamera(TPCamera)
{
}

PlanetObj::~PlanetObj()
{
}

void PlanetObj::TransformObject(XMMATRIX transformMatrix)
{
	XMStoreFloat4x4(&m_World, transformMatrix);
}
