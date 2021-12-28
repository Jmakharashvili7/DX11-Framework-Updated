#pragma once
#include "BaseObjectOBJ.h"
#include "Camera.h"

class PlayerPawn : public BaseObjectOBJ
{
private:
    //Camera m_TPCamera;
    XMFLOAT3 m_Position;
    float m_RotationY = 0.0f, m_RotationP = 0.0f, m_Speed = 0.0f;
public:
    PlayerPawn(MeshData meshData);
    ~PlayerPawn();

    // Move the player character and camera along the z axis
    void Walk(float deltaTime);

    // Move the player character and camera along the x axis
    void Strafe(float deltaTime);

    // Rotate the player character and the camera around the X axis (angle is in raidans)
    void RotatePitch(float angle);

    // Rotate the player character and the camera around the Y axis (angle is in radians)
    void RotateYaw(float angle);

    // To be called in the update function of the application
    void Update();
};

