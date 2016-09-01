#include "stdafx.h"
#include "CharacterPhysicsComponent.h"
#include "TransformComponent.h"
#include "EightEngine\Math\DirectXToPhysXMath.h"
#include "EightEngine\Xml\XmlNode.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\JumpEventData.h"
#include "EightEngine\EventManager\Events\PhysCollisionEventData.h"

void ControllerHitReport::onShapeHit(const physx::PxControllerShapeHit &hit)
{
	m_pPhysicsComponent->SetContactNormal(DirectX::XMVectorSet(hit.worldNormal.x,
		hit.worldNormal.y, hit.worldNormal.z, 0.f));
}


bool PhysXCharacterPhysicsComponent::VPostInit()
{
	EIGHT_ASSERT(m_pPhysicsModel->GetShapes().size() < 2, "Character can have only one shape");

	physx::PxShape *pShape = m_pPhysicsModel->GetShapes()[0].GetPhysXShape();
	physx::PxMaterial **pMaterial = new physx::PxMaterial*;
	pShape->getMaterials(pMaterial, sizeof(physx::PxMaterial*));
	
	physx::PxSphereGeometry geometry;
	pShape->getSphereGeometry(geometry);

	auto pTransformComponent = m_pParentActor._Get()->GetActorComponent<TransformComponent>();
	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, pTransformComponent->GetWorldMatrix().r[3]);

	physx::PxCapsuleControllerDesc controllerDesc;
	controllerDesc.density = m_Description.Density;
	controllerDesc.material = *pMaterial;
	controllerDesc.radius = geometry.radius;
	controllerDesc.position = physx::PxExtendedVec3(position.x, position.y, position.z);
	controllerDesc.userData = m_pParentActor._Get();
	controllerDesc.height = 0.001f;
	controllerDesc.stepOffset = 0.01f;
	controllerDesc.contactOffset = 0.001f;
	controllerDesc.reportCallback = &m_HitReport;

	delete pMaterial;
	
	m_pController = static_cast<physx::PxCapsuleController*>(
		g_Physics.GetControllerManager()->createController(controllerDesc));

	if (!m_pController)
	{
		EIGHT_ERROR("Creating controller failed");
		return false;
	}

	m_pController->getActor()->userData = m_pParentActor._Get();

	g_Physics.GetScene()->setGravity(physx::PxVec3(0.f, -9.81f, 0.f));

	return true;
}
void PhysXCharacterPhysicsComponent::VSaveToXML(XmlNode &configNode)
{
	XmlNode componentNode = configNode.CreateChildNode("CharacterPhysicsComponent");
	componentNode.CreateAttribute("type", "PhysicsComponent");

	PhysicsComponent::VSaveToXML(componentNode);
}
void PhysXCharacterPhysicsComponent::VApplyTransform(DirectX::FXMVECTOR translation, 
	DirectX::FXMVECTOR rotQuat)
{
	auto pTransformComponent = m_pParentActor._Get()->GetActorComponent<TransformComponent>();

	DirectX::XMMATRIX transform = pTransformComponent->GetWorldMatrix();
	transform.r[3] = translation;

	pTransformComponent->SetWorldMatrix(transform);
}
void PhysXCharacterPhysicsComponent::MoveAngular(DirectX::FXMVECTOR displacementVector,
	DirectX::FXMVECTOR rotationAxis)
{
	const float MAX_ANGULAR_SPEED = (1.f / 60.f) * 0.75f;

	float radius = m_pController->getRadius();

	float linearSpeed = DirectX::XMVectorGetX(DirectX::XMVector3Length(displacementVector));

	float angularSpeed = DirectX::XMMin(linearSpeed / radius, MAX_ANGULAR_SPEED / radius);

	auto pTransformComponent = m_pParentActor._Get()->GetActorComponent<TransformComponent>();

	DirectX::XMMATRIX transform = pTransformComponent->GetWorldMatrix();

	DirectX::XMVECTOR oldPosition = transform.r[3];
	DirectX::XMVECTOR oldQuat = DirectX::XMQuaternionRotationMatrix(transform);
	DirectX::XMVECTOR newDeltaQuat = DirectX::XMQuaternionRotationAxis(rotationAxis, angularSpeed);

	transform = DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionMultiply(oldQuat, newDeltaQuat));

	transform = DirectX::XMMatrixMultiply(transform, DirectX::XMMatrixTranslationFromVector(oldPosition));

	pTransformComponent->SetWorldMatrix(transform);
}
void PhysXCharacterPhysicsComponent::ChangeGravity(DirectX::FXMVECTOR gravity)
{
	physx::PxVec3 pxGravity;
	DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&pxGravity), gravity);

	physx::PxVec3 pxUpDir;
	DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&pxUpDir),
		DirectX::XMVectorNegate(DirectX::XMVector3Normalize(gravity)));

	m_pController->setUpDirection(pxUpDir);

	g_Physics.GetScene()->setGravity(pxGravity);
}