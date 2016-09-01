#include "stdafx.h"
#include "RenderingStatesCache.h"
#include "EightEngine\Graphics\ApiWrapper\States\SamplerState.h"
#include "EightEngine\Graphics\ApiWrapper\States\RasterizerState.h"
#include "EightEngine\Graphics\ApiWrapper\States\DepthStencilState.h"
#include "EightEngine\Graphics\ApiWrapper\States\BlendState.h"
#include "EightEngine\Graphics\ApiWrapper\PrimitiveTopology.h"

bool RenderingStatesCache::Init(const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	if (CreateSamplerStates(pRenderingDevice) && CreateRasterizerStates(pRenderingDevice)
		&& CreateDepthStencilStates(pRenderingDevice) && CreateBlendStates(pRenderingDevice))
	{
		CreatePrimitiveTopologies();

		EIGHT_LOG("RenderingStatesCache is fully initialized and ready to go");
		return true;
	}

	EIGHT_ERROR("Creating RenderingStatesCache failed");
	return false;
}
bool RenderingStatesCache::CreateSamplerStates(
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	m_SamplerStatesMap.emplace("DefaultSamplerState", 
		std::make_shared<SamplerState>());

	// Create, initialize and insert sampler states into sampler states map

	auto pWrapSS = std::make_shared<SamplerState>();
	pWrapSS->SetProperties(FilterType::ANISOTROPIC_FILTER,
		TextureAddressMode::TEXTURE_ADDRESS_MODE_WRAP, 16);
	if (!pWrapSS->Create(pRenderingDevice))
	{
		return false;
	}
	m_SamplerStatesMap.emplace("WrapSS", pWrapSS);

	auto pClampSS = std::make_shared<SamplerState>();
	pClampSS->SetProperties(FilterType::ANISOTROPIC_FILTER,
		TextureAddressMode::TEXTURE_ADDRESS_MODE_CLAMP, 16);
	if (!pClampSS->Create(pRenderingDevice))
	{
		return false;
	}
	m_SamplerStatesMap.emplace("ClampSS", pClampSS);

	auto pBorderSS = std::make_shared<SamplerState>();
	pBorderSS->SetProperties(FilterType::ANISOTROPIC_FILTER,
		TextureAddressMode::TEXTURE_ADDRESS_MODE_BORDER, 16);
	if (!pBorderSS->Create(pRenderingDevice))
	{
		return false;
	}
	m_SamplerStatesMap.emplace("BorderSS", pBorderSS);

	EIGHT_LOG("Sampler states are created and initialized");

	return true;
}
bool RenderingStatesCache::CreateRasterizerStates(
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	m_RasterizerStatesMap.emplace("DefaultRasterizerState", 
		std::make_shared<RasterizerState>());

	auto pRasterizerState = std::make_shared<RasterizerState>();
	pRasterizerState->SetProperties(FillMode::FILL_SOLID, CullMode::CULL_NONE);
	if (!pRasterizerState->Create(pRenderingDevice))
	{
		return false;
	}
	m_RasterizerStatesMap.emplace("NoCullRS", pRasterizerState);

	pRasterizerState = std::make_shared<RasterizerState>();
	pRasterizerState->SetProperties(FillMode::FILL_WIREFRAME, CullMode::CULL_BACK);
	if (!pRasterizerState->Create(pRenderingDevice))
	{
		return false;
	}
	m_RasterizerStatesMap.emplace("WireFrameRS", pRasterizerState);

	EIGHT_LOG("Rasterizer states are created and initialized");

	return true;
}
bool RenderingStatesCache::CreateDepthStencilStates(
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	m_DepthStencilStatesMap.emplace("DefaultDepthStencilState",
		std::make_shared<DepthStencilState>());

	auto pDepthStencilState = std::make_shared<DepthStencilState>();
	pDepthStencilState->SetDepthDesc(true, DepthWriteMask::DEPTH_WRITE_MASK_ALL,
		ComparisonFunc::COMPARISON_LESS_EQUAL);
	pDepthStencilState->SetStencilDesc(false, 0xff, 0xff);
	pDepthStencilState->SetBackFaceStencilOpDesc(StencilOp::STENCIL_OP_KEEP,
		StencilOp::STENCIL_OP_KEEP, StencilOp::STENCIL_OP_INCR,
		ComparisonFunc::COMPARISON_ALWAYS);
	pDepthStencilState->SetFrontFaceStencilOpDesc(StencilOp::STENCIL_OP_KEEP,
		StencilOp::STENCIL_OP_KEEP, StencilOp::STENCIL_OP_INCR,
		ComparisonFunc::COMPARISON_ALWAYS);
	pDepthStencilState->SetStencilRef(0);
	if (!pDepthStencilState->Create(pRenderingDevice))
	{
		return false;
	}
	m_DepthStencilStatesMap.emplace("LessEqualDSS", pDepthStencilState);

	EIGHT_LOG("Depth stencil states are created and initialized");

	return true;
}
bool RenderingStatesCache::CreateBlendStates(
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	m_BlendStatesMap.emplace("DefaultBlendState", 
		std::make_shared<BlendState>());

	auto pBlendState = std::make_shared<BlendState>();
	pBlendState->SetBlend(Blend::BLEND_SRC_ALPHA, Blend::BLEND_INV_SRC_ALPHA,
		BlendOp::BLEND_OP_ADD);
	pBlendState->SetBlendAlpha(Blend::BLEND_ONE, Blend::BLEND_ZERO,
		BlendOp::BLEND_OP_ADD);
	pBlendState->SetRenderTargetWriteMask(ColorWriteEnable::COLOR_WRITE_ENABLE_ALL);
	pBlendState->SetBlendFactor(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f));
	pBlendState->SetSampleMask(0xffffffff);
	if (!pBlendState->Create(pRenderingDevice))
	{
		return false;
	}

	m_BlendStatesMap.emplace("TransparentBS", pBlendState);

	EIGHT_LOG("Blend states are created and initialized");

	return true;
}
void RenderingStatesCache::CreatePrimitiveTopologies()
{
	m_PrimitiveTopologiesMap.emplace("Pointlist", std::make_shared<PrimitiveTopology>(
		PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_POINTLIST));
	m_PrimitiveTopologiesMap.emplace("Trianglelist", std::make_shared<PrimitiveTopology>(
		PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	m_PrimitiveTopologiesMap.emplace("4ControlPoints", std::make_shared<PrimitiveTopology>(
		PrimitiveTopologyType::PRIMITIVE_TOPOLOGY_4_CONTROL_PATCHLIST));

	EIGHT_LOG("Primitive topologies are created and initialized");

	m_PrimitiveTopologiesMap.emplace("DefaultPrimitiveTopology",
		m_PrimitiveTopologiesMap["Trianglelist"]);
}