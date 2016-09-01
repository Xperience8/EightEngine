#include "stdafx.h"
#include "LocalizationMap.h"
#include "EightEngine\Xml\XmlDocument.h"
#include "EightEngine\Xml\XmlNode.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\ResourceCache\ResourceHandle.h"

LocalizationMap g_LocalizationMap;

bool LocalizationMap::Init(const XmlNode &configNode)
{
	EIGHT_ASSERT(configNode.Exist(), "Configuration node for localization map is empty");

	std::string language = "Language/" + configNode.GetValue() + ".xml";

	if (!g_ResourceCache.LoadResource(language, XmlResourceLoader::s_SIGNATURE))
	{
		EIGHT_ERROR("Loading localization map " + language + " failed");
		return false;
	}

	auto pConfig = g_ResourceCache.RequestResource<XmlDocument>(language);
	XmlNode stringNode = pConfig->GetRootNode().GetChildrenNode();
	while (stringNode.Exist())
	{
		m_LocalizationMap.emplace(stringNode.GetName(), stringNode.GetValue());
		stringNode.GoToNextNode();
	}
	
	EIGHT_LOG("Localization map is fully initialized");
	return true;
}