#include "stdafx.h"
#include "XAudio.h"
#include "AudioManager.h"
#include "VoiceCallback.h"

VoiceCallback gVoiceCallback;

bool XAudio::InitApiSpecific(const BYTE *pAudioData, UINT audioBytes,
	const WAVEFORMATEXTENSIBLE &format, AudioManager &audioManager)
{
	IXAudio2 *pXAudio = static_cast<XAudioManager*>(&audioManager)->GetXAudio();

	EIGHT_ASSERT(pXAudio, "Pointer to IXAudio2 doesn't exist");

	// Create source voice
	HRESULT hr = pXAudio->CreateSourceVoice(&m_pSourceVoice, 
		reinterpret_cast<const WAVEFORMATEX*>(&format), 0, 2.f, &gVoiceCallback);
	if (FAILED(hr))
	{
		EIGHT_ERROR("Can not create source voice!");
		return false;
	}

	// Save voice description
	ZeroMemory(&m_AudioBuffer, sizeof(m_AudioBuffer));
	m_AudioBuffer.pAudioData = pAudioData;
	m_AudioBuffer.AudioBytes = audioBytes;
	m_AudioBuffer.Flags = XAUDIO2_END_OF_STREAM;
	m_AudioBuffer.pContext = this;

	return true;
}