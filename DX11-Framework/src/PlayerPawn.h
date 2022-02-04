#pragma once
#include "BaseObjectOBJ.h"
#include "Camera.h"
#include "FPCamera.h"
#include "Log.h"
#include "TPCamera.h"

class PlayerPawn : public BaseObjectOBJ
{
private:
    Camera* m_CurrentCamera;
    TPCamera *m_TPCamera;
    FP_Camera *m_FPCamera;
    bool m_IsFPCamera = false;
    XMFLOAT3 m_Position, m_RightVec, m_UpVec, m_LookVec;
    float m_RotationY = 0.0f, m_RotationP = 0.0f, m_Speed = 1.0f;
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

    inline XMFLOAT3 GetPosition() { return m_Position; }

    inline Camera* GetFPCamera() { return (Camera*) m_FPCamera; }
    inline Camera* GetTPCamera() { return (Camera*) m_TPCamera; } 

    // To be called in the update function of the application
    void Update();
};

