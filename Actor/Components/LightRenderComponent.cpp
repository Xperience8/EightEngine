#include "stdafx.h"
#include "LightRenderComponent.h"
#include "EightEngine\Actor\Components\TransformComponent.h"
#include "EightEngine\Xml\XmlNode.h"

bool LightRenderComponent::VInit(const XmlNode &configNode)
{
	// Loading ambient light data from xml config file
	auto ambientNode = configNode.GetChildrenNode("Ambient");
	m_LightsData.Ambient = DirectX::XMFLOAT4(
		std::stof(ambientNode.GetAttribute("r").GetValue()),
		std::stof(ambientNode.GetAttribute("g").GetValue()),
		std::stof(ambientNode.GetAttribute("b").GetValue()),
		std::stof(ambientNode.GetAttribute("a").GetValue())
		);

	// Loading diffuse light data from xml config file
	auto diffuseNode = configNode.GetChildrenNode("Diffuse");
	m_LightsData.Diffuse = DirectX::XMFLOAT4(
		std::stof(diffuseNode.GetAttribute("r").GetValue()),
		std::stof(diffuseNode.GetAttribute("g").GetValue()),
		std::stof(diffuseNode.GetAttribute("b").GetValue()),
		std::stof(diffuseNode.GetAttribute("a").GetValue())
		);

	// Loading specular light data from xml config file
	auto specularNode = configNode.GetChildrenNode("Specular");
	m_LightsData.Specular = DirectX::XMFLOAT4(
		std::stof(specularNode.GetAttribute("r").GetValue()),
		std::stof(specularNode.GetAttribute("g").GetValue()),
		std::stof(specularNode.GetAttribute("b").GetValue()),
		std::stof(specularNode.GetAttribute("a").GetValue())
		);

	// Loading lights boundary -> sphere about scene on which can lights affect
	auto boundaryNode = configNode.GetChildrenNode("BoundingSphere");
	m_Boundary = DirectX::BoundingSphere(DirectX::XMFLOAT3(
		std::stof(boundaryNode.GetAttribute("x").GetValue()),
		std::stof(boundaryNode.GetAttribute("y").GetValue()),
		std::stof(boundaryNode.GetAttribute("z").GetValue())),
		std::stof(boundaryNode.GetAttribute("radius").GetValue())
		);

	return true;
}
void LightRenderComponent::VSaveToXML(XmlNode &configNode)
{
	XmlNode componentNode = configNode.CreateChildNode("LightRenderComponent");
	componentNode.CreateAttribute("type", "RenderComponent");

	XmlNode ambientNode = componentNode.CreateChildNode("Ambient");
	ambientNode.CreateAttribute("r", std::to_string(m_LightsData.Ambient.x));
	ambientNode.CreateAttribute("g", std::to_string(m_LightsData.Ambient.y));
	ambientNode.CreateAttribute("b", std::to_string(m_LightsData.Ambient.z));
	ambientNode.CreateAttribute("a", std::to_string(m_LightsData.Ambient.w));

	XmlNode diffuseNode = componentNode.CreateChildNode("Diffuse");
	diffuseNode.CreateAttribute("r", std::to_string(m_LightsData.Diffuse.x));
	diffuseNode.CreateAttribute("g", std::to_string(m_LightsData.Diffuse.y));
	diffuseNode.CreateAttribute("b", std::to_string(m_LightsData.Diffuse.z));
	diffuseNode.CreateAttribute("a", std::to_string(m_LightsData.Diffuse.w));

	XmlNode specularNode = componentNode.CreateChildNode("Specular");
	specularNode.CreateAttribute("r", std::to_string(m_LightsData.Specular.x));
	specularNode.CreateAttribute("g", std::to_string(m_LightsData.Specular.y));
	specularNode.CreateAttribute("b", std::to_string(m_LightsData.Specular.z));
	specularNode.CreateAttribute("a", std::to_string(m_LightsData.Specular.w));

	XmlNode boundingSphereNode = componentNode.CreateChildNode("BoundingSphere");
	boundingSphereNode.CreateAttribute("x", std::to_string(m_Boundary.Center.x));
	boundingSphereNode.CreateAttribute("y", std::to_string(m_Boundary.Center.y));
	boundingSphereNode.CreateAttribute("z", std::to_string(m_Boundary.Center.z));
	boundingSphereNode.CreateAttribute("radius", std::to_string(m_Boundary.Radius));
}
void LightRenderComponent::BuildShadowMatrix()
{
	// Get world matrix from transform component
	auto pTransformComponent = m_pParentActor._Get()->GetActorComponent<TransformComponent>();
	DirectX::XMMATRIX worldMatrix = pTransformComponent->GetWorldMatrix();

	// Default light direction is into the screen and for getting
	// right light direction we need to transform default direction with world matrix
	// which holds rotation(translation and scaling is ignored)
	DirectX::XMVECTOR direction = DirectX::XMVector3TransformNormal(
		DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), worldMatrix);
	DirectX::XMVECTOR position = worldMatrix.r[3];

	// Creating view matrix
	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(position, 
		direction, DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));

	// Target on which light is pointing on is in the opposite direction
	// as lights
	DirectX::XMVECTOR target = DirectX::XMVectorAdd(position, 
		DirectX::XMVectorScale(direction, -2.f * m_Boundary.Radius));

	// Transform target position inside view space
	DirectX::XMFLOAT3 boundingSphereViewCenter;
	DirectX::XMStoreFloat3(&boundingSphereViewCenter, 
		DirectX::XMVector3TransformCoord(target, view));

	// Creat about the target location imaginary sphere -> lights boundary 
	float l = boundingSphereViewCenter.x - m_Boundary.Radius;
	float r = boundingSphereViewCenter.x + m_Boundary.Radius;
	float b = boundingSphereViewCenter.y - m_Boundary.Radius;
	float t = boundingSphereViewCenter.y + m_Boundary.Radius;
	float n = boundingSphereViewCenter.z - m_Boundary.Radius;
	float f = boundingSphereViewCenter.z + m_Boundary.Radius;

	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
	DirectX::XMMATRIX transform = DirectX::XMMatrixSet(
		0.5f, 0.f, 0.f, 0.f, 
		0.f, -0.5f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f, 
		0.5f, 0.5f, 0.f, 1.f
		);
	DirectX::XMMATRIX shadowTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(view, proj), transform);

	DirectX::XMStoreFloat4x4(&m_ViewMatrix, view);
	DirectX::XMStoreFloat4x4(&m_ProjMatrix, proj);
	DirectX::XMStoreFloat4x4(&m_ShadowTransform, shadowTransform);
	DirectX::XMStoreFloat3(&m_LightsData.Direction, direction);
}