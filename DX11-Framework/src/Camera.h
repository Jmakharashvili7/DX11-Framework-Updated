#pragma once
#include <d3d11_1.h>
#include <directxmath.h>
#include <memory>
#include "structs.h"

class Camera
{
protected:
	// Attributes to store the camera position
	XMFLOAT3 m_Position, m_LookVec, m_UpVec, m_RightVec;
	
	// Attributes to hold the window information and the near and far depth values 
	FLOAT m_WindowWidth, m_WindowHeight, m_NearDepth, m_FarDepth;
	
	// Attributes to hold the view and projection matrices which will be passed to the shader
	XMFLOAT4X4 m_View, m_Proj;
	
public:
	//Constructor and destructor for the camera
	Camera(XMFLOAT3 position, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	Camera(XMFLOAT3 position, XMFLOAT3 lookVec, XMFLOAT3 rightVec, XMFLOAT3 upVec, FLOAT windowWidth, 
		FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	~Camera();
	
	
	// Retrieve and Set functions for the Camera Eye
	inline void SetPosition(const XMFLOAT3& position) { m_Position = position; }
	inline XMVECTOR GetPosition() const { return XMLoadFloat3(&m_Position); }

	// Retrieve and Set functions for the Camera At
	inline void SetAt(const XMFLOAT3& at) { m_LookVec = at; }
	inline XMVECTOR GetAt() const { return XMLoadFloat3(&m_LookVec); }

	// Retrieve and Set functions for the Camera Up
	inline void SetUp(const XMFLOAT3& up) { m_UpVec = up; }
	inline XMVECTOR GetUp() const { return XMLoadFloat3(&m_UpVec); }
	
	// Functions to retrieve View, Projection, and the ViewProjection matrices
	inline XMMATRIX GetViewMatrix() const { return XMLoadFloat4x4(&m_View); }
	inline XMMATRIX GetProjMatrix() const { return XMLoadFloat4x4(&m_Proj); }
	inline XMMATRIX GetViewProjMatrix() const; // update

	// Update View Matrix
	void UpdateViewMatrix();

	// Update function to make the current view and projection matrices
	virtual void Update();

	// Rotation functions
	virtual void RotateP(float angle) {}
	virtual void RotateY(float angle) {}

	virtual void Walk(float force) {}
	virtual void Strafe(float force) {}

	// A function to reshape the camera volume if the window is resized.
	void Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
};

