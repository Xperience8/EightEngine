#include "stdafx.h"
#include "GameCamera.h"

GameCamera::GameCamera() : m_Position(0.f, 0.f, 0.f), m_Right(1.f, 0.f, 0.f),
	m_Up(0.f, 1.f, 0.f), m_Look(0.f, 0.f, 1.f), m_Yaw(0.f), m_Pitch(0.f),
	m_OffsetFromActor(0.f, 0.f, 0.f)
{
	DirectX::XMStoreFloat4x4(&m_ViewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_ProjMatrix, DirectX::XMMatrixIdentity());
}
void GameCamera::CreateProjMatrix(float fovY, float aspectRatio, float nearZ, float farZ)
{
	DirectX::XMStoreFloat4x4(&m_ProjMatrix, DirectX::XMMatrixPerspectiveFovLH(fovY,
		aspectRatio, nearZ, farZ));
}
void GameCamera::LookAt(DirectX::FXMVECTOR position, DirectX::FXMVECTOR target,
	DirectX::FXMVECTOR up)
{
	// Compute look, right and up vectors and save them

	DirectX::XMVECTOR l = DirectX::XMVector3Normalize(
		DirectX::XMVectorSubtract(target, position));
	DirectX::XMVECTOR r = DirectX::XMVector3Normalize(
		DirectX::XMVector3Cross(up, l));
	DirectX::XMVECTOR u = DirectX::XMVector3Cross(l, r);

	DirectX::XMStoreFloat3(&m_Look, l);
	DirectX::XMStoreFloat3(&m_Right, r);
	DirectX::XMStoreFloat3(&m_Up, u);
	DirectX::XMStoreFloat3(&m_Position, position);
}
void GameCamera::Update()
{
	// Compute view matrix

	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&m_Position);
	DirectX::XMVECTOR look = DirectX::XMLoadFloat3(&m_Look);
	DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&m_Right);
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_Up);

	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, right));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, up));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, look));

	m_ViewMatrix(0, 0) = m_Right.x;
	m_ViewMatrix(1, 0) = m_Right.y;
	m_ViewMatrix(2, 0) = m_Right.z;
	m_ViewMatrix(3, 0) = x;

	m_ViewMatrix(0, 1) = m_Up.x;
	m_ViewMatrix(1, 1) = m_Up.y;
	m_ViewMatrix(2, 1) = m_Up.z;
	m_ViewMatrix(3, 1) = y;

	m_ViewMatrix(0, 2) = m_Look.x;
	m_ViewMatrix(1, 2) = m_Look.y;
	m_ViewMatrix(2, 2) = m_Look.z;
	m_ViewMatrix(3, 2) = z;

	// Not needed
	m_ViewMatrix(0, 3) = 0.f;
	m_ViewMatrix(1, 3) = 0.f;
	m_ViewMatrix(2, 3) = 0.f;
	m_ViewMatrix(3, 3) = 1.f;

	ComputeFrustum();
}
void GameCamera::ComputeFrustum()
{
	DirectX::XMMATRIX vp = GetViewProjMatrix();

	DirectX::XMVECTOR zero = DirectX::XMVectorSet(DirectX::XMVectorGetX(vp.r[0]), DirectX::XMVectorGetX(vp.r[1]),
		DirectX::XMVectorGetX(vp.r[2]), DirectX::XMVectorGetX(vp.r[3]));
	DirectX::XMVECTOR one = DirectX::XMVectorSet(DirectX::XMVectorGetY(vp.r[0]), DirectX::XMVectorGetY(vp.r[1]),
		DirectX::XMVectorGetY(vp.r[2]), DirectX::XMVectorGetY(vp.r[3]));
	DirectX::XMVECTOR two = DirectX::XMVectorSet(DirectX::XMVectorGetZ(vp.r[0]), DirectX::XMVectorGetZ(vp.r[1]),
		DirectX::XMVectorGetZ(vp.r[2]), DirectX::XMVectorGetZ(vp.r[3]));
	DirectX::XMVECTOR three = DirectX::XMVectorSet(DirectX::XMVectorGetW(vp.r[0]), DirectX::XMVectorGetW(vp.r[1]),
		DirectX::XMVectorGetW(vp.r[2]), DirectX::XMVectorGetW(vp.r[3]));

	// Left
	DirectX::XMVECTOR leftPlane = DirectX::XMPlaneNormalize(DirectX::XMVectorAdd(three, zero));
	DirectX::XMStoreFloat4(&m_Frustum.Planes[0], leftPlane);
	DirectX::XMStoreFloat4(&m_Frustum.AbsPlanes[0], DirectX::XMVectorAbs(leftPlane));
	// Right
	DirectX::XMVECTOR rightPlane = DirectX::XMPlaneNormalize(DirectX::XMVectorSubtract(three, zero));
	DirectX::XMStoreFloat4(&m_Frustum.Planes[1], rightPlane);
	DirectX::XMStoreFloat4(&m_Frustum.AbsPlanes[1], DirectX::XMVectorAbs(rightPlane));
	// Bottom
	DirectX::XMVECTOR bottomPlane = DirectX::XMPlaneNormalize(DirectX::XMVectorAdd(three, one));
	DirectX::XMStoreFloat4(&m_Frustum.Planes[2], bottomPlane);
	DirectX::XMStoreFloat4(&m_Frustum.AbsPlanes[2], DirectX::XMVectorAbs(bottomPlane));
	// Top
	DirectX::XMVECTOR topPlane = DirectX::XMPlaneNormalize(DirectX::XMVectorSubtract(three, one));
	DirectX::XMStoreFloat4(&m_Frustum.Planes[3], topPlane);
	DirectX::XMStoreFloat4(&m_Frustum.AbsPlanes[3], DirectX::XMVectorAbs(topPlane));	
	// Near
	DirectX::XMVECTOR nearPlane = DirectX::XMPlaneNormalize(two);
	DirectX::XMStoreFloat4(&m_Frustum.Planes[4], nearPlane);
	DirectX::XMStoreFloat4(&m_Frustum.AbsPlanes[4], DirectX::XMVectorAbs(nearPlane));
	// Far
	DirectX::XMVECTOR farPlane = DirectX::XMPlaneNormalize(DirectX::XMVectorSubtract(three, two));
	DirectX::XMStoreFloat4(&m_Frustum.Planes[5], farPlane);
	DirectX::XMStoreFloat4(&m_Frustum.AbsPlanes[5], DirectX::XMVectorAbs(farPlane));
}
