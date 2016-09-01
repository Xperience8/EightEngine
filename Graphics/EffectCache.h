#pragma once

#include "Effect.h"

class RenderingStatesCache;
//class ResourceCache;
class XmlDocument;
class XmlNode;

class EffectCache
{
public:
	EffectCache() { }
	~EffectCache() { }

	// Shaders are loaded into memory and effects are created from them
	// Parameters:
	// configNode -> used for determining in which file shaders are 
	//				 and which file configure shaders and effects
	// pRenderingStatesCache -> pointer to cache with rendering states, it can not be null
	bool Init(const XmlNode &configNode,
		const std::shared_ptr<RenderingStatesCache> &pRenderingStatesCache);

	// Getting effect
	// If effect with this name doesn't exist -> is returned empty effect
	std::shared_ptr<Effect> &GetEffect(const std::string &name) 
	{ 
		return m_EffectMap[name]; 
	}

private:
	bool CreateEffects(const std::shared_ptr<RenderingStatesCache> &pRenderingStatesCache);


	std::unordered_map<std::string, std::shared_ptr<Effect> > m_EffectMap;

	// Config file used to configure and create effects
	std::shared_ptr<XmlDocument> m_pEffectConfig;
};