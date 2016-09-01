#include "stdafx.h"
#include "Actor.h"

#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\Xml\XmlNode.h"
#include "EightEngine\Xml\XmlDocument.h"

void Actor::SaveToXml(XmlNode &configNode)
{
	const std::string configResourceName = m_pConfigResource->GetRootNode(
		).GetAttribute("resource").GetValue();

	auto actorNode = configNode.CreateChildNode("Actor");
	actorNode.CreateAttribute("resource", configResourceName);
	actorNode.CreateAttribute("instance", "Actor_" + std::to_string(m_ActorID));

	GetActorComponent("TransformComponent")->VSaveToXML(actorNode);
}
void Actor::SaveToXml(XmlDocument &configDoc)
{
	const std::string configResourceName = m_pConfigResource->GetRootNode(
		).GetAttribute("resource").GetValue();

	configDoc.Init("Actor");
	auto actorNode = configDoc.GetRootNode();
	actorNode.CreateAttribute("resource", configResourceName);
	actorNode.CreateAttribute("instance", "Actor_" + std::to_string(m_ActorID));

	for (auto &component : m_ActorComponentMap)
	{
		component.second->VSaveToXML(actorNode);
	}
}
