#pragma once

#include "IActorComponent.h"

class TransformComponent : public IActorComponent
{
public:
	static const std::string s_COMPONENT_NAME;
	
	TransformComponent()
	{
		DirectX::XMStoreFloat4x4(&m_WorldMatrix, DirectX::XMMatrixIdentity());
	}
	~TransformComponent() { }

	// Create transform component from xml config file
	bool VInit(const XmlNode &configNode) override;

	// Transform component has no depedencies on other components so this call is empty
	bool VPostInit() override { return true; }

	// Transform component is only used for storing world matrix therefore this call is empty
	void VUpdate(float DeltaTime) override { }

	// Save transform component into xml config file(actual status is saved)
	void VSaveToXML(XmlNode &configNode) override;

	// Set parent actor
	// Transform component doesn't need pointer to actor therefore this call is empty
	void VSetActor(const std::weak_ptr<Actor> &pActor) override { }

	// Returns "TransformComponent"
	const std::string &VGetComponentType() const override { return s_COMPONENT_NAME; }

	// Set world matrix
	// Matrix is moved from SIMD register into RAM(very fast operation)
	void SetWorldMatrix(DirectX::CXMMATRIX NewWorldMatrix) 
	{ 
		DirectX::XMStoreFloat4x4(&m_WorldMatrix, NewWorldMatrix); 
	}

	// Putting matrix into SIMD register and returning it(very fast operation)
	// It has advantages in computations
	DirectX::XMMATRIX GetWorldMatrix() const 
	{ 
		return DirectX::XMLoadFloat4x4(&m_WorldMatrix); 
	}
private:
	DirectX::XMFLOAT4X4 m_WorldMatrix;
};