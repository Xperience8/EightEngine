#pragma once

inline std::wstring ToWstring(const std::string &String) { return std::wstring(String.begin(), String.end()); }

#if defined(_DEBUG)
	inline void DrawError(std::wstring File, UINT Line, std::wstring Function)
	{
		std::wstring finalMessage = L"\nFile: " + File + L"\nLine: " + std::to_wstring(Line) + L"\nFunction: " + Function;
		MessageBox(nullptr, finalMessage.c_str(), L"Error", MB_OK | MB_ICONERROR);
	}
	#define ThrowIfFailed(hr) { if (FAILED((hr))) { DrawError(__FILEW__, __LINE__, __FUNCTIONW__); return false; } }
#else
	#define ThrowIfFailed(hr) hr
#endif

#ifndef D3D11_SAFE_RELEASE

#if defined(_DEBUG)
#define D3D11_SAFE_RELEASE(relesableObject) \
	if (relesableObject) \
						{ \
		relesableObject->Release(); \
		relesableObject = nullptr; \
				} 
#else
#define D3D11_SAFE_RELEASE(relesableObject) \
	if (relesableObject) \
			{ \
		relesableObject->Release(); \
			} 
#endif

#endif