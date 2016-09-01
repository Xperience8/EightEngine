#include "stdafx.h"
#include "TransformComponent.h"
#include "EightEngine\Actor\ActorFactory.h"
#include "EightEngine\Xml\XmlNode.h"

const std::string TransformComponent::s_COMPONENT_NAME = "TransformComponent";

bool TransformComponent::VInit(const XmlNode &configNode)
{
	// Load position
	auto helperNode = configNode.GetChildrenNode("Position");
	DirectX::XMVECTOR position = DirectX::XMVectorSet(
		std::stof(helperNode.GetAttribute("x").GetValue()),
		std::stof(helperNode.GetAttribute("y").GetValue()),
		std::stof(helperNode.GetAttribute("z").GetValue()),
		1.f
		);

	// Load rotation
	helperNode = configNode.GetChildrenNode("Rotation");
	DirectX::XMVECTOR rotationQuat = DirectX::XMVectorSet(
		std::stof(helperNode.GetAttribute("x").GetValue()),
		std::stof(helperNode.GetAttribute("y").GetValue()),
		std::stof(helperNode.GetAttribute("z").GetValue()),
		std::stof(helperNode.GetAttribute("w").GetValue())
		);
	
	// Load scale
	helperNode = configNode.GetChildrenNode("Scale");
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(
		std::stof(helperNode.GetAttribute("x").GetValue()),
		std::stof(helperNode.GetAttribute("y").GetValue()),
		std::stof(helperNode.GetAttribute("z").GetValue()),
		1.f
		);

	// Compute transformation from position, rotation quaternion and scale
	// R * S * P
	DirectX::XMMATRIX transform = DirectX::XMMatrixMultiply(
		DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationQuaternion(rotationQuat), 
		DirectX::XMMatrixScalingFromVector(scale)),
		DirectX::XMMatrixTranslationFromVector(position));

	DirectX::XMStoreFloat4x4(&m_WorldMatrix, transform);

	return true;
}
void TransformComponent::VSaveToXML(XmlNode &configNode)
{
	XmlNode componentNode = configNode.CreateChildNode("TransformComponent");
	componentNode.CreateAttribute("type", "TransformComponent");

	DirectX::XMVECTOR positionXm;
	DirectX::XMVECTOR rotQuaternionXm;
	DirectX::XMVECTOR scaleXm;
	DirectX::XMMatrixDecompose(&scaleXm, &rotQuaternionXm, &positionXm, 
		DirectX::XMLoadFloat4x4(&m_WorldMatrix));

	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, positionXm);
	auto positionNode = componentNode.CreateChildNode("Position");
	positionNode.CreateAttribute("x", std::to_string(position.x));
	positionNode.CreateAttribute("y", std::to_string(position.y));
	positionNode.CreateAttribute("z", std::to_string(position.z));

	DirectX::XMFLOAT4 rotQuaternion;
	DirectX::XMStoreFloat4(&rotQuaternion, rotQuaternionXm);
	auto rotationNode = componentNode.CreateChildNode("Rotation");
	rotationNode.CreateAttribute("x", std::to_string(rotQuaternion.x));
	rotationNode.CreateAttribute("y", std::to_string(rotQuaternion.y));
	rotationNode.CreateAttribute("z", std::to_string(rotQuaternion.z));
	rotationNode.CreateAttribute("w", std::to_string(rotQuaternion.w));

	DirectX::XMFLOAT3 scale;
	DirectX::XMStoreFloat3(&scale, scaleXm);
	auto scaleNode = componentNode.CreateChildNode("Scale");
	scaleNode.CreateAttribute("x", std::to_string(scale.x));
	scaleNode.CreateAttribute("y", std::to_string(scale.y));
	scaleNode.CreateAttribute("z", std::to_string(scale.z));
}