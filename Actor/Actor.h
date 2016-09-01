#pragma once

#include "EightEngine\Actor\Components\IActorComponent.h"
const UINT INVALID_ACTOR_ID = 0;

class XmlDocument;
class XmlNode;
class ISceneNode;


enum ActorType 
{ 
	STATIC_ACTOR, 
	DYNAMIC_ACTOR, 
	KINEMATIC_ACTOR, 
	TRIGGER
};

class Actor
{
private:
	std::unordered_map<std::string, std::shared_ptr<IActorComponent> > m_ActorComponentMap;

	std::shared_ptr<XmlDocument> m_pConfigResource;

	UINT m_ActorID;
	UINT m_ActorType;
public:
	Actor(UINT ID, std::shared_ptr<XmlDocument> &pConfigResource, UINT actorType) 
		: m_ActorID(ID), m_pConfigResource(pConfigResource), m_ActorType(actorType) { }
	~Actor() { }

	// Adding new component to actors component map
	// If you add multiple components with same type, the newest is saved and the other are deleted
	void AddComponent(std::shared_ptr<IActorComponent> &pActorComponent)
	{
		m_ActorComponentMap.emplace(pActorComponent->VGetComponentType(), pActorComponent);
	}

	// Removing component from actor
	// If component with this name doesn't exist or it was allready deleted, nothing happens
	void RemoveComponent(const std::string &componentName)
	{
		m_ActorComponentMap.erase(componentName);
	}

	// Run post init components initialization
	// If components are depended on each other, they can not be fully initialized in init call
	// This method finalize each component
	bool PostInit()
	{ 
		for (auto &component : m_ActorComponentMap)
		{
			if (!component.second->VPostInit())
			{
				return false;
			}
		}
		return true;
	}

	// Update every component
	void Update(float deltaTime)
	{
		for (auto &component : m_ActorComponentMap)
		{
			component.second->VUpdate(deltaTime);
		}
	}

	// Set config xml file
	// This xml file is used to generate new xml file if SaveToXml is called
	void SetConfigXml(std::shared_ptr<XmlDocument> &pConfigResource)
	{
		m_pConfigResource = pConfigResource;
	}

	// Saving actor to xml file
	// Whole actor is saved into xml file with actual states
	void SaveToXml(XmlNode &configNode);
	void SaveToXml(XmlDocument &configNode);

	// Changing actor type
	// For editors purposes
	void ChangeActorType(UINT Type) { m_ActorType = Type; }

	// Get actor component and cast it to appropriate type
	// If requested component doesn't exist, nullptr is returned
	template <typename Component>
	std::shared_ptr<Component> GetActorComponent() const
	{
		auto component = m_ActorComponentMap.find(Component::s_COMPONENT_NAME);
		if (component != m_ActorComponentMap.end())
		{
			return std::static_pointer_cast<Component>(component->second);
		}
		return std::shared_ptr<Component>();
	}
	// Get actor component by name
	// If requested component doesn't exist, nullptr is returned
	std::shared_ptr<IActorComponent> GetActorComponent(
		const std::string &componentName) const
	{
		auto component = m_ActorComponentMap.find(componentName);
		if (component != m_ActorComponentMap.end())
		{
			return component->second;
		}
		return std::shared_ptr<IActorComponent>();
	}
	// Returning actor type(whether it is dynamic, static, trigger etc.)
	UINT GetActorType() const { return m_ActorType; }
	UINT GetActorID() { return m_ActorID; }
};