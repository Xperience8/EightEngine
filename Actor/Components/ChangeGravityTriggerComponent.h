#pragma once

#include "EightEngine\Actor\Components\TriggerComponent.h"
#include "EightEngine\EventManager\Events\ChangeGravityEventData.h"
#include "EightEngine\Xml\XmlNode.h"

class ChangeGravityTriggerComponent : public TriggerComponent
{
public:
	ChangeGravityTriggerComponent() { }
	~ChangeGravityTriggerComponent() { }

	bool VInit(const XmlNode &configNode) override { return true; }

	void VTriggerEvent(bool status, Actor *pActor) override
	{
		g_EventManager.PushEvent(EventPtr<ChangeGravityEventData>(status, pActor->GetActorID()));
	}

	// Save trigger component into xml config file
	void VSaveToXML(XmlNode &configNode) override
	{
		XmlNode componentNode = configNode.CreateChildNode("ChangeGravityTriggerComponent");
		componentNode.CreateAttribute("type", "TriggerComponent");
	}
};
