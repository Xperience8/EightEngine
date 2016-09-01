#pragma once

#include "EightEngine\Actor\Components\TriggerComponent.h"
#include "EightEngine\EventManager\Events\LooseEventData.h"
#include "EightEngine\Actor\Components\AudioComponent.h"
#include "EightEngine\Actor\Actor.h"
#include "EightEngine\Xml\XmlNode.h"

class LooseTriggerComponent : public TriggerComponent
{
public:
	LooseTriggerComponent() { }
	~LooseTriggerComponent() { }

	void VTriggerEvent(bool status, Actor *pActor) override
	{	
		g_EventManager.PushEvent(EventPtr<LooseEventData>());

		auto pAudioComponent = pActor->GetActorComponent<AudioComponent>();
		if (pAudioComponent)
		{
			pAudioComponent->GetAudio("Death")->Play();
		}
	}
	// Save trigger component into xml config file
	void VSaveToXML(XmlNode &configNode) override
	{
		XmlNode componentNode = configNode.CreateChildNode("LooseTriggerComponent");
		componentNode.CreateAttribute("type", "TriggerComponent");
	}
};