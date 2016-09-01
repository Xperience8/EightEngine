#include "stdafx.h"
#include "KulaWorldExitDoorTriggerComponent.h"

#include "ModelRenderComponent.h"
#include "EightEngine\Graphics\Model.h"
#include "EightEngine\Graphics\Material.h"

#include "EightEngine\Xml\XmlNode.h"

bool KulaWorldExitDoorTriggerComponent::VInit(const XmlNode &configNode)
{
	XmlNode closedColorNode = configNode.GetChildrenNode("ClosedColor");
	m_ClosedColor.x = std::stof(closedColorNode.GetAttribute("r").GetValue());
	m_ClosedColor.y = std::stof(closedColorNode.GetAttribute("g").GetValue());
	m_ClosedColor.z = std::stof(closedColorNode.GetAttribute("b").GetValue());
	m_ClosedColor.w = std::stof(closedColorNode.GetAttribute("a").GetValue());

	XmlNode openedColorNode = configNode.GetChildrenNode("OpenedColor");
	m_OpenedColor.x = std::stof(openedColorNode.GetAttribute("r").GetValue());
	m_OpenedColor.y = std::stof(openedColorNode.GetAttribute("g").GetValue());
	m_OpenedColor.z = std::stof(openedColorNode.GetAttribute("b").GetValue());
	m_OpenedColor.w = std::stof(openedColorNode.GetAttribute("a").GetValue());

	m_AmbientScaleFactor = std::stof(configNode.GetChildrenNode("AmbientScaleFactor").GetValue());

	return true;
}

#include "TransformComponent.h"
void KulaWorldExitDoorTriggerComponent::VUpdate(float deltaTime)
{
	auto pTransformComponent = m_pParentActor._Get()->GetActorComponent<TransformComponent>();
	
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationY(DirectX::XM_PIDIV2 * deltaTime);

	pTransformComponent->SetWorldMatrix(DirectX::XMMatrixMultiply(rotation, 
		pTransformComponent->GetWorldMatrix()));
}

void KulaWorldExitDoorTriggerComponent::VSaveToXML(XmlNode &configNode)
{
	XmlNode componentNode = configNode.CreateChildNode("KulaWorldExitDoorTriggerComponent");
	componentNode.CreateAttribute("type", "TriggerComponent");

	XmlNode closedColorNode = componentNode.CreateChildNode("ClosedColor");
	closedColorNode.CreateAttribute("r", std::to_string(m_ClosedColor.x));
	closedColorNode.CreateAttribute("g", std::to_string(m_ClosedColor.y));
	closedColorNode.CreateAttribute("b", std::to_string(m_ClosedColor.z));
	closedColorNode.CreateAttribute("a", std::to_string(m_ClosedColor.w));

	XmlNode openedColorNode = componentNode.CreateChildNode("OpenedColor");
	openedColorNode.CreateAttribute("r", std::to_string(m_OpenedColor.x));
	openedColorNode.CreateAttribute("g", std::to_string(m_OpenedColor.y));
	openedColorNode.CreateAttribute("b", std::to_string(m_OpenedColor.z));
	openedColorNode.CreateAttribute("a", std::to_string(m_OpenedColor.w));

	componentNode.CreateChildNode("AmbientScaleFactor", std::to_string(m_AmbientScaleFactor));
}

void KulaWorldExitDoorTriggerComponent::OpenExitDoorEvent(const EventPtr<IEventData> &pEvent)
{
	auto pEventData = StaticPointerCast<OpenExitDoorEventData>(pEvent);

	m_ClosedDoor = !pEventData->GetDoorStatus();

	if (m_ClosedDoor)
	{
		ModifyMaterialColor(DirectX::XMLoadFloat4(&m_ClosedColor));
	}
	else
	{
		ModifyMaterialColor(DirectX::XMLoadFloat4(&m_OpenedColor));
	}
}

void KulaWorldExitDoorTriggerComponent::ModifyMaterialColor(DirectX::FXMVECTOR color)
{
	auto pModelRenderComponent = m_pParentActor._Get()->GetActorComponent<ModelRenderComponent>();
	if (pModelRenderComponent)
	{
		auto &materials = pModelRenderComponent->GetModel()->GetMaterials();

		EIGHT_ASSERT(materials.size() < 2, "Kula world exit doors must has only one material!");

		auto lightMaterial = materials[0].GetLightMaterial();

		DirectX::XMStoreFloat4(&lightMaterial.Ambient, DirectX::XMVectorScale(
			color, m_AmbientScaleFactor));
		DirectX::XMStoreFloat4(&lightMaterial.Diffuse, color);

		materials[0].SetLightMaterial(lightMaterial);
	}
}
