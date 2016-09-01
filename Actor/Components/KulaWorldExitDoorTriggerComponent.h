#pragma once

#include "EightEngine\Actor\Components\TriggerComponent.h"

#include "EightEngine\EventManager\IEventData.h"
#include "EightEngine\EventManager\Events\OpenExitDoorEventData.h"
#include "EightEngine\EventManager\Events\WinEventData.h"
#include "EightEngine\EventManager\EventManager.h"

class KulaWorldExitDoorTriggerComponent : public TriggerComponent
{
public:
	KulaWorldExitDoorTriggerComponent() 
	{ 
		g_EventManager.AddListener(EventListener(this,
			&KulaWorldExitDoorTriggerComponent::OpenExitDoorEvent),
			OpenExitDoorEventData::s_TYPE);
	}
	~KulaWorldExitDoorTriggerComponent() 
	{ 
		g_EventManager.RemoveListener(reinterpret_cast<UINT>(this),
			OpenExitDoorEventData::s_TYPE);
	}

	bool VInit(const XmlNode &configNode) override;

	bool VPostInit() override 
	{ 
		ModifyMaterialColor(DirectX::XMLoadFloat4(&m_ClosedColor));

		return true; 
	}

	void VUpdate(float deltaTime) override;

	void VTriggerEvent(bool status, Actor *pActor) override
	{
		if (!m_ClosedDoor)
		{
			//g_EventManager.PushEvent(EventPtr<WinEventData>());
			g_EventManager.PushEvent(EventPtr<ComputePlayerRewardEventData>());
		}
	}

	// Save trigger component into xml config file
	void VSaveToXML(XmlNode &configNode) override;

private:
	void OpenExitDoorEvent(const EventPtr<IEventData> &pEvent);
	void ModifyMaterialColor(DirectX::FXMVECTOR color);

	DirectX::XMFLOAT4 m_ClosedColor;
	DirectX::XMFLOAT4 m_OpenedColor;

	float m_AmbientScaleFactor;
	bool m_ClosedDoor;
};