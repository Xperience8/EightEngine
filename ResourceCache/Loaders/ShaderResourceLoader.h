#pragma once

#include "IResourceLoader.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\ShaderBase.h"

class RenderingStatesCache;
class XmlDocument;
class XmlNode;

class ShaderResourceLoader : public IResourceLoader
{
private:
	std::shared_ptr<XmlDocument> m_pShaderConfig;
	std::shared_ptr<RenderingStatesCache> m_pRenderingStatesCache;

	void CreateInputLayoutsDesc();

	void CreateEmptyShader(ShaderType ShaderType, std::shared_ptr<ShaderBase> &pShader);
	bool ConfigureShader(const BYTE *pRawMemory, UINT memorySize,
		std::shared_ptr<ShaderBase> &pShader, const XmlNode &configNode);
public:
	static const UINT s_SIGNATURE = 0xDEADC0DD;
	ShaderResourceLoader(const std::shared_ptr<RenderingStatesCache> &pRenderingStatesCache,
		const std::string &shaderConfigName);
	~ShaderResourceLoader() { }
	
	bool VCreateResource(BYTE *pRawMemory, UINT MemorySize, const std::string &Name,
		std::unique_ptr<ResourceHandleBase> &pResource) override;
};