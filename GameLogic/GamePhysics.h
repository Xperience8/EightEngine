#pragma once

#pragma warning(push)
#pragma warning(disable : 4099)

class PhysXAllocator : public physx::PxAllocatorCallback
{
public:
	PhysXAllocator() { }
	~PhysXAllocator() { }

	// Allocate alligned memory
	void *allocate(size_t size, const char *typeName, 
		const char *filename, int line) override
	{
		void *pMemory = physx::platformAlignedAlloc(size);
		EIGHT_ASSERT((reinterpret_cast<size_t>(pMemory) & 15) == 0, "Allocating aligned memory failed");
		return pMemory;
	}
	void deallocate(void *pMemory) override 
	{ 
		physx::platformAlignedFree(pMemory); 
	}
};
class PhysXError : public physx::PxErrorCallback
{
private:

public:
	PhysXError() { }
	~PhysXError() { }

	void reportError(physx::PxErrorCode::Enum code, 
		const char *message, const char *filename, int line) override
	{
		EIGHT_ERROR("\tPHYSX:\nError Code: " + std::to_string(code) + 
			"\nMessage: " + message + "\nFile: " + filename + "\nLine: " 
			+ std::to_string(line) + "\n\n");
	}
};

class EventManager;
#include "EightEngine\EventManager\IEventData.h"
class ResourceCache;

class SimulationEventCallback : public physx::PxSimulationEventCallback
{
public:
	SimulationEventCallback() { }
	~SimulationEventCallback() { }

	void onConstraintBreak(physx::PxConstraintInfo *constraints, 
		physx::PxU32 count) override { }
	void onWake(physx::PxActor **actors, physx::PxU32 count) override { }
	void onSleep(physx::PxActor **actors, physx::PxU32 count) override { }
	
	// For every contact pair is created collision event which is then sent into game logic
	// Actually event is only sent when two objects start touching each other
	void onContact(const physx::PxContactPairHeader &pairHeader, 
		const physx::PxContactPair *pairs, physx::PxU32 nbPairs) override;
	
	// For every trigger pair is run trigger callback
	void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count) override;
};
class XmlNode;
class PhysXGamePhysics
{
public:
	PhysXGamePhysics() : m_pFoundation(nullptr), m_pPhysics(nullptr),
		m_pScene(nullptr), m_pControllerManager(nullptr), m_pCpuDispatcher(nullptr), m_ElapsedTime(0.f) { }
	~PhysXGamePhysics() { }

	// Physx is initialized and basic scene with basic rules is created
	// Materials are loaded from xml config file
	bool StartUp();

	void ShutDown();

	// Every dynamic rigid body is updated and new world matrix is sent into transform component
	void Update(float DeltaTime);

	physx::PxPhysics *GetPhysics() { return m_pPhysics; }
	physx::PxScene *GetScene() { return m_pScene; }
	physx::PxControllerManager *GetControllerManager() { return m_pControllerManager; }

	physx::PxMaterial *GetMaterial(const std::string &materialName)
	{
		return m_CollisionMaterialMap[materialName];
	}
	const std::string GetMaterialName(physx::PxMaterial *pPhysXMaterial)
	{
		for (auto &physMat : m_CollisionMaterialMap)
		{
			if (physMat.second == pPhysXMaterial)
			{
				return physMat.first;
			}
		}
		return "Null";
	}

private:
	std::unordered_map<std::string, physx::PxMaterial*> m_CollisionMaterialMap;

	SimulationEventCallback m_SimulationEventCallback;

	PhysXAllocator m_Allocator;
	PhysXError m_ErrorCallback;

	physx::PxFoundation *m_pFoundation;
	physx::PxPhysics *m_pPhysics;
	physx::PxScene *m_pScene;
	physx::PxControllerManager *m_pControllerManager;
	physx::PxDefaultCpuDispatcher *m_pCpuDispatcher;


	const float UPDATE_TIME = 1.f / 60.f;
	float m_ElapsedTime;


	bool LoadMaterials();
	
	// Connect to physX visual debugger
	void ConnectToPVD(const EventPtr<IEventData> &pEvent);
};

extern PhysXGamePhysics g_Physics;

#pragma warning(pop)
