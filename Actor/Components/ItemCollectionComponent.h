#pragma once

#include "IActorComponent.h"
#include "CollectItemTriggerComponent.h"
#include "EightEngine\Xml\XmlNode.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\OpenExitDoorEventData.h"
#include "EightEngine\EventManager\Events\LooseEventData.h"
#include "EightEngine\EventManager\Events\WinEventData.h"

class ItemCollectionComponent : public IActorComponent
{
public:
	static const std::string s_COMPONENT_NAME;

	ItemCollectionComponent() : m_NumPoints(0), m_KeysLeft(0), m_TimeLeft(0.f) 
	{ 
		g_EventManager.AddListener(EventListener(this, &ItemCollectionComponent::CollectItemEvent),
			CollectItemEventData::s_TYPE);
		g_EventManager.AddListener(EventListener(this, &ItemCollectionComponent::ComputePlayerReward),
			ComputePlayerRewardEventData::s_TYPE);
	}
	~ItemCollectionComponent() 
	{ 
		g_EventManager.RemoveListener(reinterpret_cast<UINT>(this), CollectItemEventData::s_TYPE);
		g_EventManager.RemoveListener(reinterpret_cast<UINT>(this), ComputePlayerRewardEventData::s_TYPE);
	}

	bool VInit(const XmlNode &configNode) override
	{
		m_TimeLeft = std::stof(configNode.GetChildrenNode("CompletitionTime").GetValue());
		m_KeysLeft = std::stoi(configNode.GetChildrenNode("NeededKeys").GetValue());

		XmlNode rewardNode = configNode.GetChildrenNode("RewardRequirements");
		rewardNode.GoToChildrenNode();

		while (rewardNode.Exist())
		{
			RewardRequirement reward;
			reward.NumStars = std::stoi(rewardNode.GetChildrenNode("Stars").GetValue());
			reward.EarnedPoints = std::stoi(rewardNode.GetChildrenNode("EarnedPoints").GetValue());
			reward.TimeLeft = std::stof(rewardNode.GetChildrenNode("TimeLeft").GetValue());

			m_Rewards.push_back(reward);

			rewardNode.GoToNextNode();
		}

		return true;
	}

	bool VPostInit() override;

	void VUpdate(float deltaTime) override 
	{ 
		m_TimeLeft -= deltaTime;
		if (m_TimeLeft <= 0.f)
		{
			// Send event that time ended
			g_EventManager.PushEvent(EventPtr<LooseEventData>());
		}

		VPostInit();
	}

	void VSetActor(const std::weak_ptr<Actor> &pActor) override 
	{
		m_pParentActor = pActor;
	}

	void VSaveToXML(XmlNode &configNode) override 
	{ 
		XmlNode componentNode = configNode.CreateChildNode("ItemCollectionComponent");
		componentNode.CreateAttribute("type", "CollectionComponent");
		componentNode.CreateChildNode("CompletitionTime", std::to_string(m_TimeLeft));
		componentNode.CreateChildNode("NeededKeys", std::to_string(m_KeysLeft));
	}

	const std::string &VGetComponentType() const override 
	{ 
		return s_COMPONENT_NAME; 
	}

private:
	void CollectItemEvent(const EventPtr<IEventData> &pEvent);
	void ComputePlayerReward(const EventPtr<IEventData> &pEvent);

	std::weak_ptr<Actor> m_pParentActor;
	UINT m_NumPoints;
	UINT m_KeysLeft;
	float m_TimeLeft;

	struct RewardRequirement
	{
		UINT NumStars;
		UINT EarnedPoints;
		float TimeLeft;
	};

	std::vector<RewardRequirement> m_Rewards;
};