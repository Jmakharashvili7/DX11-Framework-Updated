#pragma once
#include "BaseObjectOBJ.h"
#include "Camera.h"
#include "FPCamera.h"

class PlayerPawn : public BaseObjectOBJ
{
private:
    Camera* m_CurrentCamera, *m_TPCamera;
    FP_Camera *m_FPCamera;
    bool m_IsFPCamera = false;
    XMFLOAT3 m_Position, m_RightVec, m_UpVec, m_LookVec;
    float m_RotationY = 0.0f, m_RotationP = 0.0f, m_Speed = 0.0f;
public:
    PlayerPawn(MeshData meshData, XMFLOAT3 position, UINT WindowHeight, UINT WindowWidth, FLOAT nearDepth, FLOAT farDepth);
    ~PlayerPawn();

    // Move the player character and camera along the z axis
    void Walk(float deltaTime);

    // Move the player character and camera along the x axis
    void Strafe(float deltaTime);

    // Rotate the player character and the camera around the X axis (angle is in raidans)
    void RotatePitch(float angle);

    // Rotate the player character and the camera around the Y axis (angle is in radians)
    void RotateYaw(float angle);

    void Render(XMMATRIX& worldMatrix, ConstantBuffer& buffer, ID3D11Buffer* constBuffer, 
        ID3D11DeviceContext* deviceContext) override;

    // Get the cameras
    inline Camera* GetCamera() { return m_CurrentCamera; }
    //inline void SwitchFPCamera() { if (!m_IsFPCamera) m_CurrentCamera = m_FPCamera; }
    //inline void SwitchTPCamera() { if ( m_IsFPCamera) m_CurrentCamera = m_TPCamera; }

    // To be called in the update function of the application
    void Update();
};

