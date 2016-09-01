#pragma once

class SamplerState;
class RasterizerState;
class DepthStencilState;
class BlendState;
class PrimitiveTopology;
class RenderingDevice;

class RenderingStatesCache
{
private:
	std::unordered_map<std::string, std::shared_ptr<SamplerState> > m_SamplerStatesMap;
	std::unordered_map<std::string, std::shared_ptr<RasterizerState> > m_RasterizerStatesMap;
	std::unordered_map<std::string, std::shared_ptr<DepthStencilState> > m_DepthStencilStatesMap;
	std::unordered_map<std::string, std::shared_ptr<BlendState> > m_BlendStatesMap;
	std::unordered_map<std::string, std::shared_ptr<PrimitiveTopology> > m_PrimitiveTopologiesMap;


	bool CreateSamplerStates(const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	bool CreateRasterizerStates(const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	bool CreateDepthStencilStates(const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	bool CreateBlendStates(const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void CreatePrimitiveTopologies();
public:
	RenderingStatesCache() { }
	~RenderingStatesCache() { }

	bool Init(const std::shared_ptr<RenderingDevice> &pRenderingDevice);

	const std::shared_ptr<SamplerState> &GetSamplerState(
		const std::string &samplerStateName) const
	{
		auto &pSamplerState = m_SamplerStatesMap.find(samplerStateName);
		if (pSamplerState == m_SamplerStatesMap.end())
		{
			EIGHT_ERROR("Requested sampler state doesn't exist: " + samplerStateName);
			return m_SamplerStatesMap.find("DefaultSamplerState")->second;
		}

		return (*pSamplerState).second;
	}

	const std::shared_ptr<RasterizerState> &GetRasterizerState(
		const std::string &rasterizerStateName) const
	{
		auto &pRasterizerState = m_RasterizerStatesMap.find(rasterizerStateName);
		if (pRasterizerState == m_RasterizerStatesMap.end())
		{
			EIGHT_ERROR("Requested rasterizer state doesn't exist: " + rasterizerStateName);
			return m_RasterizerStatesMap.find("DefaultRasterizerState")->second;
		}

		return (*pRasterizerState).second;
	}

	const std::shared_ptr<DepthStencilState> &GetDepthStencilState(
		const std::string &depthStencilStateName) const
	{
		auto &pDepthStencilState = m_DepthStencilStatesMap.find(depthStencilStateName);
		if (pDepthStencilState == m_DepthStencilStatesMap.end())
		{
			EIGHT_ERROR("Requested depth stencil state doesn't exist: " + depthStencilStateName);
			return m_DepthStencilStatesMap.find("DefaultDepthStencilState")->second;
		}

		return (*pDepthStencilState).second;
	}

	const std::shared_ptr<BlendState> &GetBlendState(
		const std::string &blendStateName) const
	{
		auto &pBlendState = m_BlendStatesMap.find(blendStateName);
		if (pBlendState == m_BlendStatesMap.end())
		{
			EIGHT_ERROR("Requested blend state doesn't exist: " + blendStateName);
			return m_BlendStatesMap.find("DefaultBlendState")->second;
		}

		return (*pBlendState).second;
	}

	const std::shared_ptr<PrimitiveTopology> &GetPrimitiveTopology(
		const std::string &primitiveTopologyName) const
	{
		auto &pPrimitiveTopology = m_PrimitiveTopologiesMap.find(primitiveTopologyName);
		if (pPrimitiveTopology == m_PrimitiveTopologiesMap.end())
		{
			EIGHT_ERROR("Requested primitive topology doesn't exist: " + primitiveTopologyName);
			return m_PrimitiveTopologiesMap.find("DefaultPrimitiveTopology")->second;
		}

		return (*pPrimitiveTopology).second;
	}
};