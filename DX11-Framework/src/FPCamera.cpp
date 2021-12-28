#include "FPCamera.h"

FP_Camera::FP_Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, XMFLOAT3 right, 
	FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth) :
	Camera( position, at, up, right, windowWidth, windowHeight, nearDepth, farDepth)
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

