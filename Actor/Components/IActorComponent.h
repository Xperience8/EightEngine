#pragma once

class Actor;
class XmlNode;

class IActorComponent
{
public:
	IActorComponent() { }
	virtual ~IActorComponent() { }

	// Creating selected component from xml config file
	virtual bool VInit(const XmlNode &configNode) = 0;
	
	// If component has depedencies on other components in this call it can be fully initialized
	virtual bool VPostInit() = 0;

	// Updating component
	virtual void VUpdate(float deltaTime) = 0;

	// Set parent actor
	// Some components do not need parent component
	virtual void VSetActor(const std::weak_ptr<Actor> &pActor) = 0;

	// Save component into xml config file
	virtual void VSaveToXML(XmlNode &configNode) = 0;

	// Get component name(e.g "RenderComponent", "AudioComponent" etc.)
	virtual const std::string &VGetComponentType() const = 0;
};