#pragma once

#include "Audio.h"

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	void __stdcall OnVoiceProcessingPassStart(UINT32 BytesRequired) override
	{

	}
	void __stdcall OnVoiceProcessingPassEnd() override
	{

	}
	void __stdcall OnStreamEnd() override
	{

	}
	void __stdcall OnBufferStart(void *pBufferContext) override
	{

	}
	void __stdcall OnBufferEnd(void *pBufferContext) override
	{
		auto pAudio = reinterpret_cast<Audio*>(pBufferContext);
		pAudio->m_IsPlaying = false;
	}
	void __stdcall OnLoopEnd(void *pBufferContext) override
	{

	}
	void __stdcall OnVoiceError(void *pBufferContext, HRESULT Error) override
	{

	}
};