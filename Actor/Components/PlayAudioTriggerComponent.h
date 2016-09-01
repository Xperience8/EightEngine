#pragma once

#include "EightEngine\Actor\Components\TriggerComponent.h"
#include "EightEngine\Xml\XmlNode.h"

#include "AudioComponent.h"

class PlayAudioTriggerComponent : public TriggerComponent
{
public:
	PlayAudioTriggerComponent() { }
	~PlayAudioTriggerComponent() { }

	bool VInit(const XmlNode &configNode) override 
	{ 
		m_AudioName = configNode.GetChildrenNode("AudioName").GetValue();
		m_LoopCount = std::stoi(configNode.GetChildrenNode("LoopCount").GetValue());

		return true; 
	}

	void VTriggerEvent(bool status, Actor *pActor) override
	{
		auto pAudioComponent = m_pParentActor._Get()->GetActorComponent<AudioComponent>();

		pAudioComponent->GetAudio(m_AudioName)->Play(m_LoopCount);
	}

	// Save trigger component into xml config file
	void VSaveToXML(XmlNode &configNode) override
	{
		XmlNode componentNode = configNode.CreateChildNode("ChangeGravityTriggerComponent");
		componentNode.CreateAttribute("type", "TriggerComponent");

		componentNode.CreateChildNode("AudioName", m_AudioName);
		componentNode.CreateChildNode("LoopCount", std::to_string(m_LoopCount));
	}
private:
	std::string m_AudioName;
	UINT m_LoopCount;
};