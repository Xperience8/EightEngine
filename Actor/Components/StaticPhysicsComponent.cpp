#include "stdafx.h"
#include "StaticPhysicsComponent.h"
#include "TransformComponent.h"
#include "EightEngine\Math\DirectXToPhysXMath.h"
#include "EightEngine\Xml\XmlNode.h"

bool PhysXStaticPhysicsComponent::VPostInit()
{
	auto pTransformComponent = m_pParentActor._Get()->GetActorComponent<TransformComponent>();

	m_pRigidStatic = g_Physics.GetPhysics()->createRigidStatic(
		physx::PxTransform(DirectXToPhysx(pTransformComponent->GetWorldMatrix())));

	if (!m_pRigidStatic)
	{
		EIGHT_ERROR("Creating static rigid body failed");
		return false;
	}

	for (auto &shape : m_pPhysicsModel->GetShapes())
	{
		m_pRigidStatic->attachShape(*shape.GetPhysXShape());
	}

	// userData are used in filter shader or in collision callbacks
	m_pRigidStatic->userData = m_pParentActor._Get();

	g_Physics.GetScene()->addActor(*m_pRigidStatic);

	EIGHT_LOG("Static rigid body was successfully created, initialized and added into physics subsystem");
	
	return true;
}

void PhysXStaticPhysicsComponent::VSaveToXML(XmlNode &configNode)
{
	XmlNode componentNode = configNode.CreateChildNode("StaticPhysicsComponent");
	componentNode.CreateAttribute("type", "PhysicsComponent");

	PhysicsComponent::VSaveToXML(componentNode);
}