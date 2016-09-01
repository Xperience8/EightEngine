#pragma once

__declspec(align(16))
struct Frustum
{
	DirectX::XMFLOAT4 Planes[6];
	DirectX::XMFLOAT4 AbsPlanes[6];
};

class GameCamera
{
public:
	GameCamera();
	virtual ~GameCamera() { }

	// Create projection matrix from field of view angle Y, aspect ratio,
	// near and far plane
	void CreateProjMatrix(float fovY, float aspectRatio, float nearZ, float farZ);
	
	// Look on specific target
	// Parameters:
	// position -> camera position
	// target -> on which place should camera look to
	// up -> up direction in world(in most cases -> 0.f, 1.f, 0.f)
	void LookAt(DirectX::FXMVECTOR position, DirectX::FXMVECTOR target, 
		DirectX::FXMVECTOR up);

	// Pure virtual method
	// Every type of game camera should override it
	// This should be called every frame
	virtual void VUpdate(DirectX::FXMVECTOR actorPosition) = 0;

	// Adding pitch rotation, which will be computed in VUpdate
	void Pitch(float angleInRadians)
	{
		m_Pitch += angleInRadians;
	}

	// Adding yaw rotation, which will be computed in VUpdate
	void Yaw(float angleInRadians)
	{
		m_Yaw += angleInRadians;
	}

	// Set camera position
	void SetPosition(const DirectX::XMFLOAT3 &position)
	{
		m_Position = position;
	}
	// Set camera position from SIMD register
	void SetPosition(DirectX::FXMVECTOR position)
	{
		DirectX::XMStoreFloat3(&m_Position, position);
	}

	// Set offset from actor
	virtual void SetOffsetFromActor(const DirectX::XMFLOAT3 &offsetFromActor)
	{
		m_OffsetFromActor = offsetFromActor;
	}

	// Set offset from actor from SIMD register
	virtual void SetOffsetFromActor(DirectX::FXMVECTOR offsetFromActor)
	{
		DirectX::XMStoreFloat3(&m_OffsetFromActor, offsetFromActor);
	}

	// Pack position into SIMD register and return it
	DirectX::XMVECTOR GetPosition() const 
	{ 
		return DirectX::XMLoadFloat3(&m_Position); 
	}

	// Pack looking direction into SIMD register and return it
	DirectX::XMVECTOR GetLookingDirection() const 
	{ 
		return DirectX::XMLoadFloat3(&m_Look); 
	}

	// Pack right direction into SIMD register and return it
	DirectX::XMVECTOR GetRightDirection() const 
	{ 
		return DirectX::XMLoadFloat3(&m_Right); 
	}

	// Pack up direction into SIMD register and return it
	DirectX::XMVECTOR GetUpDirection() const 
	{
		return DirectX::XMLoadFloat3(&m_Up); 
	}

	// Pack view matrix into SIMD registers and return it
	// View matrix is transformation from world space into camera space
	DirectX::XMMATRIX GetViewMatrix() const 
	{ 
		return DirectX::XMLoadFloat4x4(&m_ViewMatrix); 
	}

	// Pack projection matrix into SIMD registers and return it
	DirectX::XMMATRIX GetProjMatrix() const 
	{ 
		return DirectX::XMLoadFloat4x4(&m_ProjMatrix); 
	}

	// Pack view and projection matrix into SIMD register and perform multiplying
	// Result is matrix which transform from world space directly to NDC space
	DirectX::XMMATRIX GetViewProjMatrix() const
	{
		return DirectX::XMMatrixMultiply(GetViewMatrix(), GetProjMatrix());
	}

	// Get frustum for frustum culling tests
	const Frustum &GetFrustum() const 
	{ 
		return m_Frustum; 
	}

protected:
	Frustum m_Frustum;

	DirectX::XMFLOAT4X4 m_ViewMatrix;
	DirectX::XMFLOAT4X4 m_ProjMatrix;

	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Look;
	DirectX::XMFLOAT3 m_Up;
	DirectX::XMFLOAT3 m_Right;

	DirectX::XMFLOAT3 m_OffsetFromActor;

	float m_Yaw;
	float m_Pitch;

	// Compute view matrix from position, look, up and right vectors
	void Update();

	// Compute frustum from ViewProj matrix
	void ComputeFrustum();
};


