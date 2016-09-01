#pragma once

#include "EightEngine\Graphics\ApiWrapper\GraphicsFlags.h"

class SamplerStateBase
{
public:
	// This is used for setting sampler state properties like FilterType,
	// TextureAddressMode and Maximum Anisotropy
	// Parameters:
	// filterType -> filter type from FilterType enum
	// textureAddressMode -> texture address mode from TextureAddressMode enum
	// maxAnisotropy -> maximum anisotropy, it can not be more than zero if you
	//					do not use ANISOTROPIC_FILTER flag in filterType parameter
	// Default settings are Anisotropic filter with wrap texture address mode
	// and maximum anisotropy is 16
	void SetProperties(FilterType filterType, TextureAddressMode textureAddressMode,
		UINT maxAnisotropy = 0)
	{
		m_FilterType = filterType;
		m_TextureAddressMode = textureAddressMode;
		m_MaxAnisotropy = maxAnisotropy;

		EIGHT_ASSERT(m_FilterType == FilterType::ANISOTROPIC_FILTER && m_MaxAnisotropy > 0,
			"You can only specify maxAnisotropy if you want to use anisotropic filter");
	}
protected:
	FilterType m_FilterType;
	TextureAddressMode m_TextureAddressMode;
	UINT m_MaxAnisotropy;

	SamplerStateBase() : m_FilterType(FilterType::ANISOTROPIC_FILTER),
		m_TextureAddressMode(TextureAddressMode::TEXTURE_ADDRESS_MODE_WRAP),
		m_MaxAnisotropy(16) { }
	~SamplerStateBase() { }
};