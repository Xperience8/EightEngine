#pragma once

#include "PhysicsComponent.h"

class PhysXStaticPhysicsComponent : public PhysicsComponent
{
public:
	PhysXStaticPhysicsComponent() : m_pRigidStatic(nullptr) { }
	~PhysXStaticPhysicsComponent()
	{
		// If physics subsystem still exist
		if (m_pRigidStatic)
		{
			g_Physics.GetScene()->removeActor(*m_pRigidStatic, false);
			m_pRigidStatic->release();
		}
	}

	// Static rigid body is created and linked with actor,
	// shapes are attached and rigid body is added into physical scene
	bool VPostInit() override;

	void VSaveToXML(XmlNode &configNode) override;

	void VApplyTransform(DirectX::FXMVECTOR translation, DirectX::FXMVECTOR rotQuat) override { }
private:
	physx::PxRigidStatic *m_pRigidStatic;
};

typedef PhysXStaticPhysicsComponent StaticPhysicsComponent;