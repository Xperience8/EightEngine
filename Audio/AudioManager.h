#pragma once

#include "XAudioManager.h"

class Audio;

class AudioManager : public XAudioManager
{
public:
	AudioManager() { }
	~AudioManager() { StopPlaying(); }
	
	// Basic audio config, create sound list and create mastering voice
	bool StartUp();

	void ShutDown();

	// Sound is added to the sound list
	// This is usefull for mass stopping or pausing
	void AttachSound(std::shared_ptr<Audio> &pAudio)
	{
		m_SoundList.push_back(pAudio);
	}

	// Pause every audio which plays
	void PausePlaying();

	// Unpause every audio which was paused 
	void UnpausePlaying();

	// Stop playing audio
	void StopPlaying();

private:
	std::list<std::shared_ptr<Audio> > m_SoundList;
};


extern AudioManager g_AudioManager;