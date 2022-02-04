#include "Camera.h"

Camera::Camera(XMFLOAT3 position, FLOAT windowWidth, FLOAT windowHeight, 
	FLOAT nearDepth, FLOAT farDepth) 
	: m_Position(position), 
	  m_LookVec(0.0f, 0.0f, 1.0f), 
	  m_UpVec(0.0f, 1.0f, 0.0f), 
	  m_RightVec(1.0f, 0.0f, 0.0f), 
	  m_WindowHeight(windowHeight),
	  m_WindowWidth(windowWidth), 
	  m_NearDepth(nearDepth), 
	  m_FarDepth(farDepth)
{
}

Camera::Camera(XMFLOAT3 position, XMFLOAT3 lookVec, XMFLOAT3 rightVec, XMFLOAT3 upVec, FLOAT windowWidth, FLOAT windowHeight, 
	FLOAT nearDepth, FLOAT farDepth) 
	: m_Position(position), 
	  m_LookVec(lookVec), 
	  m_UpVec(upVec), 
	  m_RightVec(1.0f, 0.0f, 0.0f), 
	  m_WindowHeight(windowHeight),
	  m_WindowWidth(windowWidth), 
	  m_NearDepth(nearDepth), 
	  m_FarDepth(farDepth)
{
}

Camera::~Camera()
{

}

void Camera::Update()
{
	// Convert from float3 to vector
	XMVECTOR eye = XMLoadFloat3(&m_Position);
	XMVECTOR at  = XMLoadFloat3(&m_LookVec);
	XMVECTOR up  = XMLoadFloat3(&m_UpVec);

	// Initialize the view matrix
	XMStoreFloat4x4(&m_View, XMMatrixLookToLH(eye, at, up));

    // Initialize the projection matrix
	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(XM_PIDIV2, m_WindowWidth / m_WindowHeight, m_NearDepth, m_FarDepth));
}

void Camera::UpdateViewMatrix()
{
	// Load the vectors as XMVectors so we can use the built in math
	XMVECTOR R = XMLoadFloat3(&m_RightVec);
	XMVECTOR U = XMLoadFloat3(&m_UpVec);
	XMVECTOR L = XMLoadFloat3(&m_LookVec);
	XMVECTOR P = XMLoadFloat3(&m_Position);

	// normalize the Look at vector
	L = XMVector3Normalize(L);

	// We get the cross product of look and right vector to
	// make sure the vector is orthogonal and normalize it.
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// Both Up and Look at vectors are normal and orthogonal,
	// thus we just need to calcualte the cross product and
	// the resulting vector which is orthogonal to them will 
	// end up being the right vector
	R = XMVector3Cross(U, L);

	//Fill in the view matrix entries
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	// We dont need any more math so store the vectors as floats 
	XMStoreFloat3(&m_RightVec, R);
	XMStoreFloat3(&m_LookVec, L);
	XMStoreFloat3(&m_UpVec, U);

	// Fill in the right vec for the view matrix
	m_View(0, 0) = m_RightVec.x;
	m_View(1, 0) = m_RightVec.y;
	m_View(2, 0) = m_RightVec.z;
	m_View(3, 0) = x;

	// fill in the Up vec for the view matrix
	m_View(0, 1) = m_UpVec.x;
	m_View(1, 1) = m_UpVec.y;
	m_View(2, 1) = m_UpVec.z;
	m_View(3, 1) = y;

	// fill in the Look at vec for the view matrix
	m_View(0, 2) = m_LookVec.x;
	m_View(1, 2) = m_LookVec.y;
	m_View(2, 2) = m_LookVec.z;
	m_View(3, 2) = z;

	// fill in the last row in the view matrix
	m_View(0, 3) = 0.0f;
	m_View(1, 3) = 0.0f;
	m_View(2, 3) = 0.0f;
	m_View(3, 3) = 1.0f; 
}

inline XMMATRIX Camera::GetViewProjMatrix() const
{
	// convert the float4x4 to matrix
	XMMATRIX viewMatrix, projMatrix, combinedMatrix;
	viewMatrix = XMLoadFloat4x4(&m_View);
	projMatrix = XMLoadFloat4x4(&m_Proj);

	// store the combined matrix
	combinedMatrix = viewMatrix * projMatrix;

	return combinedMatrix;
}
 
void Camera::Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	m_WindowWidth = windowHeight;
	m_WindowHeight = windowWidth;
	m_NearDepth = nearDepth;
	m_FarDepth = farDepth;
}
