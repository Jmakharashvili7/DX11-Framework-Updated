#include "FPCamera.h"

FP_Camera::FP_Camera(XMFLOAT3 position, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth) :
	Camera( position, windowWidth, windowHeight, nearDepth, farDepth)
{
}

void FP_Camera::Strafe(float force)
{
	XMVECTOR s = XMVectorReplicate(force);
	XMVECTOR r = XMLoadFloat3(&m_RightVec);
	XMVECTOR p = XMLoadFloat3(&m_Position);

	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(s, r, p));
}

void FP_Camera::Walk(float force)
{
	XMVECTOR s = XMVectorReplicate(force);
	XMVECTOR l = XMLoadFloat3(&m_LookVec);
	XMVECTOR p = XMLoadFloat3(&m_Position);

	// multiplies s by l and adds it to p
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(s, l, p));
}


void FP_Camera::RotateP(float angle)
{
	// Roate up and look vector about the right vector
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_RightVec), angle);

	XMStoreFloat3(&m_UpVec, XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), R));
	XMStoreFloat3(&m_LookVec, XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), R));
}

void FP_Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&m_RightVec, XMVector3TransformNormal(XMLoadFloat3(&m_RightVec), R));
	XMStoreFloat3(&m_UpVec, XMVector3TransformNormal(XMLoadFloat3(&m_UpVec), R));
	XMStoreFloat3(&m_LookVec, XMVector3TransformNormal(XMLoadFloat3(&m_LookVec), R));
}
