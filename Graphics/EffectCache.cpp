#include "stdafx.h"
#include "EffectCache.h"
#include "EightEngine\ResourceCache/ResourceCache.h"

#include "EightEngine\Graphics\ApiWrapper\States\DepthStencilState.h"
#include "EightEngine\Graphics\ApiWrapper\States\RasterizerState.h"
#include "EightEngine\Graphics\ApiWrapper\States\BlendState.h"
#include "EightEngine\Graphics\ApiWrapper\InputLayout.h"
#include "EightEngine\Graphics\ApiWrapper\PrimitiveTopology.h"
#include "EightEngine\Graphics\RenderingStatesCache.h"


bool EffectCache::Init(const XmlNode &configNode, 
	const std::shared_ptr<RenderingStatesCache> &pRenderingStatesCache)
{
	if (!g_ResourceCache.PreloadResources(configNode.GetAttribute("PAKFile").GetValue()))
	{
		EIGHT_ERROR("Loading shaders failed");
		return false;
	}

	m_pEffectConfig = g_ResourceCache.RequestResource<XmlDocument>(
		configNode.GetAttribute("effectConfigXML").GetValue());

	if (!CreateEffects(pRenderingStatesCache))
	{
		EIGHT_ERROR("Creating effects from shaders failed");
		return false;
	}

	EIGHT_LOG("Effects are successfully configured and they are ready to go");
	return true;
}

bool EffectCache::CreateEffects(
	const std::shared_ptr<RenderingStatesCache> &pRenderingStatesCache)
{
	auto effectNode = m_pEffectConfig->GetRootNode().GetChildrenNode();
	while (effectNode.Exist())
	{
		std::string effectName = effectNode.GetName();
		auto &pEffect = m_EffectMap[effectName];
		if (!pEffect)
		{
			pEffect = std::make_shared<Effect>();

			auto parameterNode = effectNode.GetChildrenNode();
			while (parameterNode.Exist())
			{
				std::string parameterName = parameterNode.GetName();
				if (parameterName == "InputLayout")
				{
					pEffect->SetInputLayout(g_ResourceCache.RequestResource<InputLayout>(
						parameterNode.GetValue()));
					//pEffect->SetInputLayout(pResourceCache->RequestResource<InputLayout>(
					//	parameterNode.GetValue()));
				}
				else if (parameterName == "PrimitiveTopology")
				{
					pEffect->SetPrimitiveTopology(pRenderingStatesCache->GetPrimitiveTopology(
						parameterNode.GetValue()));
				}
				else if (parameterName == "Shader")
				{
					//pEffect->SetShader(g_ResourceCache.RequestResource<ShaderBase>(
					//	"Shaders/" + std::string(parameterNode.GetValue()) + ".cso"));
					pEffect->SetShader(g_ResourceCache.RequestResource<ShaderBase>(
						"Shaders/" + std::string(parameterNode.GetValue()) + ".cso"));
					//pEffect->SetShader(pResourceCache->RequestResource<ShaderBase>(
					//	"Shaders/" + std::string(parameterNode.GetValue()) + ".cso"));
				}
				else if (parameterName == "RasterizerState")
				{
					pEffect->SetRasterizerState(pRenderingStatesCache->GetRasterizerState(
						parameterNode.GetValue()));
				}
				else if (parameterName == "DepthStencilState")
				{
					pEffect->SetDepthStencilState(pRenderingStatesCache->GetDepthStencilState(
						parameterNode.GetValue()));
				}
				else if (parameterName == "BlendState")
				{
					pEffect->SetBlendState(pRenderingStatesCache->GetBlendState(
						parameterNode.GetValue()));
				}
				else {
					EIGHT_ASSERT(false, "Unsupported parameter in creating effect");
				}

				parameterNode.GoToNextNode();
			}
		}

#if defined(_DEBUG)
		else EIGHT_WARNING("You are trying to create effect which is allready in memory!");
#endif
		pEffect->PostInit();
		effectNode.GoToNextNode();
	}


	return true;
}