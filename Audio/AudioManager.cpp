#include "stdafx.h"
#include "AudioManager.h"
#include "Audio.h"
#include "EightEngine\ResourceCache\ResourceCache.h"

AudioManager g_AudioManager;

bool AudioManager::StartUp()
{
	std::unique_ptr<IResourceLoader> pAudioLoader =
		std::make_unique<AudioLoader>();
	g_ResourceCache.AddLoader(pAudioLoader, AudioLoader::s_SIGNATURE);

	return InitApiSpecific();
}
void AudioManager::ShutDown()
{
	StopPlaying();
	m_SoundList.clear();
}

void AudioManager::UnpausePlaying()
{
	for (auto sound : m_SoundList)
	{
		sound->Unpause();
	}
}
void AudioManager::PausePlaying()
{
	for (auto sound : m_SoundList)
	{
		sound->Pause();
	}
}
void AudioManager::StopPlaying()
{
	for (auto sound : m_SoundList)
	{
		sound->Stop();
	}
}