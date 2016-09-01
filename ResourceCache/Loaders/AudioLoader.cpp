#include "stdafx.h"
#include "AudioLoader.h"
#include "EightEngine\ResourceCache\ResourceHandle.h"
#include "EightEngine\Memory\MemoryReader.h"

#include "EightEngine\Audio\Audio.h"
#include "EightEngine\Audio\AudioManager.h"

const DWORD RIFF = 'FFIR';
const DWORD WAVE = 'EVAW';
const DWORD FMT = ' tmf';
const DWORD DATA = 'atad';

bool AudioLoader::VCreateResource(BYTE *pRawData, UINT rawDataSize, 
	const std::string &resourceName, std::unique_ptr<ResourceHandleBase> &pResourceHandle)
{
	MemoryReader reader;
	reader.LoadFromMemoryStream(pRawData, rawDataSize);

	DWORD fileType = reader.ReadMemory<DWORD>();
	DWORD fileSize = reader.ReadMemory<DWORD>();

	if (fileType != RIFF)
	{
		EIGHT_WARNING("Audio resource: " + resourceName + " has not RIFF as the first bytes");
		return false;
	}
	fileType = reader.ReadMemory<DWORD>();
	if (fileType != WAVE)
	{
		EIGHT_WARNING("Audio resource: " + resourceName + " is not WAVE format");
		return false;
	}

	WAVEFORMATEXTENSIBLE waveFormatDesc;
	BYTE *pAudioData = nullptr;
	UINT audioBytes = 0;

	while (!reader.IsEnd())
	{
		DWORD flag = reader.ReadMemory<DWORD>();
		DWORD bufferSize = reader.ReadMemory<DWORD>();
		BYTE *buffer = reader.ReadMemoryArray<BYTE>(bufferSize);

		switch (flag)
		{
		case FMT:
			waveFormatDesc = *reinterpret_cast<WAVEFORMATEXTENSIBLE*>(buffer);
			break;
		case DATA:
			pAudioData = buffer;
			audioBytes = bufferSize;
			break;
		}
	}

	// If resource handle was not requested, create it
	if (!pResourceHandle)
	{
		pResourceHandle = std::make_unique<ResourceHandle<Audio>>(resourceName);
	}
	std::shared_ptr<Audio> &pAudio = static_cast<ResourceHandle<Audio> *>(
		pResourceHandle.get())->GetResource();

	if (!pAudio->Init(pAudioData, audioBytes, waveFormatDesc, g_AudioManager))
	{
		EIGHT_WARNING("Can not initialize audio " + resourceName);
		return false;
	}
	g_AudioManager.AttachSound(pAudio);

	pAudio->SetDebugName(resourceName);

	pResourceHandle->PostInit();

	EIGHT_LOG("Audio: " + resourceName + " is loaded and initialized");

	return true;
}

// What I need

// Background music ==> Loop forever
// A sound for sphere moving
// A sound for sphere hitting the wall       -> Theoretically it is done but it need optimizations
// A sound for sphere falling
