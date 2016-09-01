#pragma once

#include "RenderComponent.h"

struct LightMaterial
{
	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	DirectX::XMFLOAT4 Emissive;
	DirectX::XMFLOAT4 Reflection;
	DirectX::XMFLOAT4 Transparent;
};
struct DirectionalLight
{
	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	DirectX::XMFLOAT3 Direction;
	float Pad;
};

class LightRenderComponent : public RenderComponent
{
public:
	LightRenderComponent() : m_Boundary(DirectX::XMFLOAT3(0.f, 0.f, 0.f), 0.f) 
	{ 
		m_LightsData.Ambient = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
		m_LightsData.Diffuse = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
		m_LightsData.Specular = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
		m_LightsData.Direction = DirectX::XMFLOAT3(0.f, 0.f, 1.f);

		DirectX::XMStoreFloat4x4(&m_ViewMatrix, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&m_ProjMatrix, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&m_ShadowTransform, DirectX::XMMatrixIdentity());
	}
	~LightRenderComponent() { }

	// Load from xml config
	// Directional light is created and boundary about scene on which light act is created too
	bool VInit(const XmlNode &configNode) override;

	// Light render component has no depedencies on other components so this call is empty
	bool VPostInit() override { return true; }
	
	// Shadow transform is recomputed
	void VUpdate(float DeltaTime) override { BuildShadowMatrix(); }

	// In Editor mode you can render a sphere about scene which lights affect
	// In game rendering is this call empty
	void VRender(Renderer &renderer) override { }

	// Returning that this class isn't for normal rendering
	RenderingType VGetRenderingType() const override { return RenderingType::NoRendering; }
	
	// Saving light render component into xml config file(actuall status is saved)
	void VSaveToXML(XmlNode &configNode) override;

	// Returning directional light data
	const DirectionalLight &GetDirectionalLightData() const { return m_LightsData; }
private:
	DirectionalLight m_LightsData;
	DirectX::BoundingSphere m_Boundary;

	DirectX::XMFLOAT4X4 m_ViewMatrix;
	DirectX::XMFLOAT4X4 m_ProjMatrix;
	DirectX::XMFLOAT4X4 m_ShadowTransform;

	void BuildShadowMatrix();
};