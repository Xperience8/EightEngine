#pragma once

#include "LightRenderComponent.h"

class RenderingDeviceContext;

class DirectionalLightComponent : public RenderComponent
{
public:
	DirectionalLightComponent() : m_LightsVolume(DirectX::XMFLOAT3(0.f, 0.f, 0.f), 0.f)
	{
		m_LightData.Ambient = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
		m_LightData.Diffuse = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
		m_LightData.Specular = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
		m_LightData.Direction = DirectX::XMFLOAT3(0.f, 0.f, 1.f);

		DirectX::XMStoreFloat4x4(&m_ViewMatrix, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&m_ProjMatrix, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&m_ShadowTransform, DirectX::XMMatrixIdentity());
	}
	~DirectionalLightComponent() { }

	bool VInit(const XmlNode &configNode) override;
	
	bool VPostInit() override { return true; }

	void VUpdate(float deltaTime) override { }

	void VSaveToXML(XmlNode &configNode) override;


	void VRender(Renderer &renderer) override { }
	RenderingType VGetRenderingType() const override { return RenderingType::NoRendering; }

	void BindLights(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext);
private:

	void BuildShadowMatrix();

	DirectX::XMFLOAT4X4 m_ViewMatrix;
	DirectX::XMFLOAT4X4 m_ProjMatrix;
	DirectX::XMFLOAT4X4 m_ShadowTransform;

	DirectionalLight m_LightData;

	DirectX::BoundingSphere m_LightsVolume;
};