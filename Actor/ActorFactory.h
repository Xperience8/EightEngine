#pragma once

#include "EightEngine\Actor\Actor.h"
#include "EightEngine\Helpers\GenericClassFactory.h"

class XmlNode;

class ActorFactory
{
public:
	ActorFactory();
	~ActorFactory();
	
	// Creating actor from xml config file
	// Parameters:
	// actorXmlName -> name of actors config file, config file must be allready in memory
	// actorType -> type of actor(static, dynamic), actually physics subsystem and editor used it
	// configOverrideNode -> in level file we can change attribute of actor(such as his position, orientation etc.)
	std::shared_ptr<Actor> CreateActor(const std::string &actorXmlName, 
		UINT actorType, const XmlNode &configOverrideNode);

	// Modyfy existing actor
	// Actually we can modify only entire component or change actors config file
	void ModifyActor(const XmlNode &configNode, const std::shared_ptr<Actor> &pActor);

	UINT GetLastActorId() const { return m_LastActorID; }
protected:
	GenericClassFactory<IActorComponent> m_GenericComponentFactory;

	UINT m_LastActorID;


	void CreateActorComponentsMap(XmlNode configNode, XmlNode configOverrideNode,
		std::vector<XmlNode> &componentConfigs);
};