#pragma once

#include "GameCamera.h"

class TopCamera : public GameCamera
{
public:
	TopCamera() { }
	~TopCamera() { }

	// Compute new camera position, view matrix and frustum
	// This should be called every frame
	void VUpdate(DirectX::FXMVECTOR actorPosition) override
	{
		// Compute new camera position from offset and new actor position
		DirectX::XMVECTOR offsetFromActor = DirectX::XMLoadFloat3(&m_OffsetFromActor);
		DirectX::XMVECTOR cameraPosition = DirectX::XMVectorAdd(actorPosition, offsetFromActor);

		// Compute look, right and up vectors and save them
		LookAt(cameraPosition, actorPosition, DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f));

		// Compute new view matrix and new frustum
		GameCamera::Update();

		// Set look and right direction to default, 
		// because player is moving in camera looking direction
		// and camera looking direction is in this case down
		m_Look = DirectX::XMFLOAT3(0.f, 0.f, 1.f);
		m_Right = DirectX::XMFLOAT3(1.f, 0.f, 0.f);
	}
};