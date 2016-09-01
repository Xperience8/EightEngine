#include "stdafx.h"
#include "DynamicPhysicsComponent.h"
#include "TransformComponent.h"
#include "EightEngine\Math\DirectXToPhysXMath.h"
#include "EightEngine\Xml\XmlNode.h"

bool PhysXDynamicPhysicsComponent::VPostInit()
{
	auto pTransformComponent = m_pParentActor._Get()->GetActorComponent<TransformComponent>();

	m_pRigidDynamic = g_Physics.GetPhysics()->createRigidDynamic(
		physx::PxTransform(DirectXToPhysx(pTransformComponent->GetWorldMatrix())));

	if (!m_pRigidDynamic)
	{
		EIGHT_ERROR("Creating dynamic rigid body failed");
		return false;
	}

	for (auto &shape : m_pPhysicsModel->GetShapes())
	{
		m_pRigidDynamic->attachShape(*shape.GetPhysXShape());
	}

	physx::PxRigidBodyExt::updateMassAndInertia(*m_pRigidDynamic, m_Description.Density);
	m_pRigidDynamic->setLinearDamping(m_Description.LinearDamping);
	m_pRigidDynamic->setAngularDamping(m_Description.AngularDamping);

	// userData are used in filter shader or in collision callbacks
	m_pRigidDynamic->userData = m_pParentActor._Get();

	g_Physics.GetScene()->addActor(*m_pRigidDynamic);

	EIGHT_LOG("Dynamic rigid body was successfully created, initialized and added into physics subsystem");

	return true;
}

void PhysXDynamicPhysicsComponent::VSaveToXML(XmlNode &configNode)
{
	XmlNode componentNode = configNode.CreateChildNode("DynamicPhysicsComponent");
	componentNode.CreateAttribute("type", "PhysicsComponent");

	PhysicsComponent::VSaveToXML(componentNode);
}
void PhysXDynamicPhysicsComponent::VApplyTransform(DirectX::FXMVECTOR translation, 
	DirectX::FXMVECTOR rotQuat) 
{ 
	m_pParentActor._Get()->GetActorComponent<TransformComponent>()->SetWorldMatrix(
		DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationQuaternion(rotQuat),
		DirectX::XMMatrixTranslationFromVector(translation)));
}