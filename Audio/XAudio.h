#pragma once

class AudioManager;
class VoiceCallback;

class XAudio
{
protected:
	XAudio() : m_pSourceVoice(nullptr) { }
	~XAudio()
	{
		if (m_pSourceVoice)
		{
			m_pSourceVoice->Stop();
			m_pSourceVoice->FlushSourceBuffers();
			m_pSourceVoice->DestroyVoice();
		}
	}

	// Create source voice and fill audio buffer with audio data
	bool InitApiSpecific(const BYTE *pAudioData, UINT audioBytes,
		const WAVEFORMATEXTENSIBLE &format, AudioManager &audioManager);
	
	// To apply this on audio which allready plays you have to flushed buffers and play it again
	void SetLoopProperties(UINT loopCount, UINT loopBegin = 0, UINT loopLength = -1)
	{
		m_AudioBuffer.LoopBegin = loopBegin;
		m_AudioBuffer.LoopCount = loopCount;
		m_AudioBuffer.LoopLength = loopLength == - 1 ? 
			m_AudioBuffer.PlayBegin + m_AudioBuffer.PlayLength : loopLength;
	}
	void SubmitBuffers()
	{
		EIGHT_ASSERT(m_pSourceVoice, "Source Voice doesn't exist");
		EIGHT_ASSERT(m_AudioBuffer.pAudioData, "You are trying to submit unintialized buffer");

		HRESULT hr = m_pSourceVoice->SubmitSourceBuffer(&m_AudioBuffer);

		if (hr != S_OK)
		{
			EIGHT_WARNING("Submit buffer failed");
		}
	}
	void FlushBuffers()
	{
		EIGHT_ASSERT(m_pSourceVoice, "Source Voice doesn't exist");

		m_pSourceVoice->FlushSourceBuffers();
	}
	void StartPlaying()
	{
		EIGHT_ASSERT(m_pSourceVoice, "Source Voice doesn't exist");

		m_pSourceVoice->Start();
	}
	void StopPlaying()
	{
		EIGHT_ASSERT(m_pSourceVoice, "Source Voice doesn't exist");

		m_pSourceVoice->Stop();
		//m_pSourceVoice->FlushSourceBuffers();
	}
	void SetVolumeApiSpecific(float volume)
	{
		EIGHT_ASSERT(m_pSourceVoice, "Source Voice doesn't exist");

		m_pSourceVoice->SetVolume(volume);
	}
	void SetPitchApiSpecific(float pitch)
	{
		EIGHT_ASSERT(m_pSourceVoice, "Source Voice doesn't exist");

		m_pSourceVoice->SetFrequencyRatio(pitch);
	}

public:
	float GetVolume() const
	{
		EIGHT_ASSERT(m_pSourceVoice, "Source Voice doesn't exist");

		float volume;
		m_pSourceVoice->GetVolume(&volume);
		return volume;
	}

private:
	IXAudio2SourceVoice *m_pSourceVoice;
	XAUDIO2_BUFFER m_AudioBuffer;
};