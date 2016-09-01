#pragma once

#include "IActorComponent.h"
#include "EightEngine\Audio\Audio.h"

class AudioComponent : public IActorComponent
{
public:
	static const std::string s_COMPONENT_NAME;

	AudioComponent() { }
	~AudioComponent() { }

	// Load from xml file table which links specific sound with specific game command
	bool VInit(const XmlNode &configNode) override;

	// Audio component has no depedencies on other components so this call is empty
	bool VPostInit() override { return true; }

	// Audio component does not do anything only holds pointers to audio, so this call is empty
	void VUpdate(float DeltaTime) override { }

	// Audio component doesn't need pointer to parend actor so this call is empty
	void VSetActor(const std::weak_ptr<Actor> &pActor) override { }

	// Save audio component into xml config file(actuall status is saved)
	void VSaveToXML(XmlNode &configNode) override;

	// "AudioComponent" is returned
	const std::string &VGetComponentType() const override { return s_COMPONENT_NAME; }

	// Find audio with specific name in hash map
	// If audio with this name is not in map empty pointer is returned
	// Parameters:
	// audioType -> this is not audio full name, but this is name of game command
	std::shared_ptr<Audio> GetAudio(const std::string &audioType) 
	{
		auto sound = m_SoundMap.find(audioType);
		if (sound == m_SoundMap.end())
		{
			return std::shared_ptr<Audio>();
		}
		return sound->second.pAudio; 
	}
private:
	enum class AudioType
	{
		RegularSound = 0x00,
		BackgroundSound = 0x01,
	};
	struct AudioDesc
	{
		std::shared_ptr<Audio> pAudio;
		UINT LoopCount;
		AudioType Type;
	};

	std::unordered_map<std::string, AudioDesc> m_SoundMap;
};