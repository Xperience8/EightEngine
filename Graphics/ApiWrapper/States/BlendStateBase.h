#pragma once

#include "EightEngine\Graphics\ApiWrapper\GraphicsFlags.h"

class BlendStateBase
{
protected:
	DirectX::XMFLOAT4 m_BlendFactor;
	UINT m_SampleMask;

	ColorWriteEnable m_RenderTargetWriteMask;

	Blend m_SourceBlend;
	Blend m_DestinationBlend;
	BlendOp m_BlendOp;

	Blend m_SourceBlendAlpha;
	Blend m_DestinationBlendAlpha;
	BlendOp m_BlendOpAlpha;

	BlendStateBase() : m_SampleMask(0xffffffff), m_BlendFactor(0.f, 0.f, 0.f, 0.f),
		m_RenderTargetWriteMask(ColorWriteEnable::COLOR_WRITE_ENABLE_ALL),
		m_SourceBlend(Blend::BLEND_ONE), m_DestinationBlend(Blend::BLEND_ZERO),
		m_BlendOp(BlendOp::BLEND_OP_ADD), m_SourceBlendAlpha(Blend::BLEND_ONE),
		m_DestinationBlendAlpha(Blend::BLEND_ZERO), m_BlendOpAlpha(BlendOp::BLEND_OP_ADD)
	{

	}
	~BlendStateBase() { }

public:
	// Setting source blend, destination blend and blend operation for color channels
	// Default parameters are:
	// source blend is one
	// destination blend is zero
	// and blend operation is add
	// Parameters:
	// sourceBlend -> source blend for color channels
	// destinationBlend -> destination blend for color channels
	// blendOp -> blending operation for color channels
	// You can call this method only before Create method is called
	void SetBlend(Blend sourceBlend, Blend destinationBlend, BlendOp blendOp)
	{
		m_SourceBlend = sourceBlend;
		m_DestinationBlend = destinationBlend;
		m_BlendOp = blendOp;
	}
	// Setting source blend, destination blend and blend operation for alpha channel
	// Default parameters are:
	// source blend alpha is one
	// destination blend alpha is zero
	// and blend operation alpha is add
	// Parameters:
	// sourceBlendAlpha -> source blend for alpha channel
	// destinationBlendAlpha -> destination blend for alpha channel
	// blendOpAlpha -> blending operation for alpha channel
	// You can call this method only before Create method is called
	void SetBlendAlpha(Blend sourceBlendAlpha, Blend destinationBlendAlpha,
		BlendOp blendOpAlpha)
	{
		m_SourceBlendAlpha = sourceBlendAlpha;
		m_DestinationBlendAlpha = destinationBlendAlpha;
		m_BlendOpAlpha = blendOpAlpha;
	}
	// Setting render target write mask
	// This mean that you can block writing to specific color channel
	// Default parameter is that writing to all channels is enabled
	// You can call this method only before Create method is called
	void SetRenderTargetWriteMask(ColorWriteEnable renderTargetWriteMask)
	{
		m_RenderTargetWriteMask = renderTargetWriteMask;
	}
	// Setting sample mask
	// Default paramater is 0xffffffff
	void SetSampleMask(UINT sampleMask)
	{
		m_SampleMask = sampleMask;
	}
	// Setting blend factor
	// Blend factor is used in blending equation(only if blend factor is specified in 
	// source blend, destination blend...)
	// Default it is zero
	void SetBlendFactor(const DirectX::XMFLOAT4 &blendFactor)
	{
		m_BlendFactor = blendFactor;
	}
};