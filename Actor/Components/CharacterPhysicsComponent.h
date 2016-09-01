#pragma once

#include "PhysicsComponent.h"

class PhysXCharacterPhysicsComponent;


class ControllerHitReport : public physx::PxUserControllerHitReport
{
public:
	explicit ControllerHitReport(PhysXCharacterPhysicsComponent *pPhysicsComponent) :
		m_pPhysicsComponent(pPhysicsComponent) { }

	void onShapeHit(const physx::PxControllerShapeHit &hit) override;

	void onControllerHit(const physx::PxControllersHit &hit) override
	{

	}
	void onObstacleHit(const physx::PxControllerObstacleHit &hit) override
	{

	}
private:
	PhysXCharacterPhysicsComponent *m_pPhysicsComponent;
};


class PhysXCharacterPhysicsComponent : public PhysicsComponent
{
public:
	PhysXCharacterPhysicsComponent() : m_pController(nullptr), m_GravityScaleFactor(0.025f),
		m_HitReport(this), m_ContactNormal(0.f, 1.f, 0.f), m_ShouldFall(false) { }
	~PhysXCharacterPhysicsComponent() 
	{ 
		if (m_pController)
		{
			m_pController->release();
		}
	}

	bool VPostInit() override;

	void VSaveToXML(XmlNode &configNode) override;

	void VApplyTransform(DirectX::FXMVECTOR translation, DirectX::FXMVECTOR rotQuat) override;

	void Move(DirectX::FXMVECTOR displacementVector, float elapsedTime)
	{
		physx::PxVec3 dispVector;
		DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&dispVector), displacementVector);
		m_CollisionFlags = m_pController->move(dispVector, 0.f, elapsedTime, physx::PxControllerFilters());
	}
	void MoveAngular(DirectX::FXMVECTOR displacementVector, DirectX::FXMVECTOR rotationAxis);


	// True means falling
	bool ApplyGravity(float deltaTime)
	{
		physx::PxVec3 pxGravity = g_Physics.GetScene()->getGravity();

		DirectX::XMVECTOR gravity = DirectX::XMVectorScale(DirectX::XMVectorSet(
			pxGravity.x, pxGravity.y, pxGravity.z, 0.f), m_GravityScaleFactor * deltaTime);

		Move(gravity, deltaTime);
		
		if (!IsDownSideCollision())
		{
			m_GravityScaleFactor *= 1.1f;

			if (m_GravityScaleFactor > 0.1f)
			{
				m_GravityScaleFactor = 0.025f;
				return true;
			}
		}
		else
		{
			m_GravityScaleFactor = 0.025f;
		}
		
		gravity = DirectX::XMVector3Normalize(gravity);
		DirectX::XMVECTOR contactNormal = DirectX::XMLoadFloat3(&m_ContactNormal);		

		if (!DirectX::XMVector3NearEqual(DirectX::XMVectorNegate(contactNormal), gravity,
			DirectX::XMVectorScale(DirectX::g_XMEpsilon, 1000.f)))
		{
				DirectX::XMVECTOR movementDir = DirectX::XMVectorSubtract(contactNormal,
				DirectX::XMVectorMultiply(gravity, DirectX::XMVector3Dot(contactNormal, gravity)));
				movementDir = DirectX::XMVector3Normalize(movementDir);

				DirectX::XMVECTOR angularMovementDir = DirectX::XMVector3Cross(
				movementDir, gravity);

				DirectX::XMVECTOR disp = DirectX::XMVectorScale(movementDir, deltaTime * 0.25f);

				Move(disp, deltaTime);
				MoveAngular(disp, angularMovementDir);
		}

		return false;
	}
	void SetContactNormal(DirectX::FXMVECTOR contactNormal)
	{
		DirectX::XMStoreFloat3(&m_ContactNormal, contactNormal);
	}
	DirectX::XMVECTOR GetContactNormal() const
	{
		return DirectX::XMLoadFloat3(&m_ContactNormal);
	}


	void ChangeGravity(DirectX::FXMVECTOR gravity);

	bool IsDownSideCollision() const
	{
		return m_CollisionFlags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN);
	}
	bool IsSideCollision() const
	{
		return m_CollisionFlags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES);
	}
	bool IsCollision() const
	{
		return !m_CollisionFlags.isSet(static_cast<physx::PxControllerCollisionFlag::Enum>(0));
	}
private:
	physx::PxCapsuleController *m_pController;
	ControllerHitReport m_HitReport;

	physx::PxControllerCollisionFlags m_CollisionFlags;


	DirectX::XMFLOAT3 m_ContactNormal;

	float m_GravityScaleFactor;
	bool m_ShouldFall;
};

typedef PhysXCharacterPhysicsComponent CharacterPhysicsComponent;