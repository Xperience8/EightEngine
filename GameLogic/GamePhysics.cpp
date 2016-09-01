#include "stdafx.h"
#include "GamePhysics.h"
#include "EightEngine\Actor\Actor.h"
#include "EightEngine\Actor\Components\TransformComponent.h"
#include "EightEngine\Actor\Components\ModelRenderComponent.h"
#include "EightEngine\Actor\Components\PhysicsComponent.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\EventManager\Events\ConnectToPVDEventData.h"
#include "EightEngine\Actor\Components\TriggerComponent.h"
#include "EightEngine\EventManager\Events\PhysCollisionEventData.h"

PhysXGamePhysics g_Physics;

physx::PxFilterFlags TestFilterShader(
	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	if (physx::PxFilterObjectIsTrigger(attributes0) ||
		physx::PxFilterObjectIsTrigger(attributes1))
	{
		//pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		pairFlags = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
		return physx::PxFilterFlag::eDEFAULT;
	}

	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
	pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return physx::PxFilterFlag::eDEFAULT;
}



void SimulationEventCallback::onContact(const physx::PxContactPairHeader &pairHeader, 
	const physx::PxContactPair *pairs, physx::PxU32 nbPairs) 
{
	for (UINT i = 0; i < nbPairs; i++)
	{
		auto &pair = pairs[i];
		if (pair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			auto pActor1 = reinterpret_cast<Actor*>(pairHeader.actors[0]->userData);
			auto pActor2 = reinterpret_cast<Actor*>(pairHeader.actors[1]->userData);

			g_EventManager.PushEvent(EventPtr<PhysCollisionEventData>(pActor1, pActor2));
		}
	}
}

void SimulationEventCallback::onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count)
{
	for (UINT i = 0; i < count; i++)
	{
		if (pairs[i].flags & physx::PxTriggerPairFlag::Enum::eDELETED_SHAPE_OTHER,
			pairs[i].flags & physx::PxTriggerPairFlag::Enum::eDELETED_SHAPE_TRIGGER)
		{
			continue;
		}

		auto &pair = pairs[i];

		auto pTriggerActor = reinterpret_cast<Actor*>(pairs[i].triggerActor->userData);
		auto pOtherActor = reinterpret_cast<Actor*>(pairs[i].otherActor->userData);

		bool status = pair.status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND ? true : false;
		pTriggerActor->GetActorComponent<TriggerComponent>()->VTriggerEvent(status, pOtherActor);
	}
}

bool PhysXGamePhysics::StartUp()
{
	// Create physx foundation
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
	if (!m_pFoundation)
	{
		EIGHT_ERROR("Creating physx foundation failed");
		return false;
	}

	bool trackAllocations = false;
#if defined _DEBUG
	trackAllocations = true;
#endif

	// Create physics
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxTolerancesScale(), trackAllocations);
	if (!m_pPhysics)
	{
		EIGHT_ERROR("Creating physx physics failed");
		return false;
	}

	// Create cpu dispatcher
	m_pCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	if (!m_pCpuDispatcher)
	{
		EIGHT_ERROR("Creating Cpu dispatcher failed");
		return false;
	}

	// Create scene with default gravity and specific filter shader
	// ActiveTransforms are fast way for transfering new computed transforms from physx to transform component
	physx::PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.f, -9.81f, 0.f);
	sceneDesc.cpuDispatcher = m_pCpuDispatcher;
	sceneDesc.filterShader = TestFilterShader;
	sceneDesc.flags = physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
	m_pScene = m_pPhysics->createScene(sceneDesc);
	if (!m_pScene)
	{
		EIGHT_ERROR("Creating physics scene failed");
		return false;
	}

	// Create controller manager for kinematic actors
	m_pControllerManager = PxCreateControllerManager(*m_pScene);
	if (!m_pControllerManager)
	{
		EIGHT_ERROR("Creating physics controler manager failed");
		return false;
	}

	// If collision occures etc.
	m_pScene->setSimulationEventCallback(&m_SimulationEventCallback);

	// Load and create materials
	if (!LoadMaterials())
	{
		return false;
	}

	g_EventManager.AddListener(EventListener(this, &PhysXGamePhysics::ConnectToPVD),
		ConnectToPVDEventData::s_TYPE);

	std::unique_ptr<IResourceLoader> pPhysicsModelLoader = std::make_unique<PhysicsModelResourceLoader>();
	g_ResourceCache.AddLoader(pPhysicsModelLoader, PhysicsModelResourceLoader::s_SIGNATURE);

	m_ElapsedTime = 0.f;

	return true;
}
void PhysXGamePhysics::ShutDown()
{
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this), 
		ConnectToPVDEventData::s_TYPE);

	m_CollisionMaterialMap.clear();

	if (m_pCpuDispatcher)
		m_pCpuDispatcher->release();
	if (m_pControllerManager)
		m_pControllerManager->release();
	if (m_pScene)
		m_pScene->release();
	if (m_pPhysics)
		m_pPhysics->release();
	if (m_pFoundation)
		m_pFoundation->release();
}
bool PhysXGamePhysics::LoadMaterials()
{
	if (!g_ResourceCache.LoadResource("PhysicsMaterials.xml",
		XmlResourceLoader::s_SIGNATURE))
	{
		EIGHT_ERROR("File with information about physical materials can not be loaded");
		return false;
	}

	auto pDoc = g_ResourceCache.RequestResource<XmlDocument>(
		"PhysicsMaterials.xml");

	auto physicsMaterials = pDoc->GetRootNode().GetChildrenNode("PhysicsMaterials");
	if (physicsMaterials.Exist())
	{
		auto materialNode = physicsMaterials.GetChildrenNode();
		while (materialNode.Exist())
		{
			// Load material description
			float staticFriction = std::stof(materialNode.GetChildrenNode(
				"StaticFriction").GetValue());
			float dynamicFriction = std::stof(materialNode.GetChildrenNode(
				"DynamicFriction").GetValue());
			float restitution = std::stof(materialNode.GetChildrenNode(
				"Restitution").GetValue());

			// Create material and insert it into map
			auto pMaterial = m_pPhysics->createMaterial(
				staticFriction, dynamicFriction, restitution);
			if (!pMaterial)
			{
				EIGHT_ERROR("Creating physical material failed");
				return false;
			}

			m_CollisionMaterialMap.emplace(materialNode.GetName(), pMaterial);

			materialNode.GoToNextNode();
		}
	}

	EIGHT_LOG("Physical materials were successfully loaded, created and initialized");
	return true;
}
void PhysXGamePhysics::ConnectToPVD(const EventPtr<IEventData> &pEvent)
{
	if (m_pPhysics->getVisualDebugger()->getPvdConnection())
	{
		EIGHT_WARNING("You are allready connected into physx visual debugger, reconection is not needed");
		return;
	}

	if (!m_pPhysics->getPvdConnectionManager())
	{
		EIGHT_ERROR("Connecting to physX visual debugger failed");
		return;
	}

	physx::PxVisualDebuggerConnection *pDebuger = physx::PxVisualDebuggerExt::createConnection(
		m_pPhysics->getPvdConnectionManager(), "127.0.0.1", 5425, 10);
	if (pDebuger)
	{
		pDebuger->release();
	}

	m_pPhysics->getVisualDebugger()->setVisualizeConstraints(true);
	m_pPhysics->getVisualDebugger()->setVisualDebuggerFlag(
		physx::PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
	
	EIGHT_LOG("Connection with physX visual debugger was successfully established");
}

void PhysXGamePhysics::Update(float DeltaTime)
{

	// Update
	m_pScene->simulate(DeltaTime);
	m_pScene->fetchResults(true);

	// And copy new transform into transform component

	UINT activeTransformsCount;
	const physx::PxActiveTransform *pActiveTransforms = m_pScene->getActiveTransforms(activeTransformsCount);

	for (UINT i = 0; i < activeTransformsCount; i++)
	{
		auto pActor = reinterpret_cast<Actor*>(pActiveTransforms[i].userData);
		if (pActor)
		{
			const physx::PxTransform &pxTransform = pActiveTransforms[i].actor2World;
			
			pActor->GetActorComponent<PhysicsComponent>()->VApplyTransform(
				DirectX::XMVectorSet(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z, 1.f),
				DirectX::XMVectorSet(pxTransform.q.x, pxTransform.q.y, pxTransform.q.z, pxTransform.q.w));
		}
	}
}