#include "stdafx.h"
#include "DirectionalLightComponent.h"
#include "EightEngine\Xml\XmlNode.h"

bool DirectionalLightComponent::VInit(const XmlNode &configNode)
{
	// Loading ambient light data from xml config file
	auto ambientNode = configNode.GetChildrenNode("Ambient");
	m_LightData.Ambient = DirectX::XMFLOAT4(
		std::stof(ambientNode.GetAttribute("r").GetValue()),
		std::stof(ambientNode.GetAttribute("g").GetValue()),
		std::stof(ambientNode.GetAttribute("b").GetValue()),
		std::stof(ambientNode.GetAttribute("a").GetValue())
		);

	// Loading diffuse light data from xml config file
	auto diffuseNode = configNode.GetChildrenNode("Diffuse");
	m_LightData.Diffuse = DirectX::XMFLOAT4(
		std::stof(diffuseNode.GetAttribute("r").GetValue()),
		std::stof(diffuseNode.GetAttribute("g").GetValue()),
		std::stof(diffuseNode.GetAttribute("b").GetValue()),
		std::stof(diffuseNode.GetAttribute("a").GetValue())
		);

	// Loading specular light data from xml config file
	auto specularNode = configNode.GetChildrenNode("Specular");
	m_LightData.Specular = DirectX::XMFLOAT4(
		std::stof(specularNode.GetAttribute("r").GetValue()),
		std::stof(specularNode.GetAttribute("g").GetValue()),
		std::stof(specularNode.GetAttribute("b").GetValue()),
		std::stof(specularNode.GetAttribute("a").GetValue())
		);


	auto directionNode = configNode.GetChildrenNode("Direction");
	m_LightData.Direction = DirectX::XMFLOAT3(
		std::stof(directionNode.GetAttribute("x").GetValue()),
		std::stof(directionNode.GetAttribute("y").GetValue()),
		std::stof(directionNode.GetAttribute("z").GetValue())
		);
	

	// Loading lights volume -> sphere about scene on which can lights affect
	auto volumeNode = configNode.GetChildrenNode("LightsVolume");
	m_LightsVolume = DirectX::BoundingSphere(DirectX::XMFLOAT3(
		std::stof(volumeNode.GetAttribute("x").GetValue()),
		std::stof(volumeNode.GetAttribute("y").GetValue()),
		std::stof(volumeNode.GetAttribute("z").GetValue())),
		std::stof(volumeNode.GetAttribute("radius").GetValue())
		);

	BuildShadowMatrix();

	return true;
}
void DirectionalLightComponent::VSaveToXML(XmlNode &configNode)
{
	XmlNode componentNode = configNode.CreateChildNode("DirectionalLightComponent");
	componentNode.CreateAttribute("type", "RenderComponent");

	XmlNode ambientNode = componentNode.CreateChildNode("Ambient");
	ambientNode.CreateAttribute("r", std::to_string(m_LightData.Ambient.x));
	ambientNode.CreateAttribute("g", std::to_string(m_LightData.Ambient.y));
	ambientNode.CreateAttribute("b", std::to_string(m_LightData.Ambient.z));
	ambientNode.CreateAttribute("a", std::to_string(m_LightData.Ambient.w));

	XmlNode diffuseNode = componentNode.CreateChildNode("Diffuse");
	diffuseNode.CreateAttribute("r", std::to_string(m_LightData.Diffuse.x));
	diffuseNode.CreateAttribute("g", std::to_string(m_LightData.Diffuse.y));
	diffuseNode.CreateAttribute("b", std::to_string(m_LightData.Diffuse.z));
	diffuseNode.CreateAttribute("a", std::to_string(m_LightData.Diffuse.w));

	XmlNode specularNode = componentNode.CreateChildNode("Specular");
	specularNode.CreateAttribute("r", std::to_string(m_LightData.Specular.x));
	specularNode.CreateAttribute("g", std::to_string(m_LightData.Specular.y));
	specularNode.CreateAttribute("b", std::to_string(m_LightData.Specular.z));
	specularNode.CreateAttribute("a", std::to_string(m_LightData.Specular.w));

	XmlNode directionNode = componentNode.CreateChildNode("Direction");
	directionNode.CreateAttribute("x", std::to_string(m_LightData.Direction.x));
	directionNode.CreateAttribute("y", std::to_string(m_LightData.Direction.y));
	directionNode.CreateAttribute("z", std::to_string(m_LightData.Direction.z));

	XmlNode boundingSphereNode = componentNode.CreateChildNode("LightsVolume");
	boundingSphereNode.CreateAttribute("x", std::to_string(m_LightsVolume.Center.x));
	boundingSphereNode.CreateAttribute("y", std::to_string(m_LightsVolume.Center.y));
	boundingSphereNode.CreateAttribute("z", std::to_string(m_LightsVolume.Center.z));
	boundingSphereNode.CreateAttribute("radius", std::to_string(m_LightsVolume.Radius));
}
void DirectionalLightComponent::BuildShadowMatrix()
{
	DirectX::XMVECTOR direction = DirectX::XMLoadFloat3(&m_LightData.Direction);
	DirectX::XMVECTOR target = DirectX::XMLoadFloat3(&m_LightsVolume.Center);

	DirectX::XMVECTOR position = DirectX::XMVectorAdd(target, DirectX::XMVectorScale(
		direction, m_LightsVolume.Radius * -2.f));

	// Creating view matrix
	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(position,
		direction, DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));

	// Transform target position inside view space
	DirectX::XMFLOAT3 boundingSphereViewCenter;
	DirectX::XMStoreFloat3(&boundingSphereViewCenter,
		DirectX::XMVector3TransformCoord(target, view));

	// Creat about the target location imaginary sphere -> lights boundary 
	float l = boundingSphereViewCenter.x - m_LightsVolume.Radius;
	float r = boundingSphereViewCenter.x + m_LightsVolume.Radius;
	float b = boundingSphereViewCenter.y - m_LightsVolume.Radius;
	float t = boundingSphereViewCenter.y + m_LightsVolume.Radius;
	float n = boundingSphereViewCenter.z - m_LightsVolume.Radius;
	float f = boundingSphereViewCenter.z + m_LightsVolume.Radius;

	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
	DirectX::XMMATRIX transform = DirectX::XMMatrixSet(
		0.5f, 0.f, 0.f, 0.f,
		0.f, -0.5f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.5f, 0.5f, 0.f, 1.f
		);
	DirectX::XMMATRIX shadowTransform = DirectX::XMMatrixMultiply(
		DirectX::XMMatrixMultiply(view, proj), transform);

	DirectX::XMStoreFloat4x4(&m_ViewMatrix, view);
	DirectX::XMStoreFloat4x4(&m_ProjMatrix, proj);
	DirectX::XMStoreFloat4x4(&m_ShadowTransform, shadowTransform);
}
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"
#include "EightEngine\Graphics\ConstantBufferCache.h"

void DirectionalLightComponent::BindLights(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	LightCBData lightCBData(m_LightData);

	pRenderingDeviceContext->GetConstantBuffer(LightCBData::NAME)->UpdateBuffer(
		&lightCBData, pRenderingDeviceContext);
}