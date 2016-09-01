#include "stdafx.h"
#include "XAudioManager.h"

bool XAudioManager::InitApiSpecific()
{
	if (FAILED(XAudio2Create(&m_pXAudio)))
	{
		EIGHT_ERROR("Initializing XAudio2 failed");
		return false;
	}
	
	XAUDIO2_DEBUG_CONFIGURATION debugConfig;
	debugConfig.TraceMask = XAUDIO2_LOG_ERRORS;
	debugConfig.LogFileline = true;
	debugConfig.LogFunctionName = true;
	debugConfig.LogThreadID = true;
	debugConfig.LogTiming = true;
	m_pXAudio->SetDebugConfiguration(&debugConfig);

	if (FAILED(m_pXAudio->CreateMasteringVoice(&m_pXAudioMasteringVoice)))
	{
		EIGHT_ERROR("Creating mastering voice failed");
		return false;
	}

	EIGHT_LOG("XAudio2 and mastering voice were successfully initialized");
	return true;
}