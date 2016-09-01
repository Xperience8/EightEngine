#include "stdafx.h"
#include "GameApp.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\Xml\XmlDocument.h"
#include "EightEngine\String\LocalizationMap.h"

bool GameApp::CheckOneInstance(const std::string &appName)
{
	// If there is allready running instance of this app then this instance is closed
	if (CreateMutexA(nullptr, false, appName.c_str()))
	{
		HWND hwnd = FindWindowA(appName.c_str(), appName.c_str());
		if (hwnd)
		{
			ShowWindow(hwnd, SW_SHOW);
			SetFocus(hwnd);
			SetForegroundWindow(hwnd);

			EIGHT_LOG("There is allready running instance of this application therefore this instance will be closed");
			return false;
		}
	}
	return true;
}
bool GameApp::CheckRequirements(const XmlNode &configNode)
{
	ULONG minPhysMemory = std::stoul(configNode.GetAttribute("physRAM_MB").GetValue());
	ULONG minVirtualMemory = std::stoul(configNode.GetAttribute("virtualRAM_MB").GetValue());
	if (!CheckRAMSize(minPhysMemory, minVirtualMemory))
	{
		EIGHT_ERROR("There isn't enought memory");
		return false;
	}
	
	ULONG minHddSpace = std::stoul(configNode.GetAttribute("HDDSpace_MB").GetValue());
	if (!CheckHDDSize(minHddSpace))
	{
		EIGHT_ERROR("There isn't enought hdd space");
		return false;
	}

	EIGHT_LOG("Requirements checking is done, this app is ready to start");
	return true;
}
bool GameApp::CheckRAMSize(ULONG neededPhysMemory, ULONG neededVirtualMemory)
{
	// Get memory status and check whether there is enought virtual and physical memory
	// If it is not, show message box to the user
	
	MEMORYSTATUSEX memoryStatus;
	memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
	if (!GlobalMemoryStatusEx(&memoryStatus))
	{
		EIGHT_ERROR("There is a problem with getting memory status");
		return false;
	}

	if (neededPhysMemory > memoryStatus.ullAvailPhys / (1024 * 1024))
	{
		MessageBox(nullptr, ToWstring(g_LocalizationMap["IDDS_PHYS_RAM"]).c_str(), 
			ToWstring(g_LocalizationMap["IDDS_ERROR"]).c_str(), MB_OK | MB_ICONERROR);
		return false;
	}
	if (neededVirtualMemory > memoryStatus.ullAvailVirtual / (1024 * 1024))
	{
		MessageBox(nullptr, ToWstring(g_LocalizationMap["IDDS_VIRTUAL_RAM"]).c_str(),
			ToWstring(g_LocalizationMap["IDDS_ERROR"]).c_str(), MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}
bool GameApp::CheckHDDSize(ULONG NeededMemory)
{
	// Get disk space status
	// If there is not enought disk space, show message box to the user
	ULARGE_INTEGER freeBytes;
	GetDiskFreeSpaceEx(nullptr, &freeBytes, nullptr, nullptr);
	
	if (NeededMemory > freeBytes.QuadPart / (1024 * 1024))
	{
		MessageBox(nullptr, ToWstring(g_LocalizationMap["IDDS_HDD_SIZE"]).c_str(), 
			ToWstring(g_LocalizationMap["IDDS_ERROR"]).c_str(), MB_OK | MB_ICONERROR);
		return false;
	}
	
	return true;
}