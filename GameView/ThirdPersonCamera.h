#pragma once

#include "GameCamera.h"

class ThirdPersonCamera : public GameCamera
{
public:
	ThirdPersonCamera() : m_ClampPitchDown(-1.f), m_ClampPitchUp(1.f)
	{
		SetOffsetFromActor(DirectX::XMVectorSet(0.f, 0.7f, -0.7f, 0.f));
	}
	~ThirdPersonCamera() { }

	// Set offset from actor
	// Maximum pitch will be computed
	void SetOffsetFromActor(const DirectX::XMFLOAT3 &offsetFromActor) override
	{
		m_OffsetFromActor = offsetFromActor;
		float initAngle = -atanf(m_OffsetFromActor.y / m_OffsetFromActor.z);

		m_PitchMaxUp = m_ClampPitchUp - initAngle;
		m_PitchMaxDown = m_ClampPitchDown - initAngle;
	}

	// Set offset from actor from SIMD register
	// Maximum pitch will be computed
	void SetOffsetFromActor(DirectX::FXMVECTOR offsetFromActor) override
	{
		DirectX::XMStoreFloat3(&m_OffsetFromActor, offsetFromActor);
		float initAngle = -atanf(m_OffsetFromActor.y / m_OffsetFromActor.z);

		m_PitchMaxUp = m_ClampPitchUp - initAngle;
		m_PitchMaxDown = m_ClampPitchDown - initAngle;
	}

	// Compute new camera position and rotate it by yaw and pitch
	// Compute view matrix and frustum
	void VUpdate(DirectX::FXMVECTOR actorPosition) override
	{
		m_Pitch = DirectX::XMMax(DirectX::XMMin(m_Pitch, m_PitchMaxUp), m_PitchMaxDown);

		DirectX::XMMATRIX rotationMatrixYaw = DirectX::XMMatrixRotationY(m_Yaw);
		DirectX::XMMATRIX rotationMatrixPitch = DirectX::XMMatrixRotationAxis(
			DirectX::XMLoadFloat3(&m_Right), m_Pitch);

		DirectX::XMVECTOR offsetFromActor = DirectX::XMVector3TransformCoord(
			DirectX::XMLoadFloat3(&m_OffsetFromActor), rotationMatrixYaw);
		offsetFromActor = DirectX::XMVector3TransformCoord(
			offsetFromActor, rotationMatrixPitch);

		DirectX::XMVECTOR cameraPosition = DirectX::XMVectorAdd(actorPosition, offsetFromActor);

		LookAt(cameraPosition, actorPosition, DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));

		GameCamera::Update();
	}
private:
	float m_PitchMaxUp;
	float m_PitchMaxDown;
	float m_ClampPitchUp;
	float m_ClampPitchDown;
};