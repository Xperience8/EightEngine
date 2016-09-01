#pragma once

#include "EightEngine\Actor\Components\TriggerComponent.h"
#include "EightEngine\EventManager\Events\WinEventData.h"

class WinTriggerComponent : public TriggerComponent
{
public:
	WinTriggerComponent() { }
	~WinTriggerComponent() { }

	// Send event that actor is in the end of the level(WinEventData)
	void VTriggerEvent(bool status, Actor *pActor) override
	{
		g_EventManager.PushEvent(EventPtr<WinEventData>());
	}

	// Save trigger component into xml config file
	void VSaveToXML(XmlNode &configNode) override
	{
		XmlNode componentNode = configNode.CreateChildNode("WinTriggerComponent");
		componentNode.CreateAttribute("type", "TriggerComponent");
	}
};