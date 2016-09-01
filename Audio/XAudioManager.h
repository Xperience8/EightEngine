#pragma once

class XAudioManager
{
public:
	IXAudio2 *GetXAudio() { return m_pXAudio; }

protected:
	bool InitApiSpecific();

	XAudioManager() : m_pXAudio(nullptr), m_pXAudioMasteringVoice(nullptr) { }
	~XAudioManager()
	{
		if (m_pXAudioMasteringVoice)
		{
			m_pXAudioMasteringVoice->DestroyVoice();
		}
		if (m_pXAudio)
		{
			m_pXAudio->Release();
		}
	}

private:
	IXAudio2 *m_pXAudio;
	IXAudio2MasteringVoice *m_pXAudioMasteringVoice;
};