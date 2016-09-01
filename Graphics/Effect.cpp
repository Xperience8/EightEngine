#include "stdafx.h"
#include "Effect.h"

#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

#include "EightEngine\Graphics\ApiWrapper\Shaders\VertexShader.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\HullShader.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\DomainShader.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\GeometryShader.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\PixelShader.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\ComputeShader.h"

#include "EightEngine\Graphics\ApiWrapper\States\RasterizerState.h"
#include "EightEngine\Graphics\ApiWrapper\States\DepthStencilState.h"
#include "EightEngine\Graphics\ApiWrapper\States\BlendState.h"

#include "EightEngine\Graphics\ApiWrapper\InputLayout.h"
#include "EightEngine\Graphics\ApiWrapper\PrimitiveTopology.h"

void Effect::PostInit()
{
	EIGHT_ASSERT(m_Shaders[static_cast<UINT>(ShaderType::VERTEX_SHADER)],
		"You can not create effect without specifying vertex shader");
	EIGHT_ASSERT(m_pInputLayout, 
		"You can not create effect without specifying input layout");
	EIGHT_ASSERT(m_pPrimitiveTopology, 
		"You can not create effect without specifying primitive topology");

	// Creating default shaders -> it's mean null shaders(shaders which bind
	// empty shader to the pipeline)
	if (!m_Shaders[static_cast<UINT>(ShaderType::HULL_SHADER)])
	{
		m_Shaders[static_cast<UINT>(ShaderType::HULL_SHADER)] =
			std::make_shared<HullShader>();
	}
	if (!m_Shaders[static_cast<UINT>(ShaderType::DOMAIN_SHADER)])
	{
		m_Shaders[static_cast<UINT>(ShaderType::DOMAIN_SHADER)] =
			std::make_shared<DomainShader>();
	}
	if (!m_Shaders[static_cast<UINT>(ShaderType::GEOMETRY_SHADER)])
	{
		m_Shaders[static_cast<UINT>(ShaderType::GEOMETRY_SHADER)] =
			std::make_shared<GeometryShader>();
	}
	if (!m_Shaders[static_cast<UINT>(ShaderType::PIXEL_SHADER)])
	{
		m_Shaders[static_cast<UINT>(ShaderType::PIXEL_SHADER)] =
			std::make_shared<PixelShader>();
	}
	if (!m_Shaders[static_cast<UINT>(ShaderType::COMPUTE_SHADER)])
	{
		m_Shaders[static_cast<UINT>(ShaderType::COMPUTE_SHADER)] =
			std::make_shared<ComputeShader>();
	}

	// Creating default rasterizer, depth stencil and blend state
	if (!m_pRasterizerState)
	{
		m_pRasterizerState = std::make_shared<RasterizerState>();
	}
	if (!m_pDepthStencilState)
	{
		m_pDepthStencilState = std::make_shared<DepthStencilState>();
	}
	if (!m_pBlendState)
	{
		m_pBlendState = std::make_shared<BlendState>();
	}
}
void Effect::Bind(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	m_pRasterizerState->Bind(pRenderingDeviceContext);
	m_pDepthStencilState->Bind(pRenderingDeviceContext);
	m_pBlendState->Bind(pRenderingDeviceContext);

	m_pInputLayout->Bind(pRenderingDeviceContext);
	m_pPrimitiveTopology->Bind(pRenderingDeviceContext);

	for (auto pShader : m_Shaders)
	{
		pShader->VBind(pRenderingDeviceContext);
	}
}



