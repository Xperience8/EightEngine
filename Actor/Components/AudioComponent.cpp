#include "stdafx.h"
#include "AudioComponent.h"
#include "EightEngine\Actor\ActorFactory.h"
#include "EightEngine\ResourceCache\ResourceCache.h"

const std::string AudioComponent::s_COMPONENT_NAME = "AudioComponent";

bool AudioComponent::VInit(const XmlNode &configNode)
{
	auto soundsNode = configNode.GetChildrenNode("Sounds");
	if (soundsNode.Exist())
	{
		auto soundNode = soundsNode.GetChildrenNode();
		while (soundNode.Exist())
		{
			// This is name of game command like walking, running etc.
			std::string name = soundNode.GetChildrenNode("Name").GetValue();
			// And this is name of audio which will be played after this game command will be requested
			std::string resourceName = soundNode.GetChildrenNode("Resource").GetValue();

			// Getting audio from resource cache
			// Audio doesn't have to be in memory right now
			AudioDesc desc;
			desc.pAudio = g_ResourceCache.RequestResource<Audio>(resourceName);
			desc.LoopCount = std::stoi(soundNode.GetChildrenNode("Loop").GetValue());
			desc.Type = static_cast<AudioType>(std::stoi(
				soundNode.GetChildrenNode("Type").GetValue()));

			// Link game command with audio 
			m_SoundMap.emplace(name, desc);

			soundNode.GoToNextNode();

			if (desc.Type == AudioType::BackgroundSound)
			{
				desc.pAudio->Play(desc.LoopCount);
			}
		}
	}

	return true;
}
void AudioComponent::VSaveToXML(XmlNode &configNode)
{
	XmlNode componentNode = configNode.CreateChildNode("AudioComponent");
	componentNode.CreateAttribute("type", "AudioComponent");

	XmlNode soundsNode = componentNode.CreateChildNode("Sounds");
	soundsNode.CreateAttribute("count", std::to_string(m_SoundMap.size()));

	for (auto &sound : m_SoundMap)
	{
		XmlNode soundNode = soundsNode.CreateChildNode("Sound");
		soundNode.CreateChildNode("Name", sound.first);
		soundNode.CreateChildNode("Resource", sound.second.pAudio->GetDebugName());
		soundNode.CreateChildNode("Loop", std::to_string(sound.second.LoopCount));
		soundNode.CreateChildNode("SoundType", std::to_string(static_cast<UINT>(sound.second.Type)));
	}
}