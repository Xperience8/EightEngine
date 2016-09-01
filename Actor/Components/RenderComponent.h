#pragma once

#include "EightEngine\Actor\Actor.h"
#include "EightEngine\Graphics\IRenderableEntity.h"

class GameCamera;

class RenderComponent : public IActorComponent, public IRenderableEntity
{
public:
	static const std::string s_COMPONENT_NAME;

	RenderComponent() { }
	~RenderComponent() { }

	// Rendering in debug mode
	void VRenderDebug(Renderer &renderer) { }

	// Set parent actor
	// Render component needs parent actor
	void VSetActor(const std::weak_ptr<Actor> &pActor) override
	{
		m_pParentActor = pActor;
	}

	// Returns "RenderComponent"
	const std::string &VGetComponentType() const override { return s_COMPONENT_NAME; }

	// Access parent actor from outside
	std::weak_ptr<Actor> &GetActor() { return m_pParentActor; }
protected:
	std::weak_ptr<Actor> m_pParentActor;
};