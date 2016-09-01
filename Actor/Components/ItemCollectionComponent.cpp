#include "stdafx.h"
#include "ItemCollectionComponent.h"
#include "EightEngine\Actor\Actor.h"
#include "EightEngine\Actor\Components\UIRenderComponent.h"
#include "EightEngine\Graphics\UserInterface\HUD.h"
#include "EightEngine\Graphics\UserInterface\TextMessageBox.h"
#include "EightEngine\Graphics\UserInterface\BackgroundImage.h"
#include "EightEngine\String\LocalizationMap.h"
#include "EightEngine\ResourceCache\ResourceCache.h"

const std::string ItemCollectionComponent::s_COMPONENT_NAME = "CollectionComponent";

inline std::string ToString(float value, int decimalPlaces)
{
	decimalPlaces += decimalPlaces == 0 ? 0 : 1;

	std::string strValue = std::to_string(value);
	return strValue.substr(0, strValue.find_last_of('.') + decimalPlaces);
}

bool ItemCollectionComponent::VPostInit()
{ 
	auto pUIComponent = m_pParentActor._Get()->GetActorComponent<UIRenderComponent>();
	if (pUIComponent)
	{
		auto guiElements = pUIComponent->GetUserInterface()->VGetGuiElements();

		auto pTimeGui = std::static_pointer_cast<TextMessageBox>(
			guiElements[static_cast<UINT>(ItemUsage::AddTime) - 1]);
		pTimeGui->GetTextSprite()->ChangeText(ToString(m_TimeLeft, 2));

		auto pPointsGui = std::static_pointer_cast<TextMessageBox>(
			guiElements[static_cast<UINT>(ItemUsage::AddPoints) - 1]);
		pPointsGui->GetTextSprite()->ChangeText(g_LocalizationMap["IDDS_NUM_POINTS"] +
			std::to_string(m_NumPoints));
	}

	return true; 
}
#include "AudioComponent.h"
void ItemCollectionComponent::CollectItemEvent(const EventPtr<IEventData> &pEvent)
{
	auto pEventData = StaticPointerCast<CollectItemEventData>(pEvent);

	auto pAudioComponent = m_pParentActor._Get()->GetActorComponent<AudioComponent>();
	EIGHT_ASSERT(pAudioComponent, "ItemCollectionComponent needs audio component");

	std::shared_ptr<Audio> pAudio;
	switch (static_cast<ItemUsage>(pEventData->GetItemUsage()))
	{
	case ItemUsage::AddKey:
		--m_KeysLeft;
		if (m_KeysLeft == 0)
		{
			// Send event to spawn exit door
			g_EventManager.PushEvent(EventPtr<OpenExitDoorEventData>(true));
		}
		{
			auto pUIComponent = m_pParentActor._Get()->GetActorComponent<UIRenderComponent>();
			if (pUIComponent)
			{
				auto guiElements = pUIComponent->GetUserInterface()->VGetGuiElements();

				auto pKeyGui = std::static_pointer_cast<BackgroundImage>(
					guiElements[guiElements.size() - m_KeysLeft - 1]);
				pKeyGui->GetSprite()->ChangeTexture(g_ResourceCache.RequestResource<Texture2D>(
					"Textures/KeyCollected.dds"));
			}
		}

		pAudio = pAudioComponent->GetAudio("PickupKey");
		break;
	case ItemUsage::AddPoints:
		m_NumPoints += pEventData->GetNumPoints();

		pAudio = pAudioComponent->GetAudio("PickupCoin");
		break;
	case ItemUsage::AddTime:
		m_TimeLeft += pEventData->GetNumPoints();

		pAudio = pAudioComponent->GetAudio("AddTime");
		break;
	}

	pAudio->Play();
}
void ItemCollectionComponent::ComputePlayerReward(const EventPtr<IEventData> &pEvent)
{
	UINT numStars = 1;

	for (auto &reward : m_Rewards)
	{
		if (m_NumPoints >= reward.EarnedPoints &&
			m_TimeLeft >= reward.TimeLeft &&
			numStars < reward.NumStars)
		{
			numStars = reward.NumStars;
		}
	}

	g_EventManager.PushEvent(EventPtr<WinEventData>(numStars));
}