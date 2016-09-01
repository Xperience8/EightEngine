#pragma once

#include "PhysicsComponent.h"

class PhysXDynamicPhysicsComponent : public PhysicsComponent
{
public:
	PhysXDynamicPhysicsComponent() : m_pRigidDynamic(nullptr) { }
	~PhysXDynamicPhysicsComponent()
	{
		if (m_pRigidDynamic)
		{
			g_Physics.GetScene()->removeActor(*m_pRigidDynamic, false);
			m_pRigidDynamic->release();
		}
	}

	// Dynamic rigid body is created and linked with actor, shapes are attached,
	// linear and angular damping is set, mas and inertia tensor are computed
	// and rigid body is added into physical scene
	bool VPostInit() override;

	// Add torque on object
	// Multiple torques can be combined
	void AddTorque(const DirectX::XMFLOAT3 &torque)
	{
		physx::PxVec3 pxTorque(torque.x, torque.y, torque.z);
		m_pRigidDynamic->addTorque(pxTorque);
	}

	// Add torque on object
	// Multiple torques can be combined
	void AddTorque(DirectX::FXMVECTOR torque)
	{
		physx::PxVec3 pxTorque;
		DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&pxTorque), torque);

		m_pRigidDynamic->addTorque(pxTorque);
	}

	// Add force on object
	// Multiple forces can be combined
	void AddForce(const DirectX::XMFLOAT3 &force)
	{
		physx::PxVec3 pxForce(force.x, force.y, force.z);
		m_pRigidDynamic->addForce(pxForce);
	}

	void AddForce(DirectX::FXMVECTOR force)
	{
		physx::PxVec3 pxForce;
		DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&pxForce), force);

		m_pRigidDynamic->addForce(pxForce);
	}

	// Clear every torque which acts on this object
	void ClearTorque()
	{
		m_pRigidDynamic->clearTorque();
	}

	// Clear every force which acts on this object
	void ClearForce()
	{
		m_pRigidDynamic->clearForce();
	}

	// Velocity is set to zero and every force is cleared
	void PutToSleep()
	{
		m_pRigidDynamic->putToSleep();
	}

	// Get actual velocity of this object
	DirectX::XMFLOAT3 GetVelocity()
	{
		auto velocity = m_pRigidDynamic->getLinearVelocity();

		return DirectX::XMFLOAT3(velocity.x, velocity.y, velocity.z);
	}

	DirectX::XMVECTOR GetVelocityXM()
	{
		auto velocity = m_pRigidDynamic->getLinearVelocity();

		return DirectX::XMVectorSet(velocity.x, velocity.y, velocity.z, 1.f);
	}

	void SetLinearDamping(float linearDamping)
	{
		m_pRigidDynamic->setLinearDamping(linearDamping);
	}

	void Stop()
	{
		m_pRigidDynamic->setLinearVelocity(physx::PxVec3(0.f, 0.f, 0.f));
		//m_pRigidDynamic->setAngularVelocity(physx::PxVec3(0.f, 0.f, 0.f));
	}

	bool Sleeps() const
	{
		return m_pRigidDynamic->isSleeping();
	}
	void SetPosition(DirectX::FXMVECTOR position)
	{
		DirectX::XMFLOAT3 pxForce;
		DirectX::XMStoreFloat3(&pxForce, position);
		
		m_pRigidDynamic->setGlobalPose(physx::PxTransform(pxForce.x, pxForce.y, pxForce.z));
	}

	void VSaveToXML(XmlNode &configNode) override;

	void VApplyTransform(DirectX::FXMVECTOR translation, DirectX::FXMVECTOR rotQuat) override;
private:
	physx::PxRigidDynamic *m_pRigidDynamic;
};

typedef PhysXDynamicPhysicsComponent DynamicPhysicsComponent;
