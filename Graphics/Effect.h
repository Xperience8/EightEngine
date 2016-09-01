#pragma once

#include "EightEngine\Graphics\ApiWrapper\Shaders\ShaderBase.h"

class RenderingDeviceContext;

class RasterizerState;
class BlendState;
class DepthStencilState;

class InputLayout;
class PrimitiveTopology;

class Effect
{
public:
	Effect() : m_Shaders(static_cast<UINT>(ShaderType::ShadersCount)) { }
	~Effect() { }

	// Shaders and states which did not be set during initialization
	// will be replaced with default ones(e.g null geometry shader, null compute shader, default rasterizer state...)
	void PostInit();

	// Binding vertex, hull, domain, geometry, pixel and compute shader 
	// with constant buffers and samplers states to the vertex, hull, domain, geometry, 
	// pixel and compute stage
	// Binding input layout and primitive topology to the input assembler stage
	// Binding rasterizer state to the rasterizer stage
	// Binding depth stencil state and blend state to the output merger stage 
	void Bind(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext);

	// Setting input layout to the effect
	// Parameters:
	// pInputLayout -> smart pointer to input layout, it can not be null
	//void SetInputLayout(const std::shared_ptr<InputLayout> &pInputLayout)
	//{
	//	m_pInputLayout = pInputLayout;
	//}
	void SetInputLayout(const std::shared_ptr<InputLayout> &pInputLayout)
	{
		m_pInputLayout = pInputLayout;
	}
	// Setting primitive topology to the effect
	// Parameters:
	// pPrimitiveTopology -> smart pointer to primitive topology, it can not be null
	void SetPrimitiveTopology(const std::shared_ptr<PrimitiveTopology> &pPrimitiveTopology)
	{
		m_pPrimitiveTopology = pPrimitiveTopology;
	}
	// Setting shader to the effect
	// Parameters:
	// pShader -> smart pointer to shader, it can not be null
	//void SetShader(std::shared_ptr<ShaderBase> &pShader)
	//{
	//	m_Shaders[static_cast<UINT>(pShader->GetShaderType())] = pShader;
	//}
	void SetShader(std::shared_ptr<ShaderBase> &pShader)
	{
		m_Shaders[static_cast<UINT>(pShader->GetShaderType())] = pShader;
	}
	// Setting rasterizer state to the effect
	// Parameters:
	// pRasterizerState -> smart pointer to the rasterizer state, it can not be null
	void SetRasterizerState(const std::shared_ptr<RasterizerState> &pRasterizerState)
	{
		m_pRasterizerState = pRasterizerState;
	}
	// Setting depth stencil state to the effect
	// Parameters:
	// pDepthStencilState -> smart pointer to the depth stencil state, it can not be null
	void SetDepthStencilState(const std::shared_ptr<DepthStencilState> &pDepthStencilState)
	{
		m_pDepthStencilState = pDepthStencilState;
	}
	// Setting blend state to the effect
	// Parameters:
	// pBlendState -> smart pointer to the blend state, it can not be null
	void SetBlendState(const std::shared_ptr<BlendState> &pBlendState)
	{
		m_pBlendState = pBlendState;
	}
private:
	//std::vector<std::shared_ptr<ShaderBase> > m_Shaders;
	std::vector<std::shared_ptr<ShaderBase> > m_Shaders;
	std::shared_ptr<InputLayout> m_pInputLayout;



	std::shared_ptr<RasterizerState> m_pRasterizerState;
	std::shared_ptr<DepthStencilState> m_pDepthStencilState;
	std::shared_ptr<BlendState> m_pBlendState;
	std::shared_ptr<PrimitiveTopology> m_pPrimitiveTopology;
};
