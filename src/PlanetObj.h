#pragma once
#include "BaseObjectOBJ.h"
#include "Camera.h"

class PlanetObj :
    public BaseObjectOBJ
{
private:
    // Third person camera
    Camera m_TPCamera;
public:
    PlanetObj(MeshData meshData, Camera TPCamera);
    ~PlanetObj();

    void TransformObject(XMMATRIX transformMatrix);
};

