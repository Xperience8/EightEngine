#pragma once

#include "IActorComponent.h"
#include "EightEngine\Actor\Actor.h"
#include "EightEngine\EventManager\EventManager.h"

class TriggerComponent : public IActorComponent
{
public:
	static const std::string s_COMPONENT_NAME;

	TriggerComponent() { }
	~TriggerComponent() { }

	// Trigger component doesn't need initialization therefore this call is empty
	bool VInit(const XmlNode &configNode) override
	{
		return true;
	}

	// Trigger component has no depedencies on other components therefore this call is empty
	bool VPostInit() override { return true; }

	// Trigger component only trigger event therefore this call is empty
	void VUpdate(float DeltaTime) override { }

	// Trigger event
	virtual void VTriggerEvent(bool status, Actor *pActor) = 0;

	// This is used for calling specific components callbacks
	void VSetActor(const std::weak_ptr<Actor> &pActor) override 
	{ 
		m_pParentActor = pActor;
	}
	
	// Returns "TriggerComponent"
	const std::string &VGetComponentType() const override { return s_COMPONENT_NAME; }
protected:
	std::weak_ptr<Actor> m_pParentActor;
};
