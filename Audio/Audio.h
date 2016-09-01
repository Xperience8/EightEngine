#pragma once

#include "XAudio.h"

class VoiceCallback;

class Audio : public XAudio
{
public:
	static const UINT LOOP_INFINITE = 255;
	Audio() : m_pAudioData(nullptr), m_AudioBytes(0),
		m_IsPlaying(false), m_IsPaused(false) { }
	~Audio()
	{
		if (m_pAudioData)
		{
			delete[] m_pAudioData;
		}
	}

	// Init audio, allocate new audio buffer, fill audio buffer with audio data and create voice
	bool Init(const BYTE *pAudioData, UINT audioBytes, const WAVEFORMATEXTENSIBLE &format,
		AudioManager &audioManager)
	{
		if (m_pAudioData)
		{
			delete[] m_pAudioData;
		}

		m_AudioBytes = audioBytes;
		m_pAudioData = new BYTE[m_AudioBytes];
		memcpy(m_pAudioData, pAudioData, m_AudioBytes);

		return InitApiSpecific(m_pAudioData, m_AudioBytes, format, audioManager);
	}

	// Start playing audio and set loop count
	// If audio was not playing, buffer is submited into audio buffer and audio starts playing
	// Otherwise nothing happens
	void Play(int loopCount = 0)
	{
		if (loopCount > 0)
		{
			SetLoopProperties(loopCount);
		}
		if (!m_IsPlaying)
		{
			SubmitBuffers();
			StartPlaying();
			m_IsPlaying = true;
		}
	}

	// Pause audio
	// If audio was not playing, nothing happens
	void Pause()
	{
		if (m_IsPlaying && !m_IsPaused)
		{
			StopPlaying();
			m_IsPaused = true;
		}
	}

	// Continue to played paused audio
	// If audio was not paused, nothing happens
	void Unpause()
	{
		if (m_IsPlaying && m_IsPaused)
		{
			StartPlaying();
			m_IsPaused = false;
		}
	}

	// Stop playing audio and flushed buffers(remove every buffer from queue)
	void Stop()
	{
		if (m_IsPlaying)
		{
			StopPlaying();
			FlushBuffers();

			m_IsPlaying = false;
			m_IsPaused = false;
		}
	}

	// 0.f means complete silence
	// 1.f means no gain and no attenuation
	void SetVolume(float volume)
	{
		SetVolumeApiSpecific(volume);
	}

	void SetFrequencyRatio(float ratio)
	{
		SetPitchApiSpecific(ratio);
	}
private:
	friend VoiceCallback;
private:
	BYTE *m_pAudioData;
	UINT m_AudioBytes;

	bool m_IsPlaying;
	bool m_IsPaused;


	std::string m_AudioName;
public:
	void SetDebugName(const std::string &audioName)
	{
		m_AudioName = audioName;
	}
	const std::string &GetDebugName() const
	{
		return m_AudioName;
	}
};