#pragma once

#include "EightEngine\Actor\Components\TriggerComponent.h"
#include "EightEngine\EventManager\Events\CollectItemEventData.h"
#include "EightEngine\Xml\XmlNode.h"

#include "TransformComponent.h"

enum class ItemUsage
{
	AddPoints = 0x01,
	AddTime = 0x02,
	AddKey = 0x03
};

class CollectItemTriggerComponent : public TriggerComponent
{
public:
	CollectItemTriggerComponent() { }
	~CollectItemTriggerComponent() { }

	bool VInit(const XmlNode &configNode) override
	{
		m_ItemUsage = static_cast<ItemUsage>(std::stoi(configNode.GetChildrenNode("Usage").GetValue()));
		m_NumPoints = std::stoi(configNode.GetChildrenNode("NumPoints").GetValue());

		return true;
	}
	void VUpdate(float deltaTime) override
	{
		auto pTransformComponent = m_pParentActor._Get()->GetActorComponent<TransformComponent>();

		DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationZ(DirectX::XM_PIDIV2 * deltaTime);

		pTransformComponent->SetWorldMatrix(DirectX::XMMatrixMultiply(rotation,
			pTransformComponent->GetWorldMatrix()));
	}
	void VTriggerEvent(bool status, Actor *pActor) override
	{
		g_EventManager.PushEvent(EventPtr<CollectItemEventData>(static_cast<UINT>(m_ItemUsage), 
			m_NumPoints, m_pParentActor._Get()->GetActorID(), pActor->GetActorID()));
	}

	// Save trigger component into xml config file
	void VSaveToXML(XmlNode &configNode) override
	{
		XmlNode componentNode = configNode.CreateChildNode("CollectItemTriggerComponent");
		componentNode.CreateAttribute("type", "TriggerComponent");
		componentNode.CreateChildNode("Usage", std::to_string(static_cast<int>(m_ItemUsage)));
		componentNode.CreateChildNode("NumPoints", std::to_string(m_NumPoints));
	}
private:
	UINT m_NumPoints;
	ItemUsage m_ItemUsage;
};