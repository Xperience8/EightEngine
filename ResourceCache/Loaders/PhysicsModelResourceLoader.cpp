#include "stdafx.h"
#include "PhysicsModelResourceLoader.h"
#include "EightEngine\Physics\PhysicsShape.h"
#include "EightEngine\Memory\MemoryReader.h"
#include "EightEngine\GameLogic\GamePhysics.h"
#include "EightEngine\ResourceCache\ResourceHandle.h"

const UINT IDENTIFICATION_CODE = 'P' * '3' * 'D';
const UINT ACTUAL_VERSION = 0x01;

struct PhysicsFileHeader
{
	UINT IdentificationCode;
	UINT Version;
	UINT NumShapes;
};

bool PhysicsModelResourceLoader::VCreateResource(BYTE *pRawMemory, UINT memorySize,
	const std::string &resourceName, std::unique_ptr<ResourceHandleBase> &pResourceHandle)
{
	MemoryReader reader;
	reader.LoadFromMemoryStream(pRawMemory, memorySize);

	auto pFileHeader = reader.ReadMemoryArray<PhysicsFileHeader>(1);
	if (pFileHeader->IdentificationCode != IDENTIFICATION_CODE ||
		pFileHeader->Version != ACTUAL_VERSION)
	{
#if defined(_DEBUG)
		if (pFileHeader->IdentificationCode != IDENTIFICATION_CODE)
		{
			EIGHT_WARNING("Model resource: " + resourceName + " , there is a file with bad identification code hence it is file with different format");
		}
		else if (pFileHeader->Version < ACTUAL_VERSION)
		{
			EIGHT_WARNING("Model resource: " + resourceName + " has old version. Please update it on the newest version " +
				std::to_string(ACTUAL_VERSION));
		}
		else if (pFileHeader->Version > ACTUAL_VERSION)
		{
			EIGHT_WARNING("Model resource: " + resourceName + " has newer version, please update model resource loader on version" +
				std::to_string(pFileHeader->Version));
		}
#endif
		return false;
	}

	if (!pResourceHandle)
	{
		pResourceHandle = std::make_unique<ResourceHandle<PhysicsModel> >(resourceName);
	}
	auto pPhysicsModel = static_cast<ResourceHandle<PhysicsModel> *>(
		pResourceHandle.get())->GetResource();

	pPhysicsModel->SetDebugName(resourceName);

	for (UINT i = 0; i < pFileHeader->NumShapes; i++)
	{
		PhysicsShape shape;
		if (LoadPhysicsShape(reader, shape))
		{
			pPhysicsModel->AttachShape(shape);
		}
	}

	pResourceHandle->PostInit();
	EIGHT_LOG("Collision model: " + resourceName + " is loaded and initialized");
	return true;
}
bool PhysicsModelResourceLoader::LoadPhysicsShape(MemoryReader &memoryReader, PhysicsShape &physicalShape)
{
	UINT shapeType = memoryReader.ReadMemory<UINT>();
	BYTE geometryType = memoryReader.ReadMemory<BYTE>();

	USHORT collMaterialNameSize = memoryReader.ReadMemory<USHORT>();
	std::string collMaterialName(memoryReader.ReadMemoryArray<char>(
		collMaterialNameSize), collMaterialNameSize);

	physx::PxVec3 offset = memoryReader.ReadMemory<physx::PxVec3>();
	physx::PxQuat quat = memoryReader.ReadMemory<physx::PxQuat>();

	std::shared_ptr<IPhysicsGeometry> pPhysicsGeometry;

	switch (geometryType)
	{
	case physx::PxGeometryType::eBOX:
	{
		DirectX::BoundingBox bb = memoryReader.ReadMemory<DirectX::BoundingBox>();

		auto pBoxGeometry = std::make_shared<BoxPhysicsGeometry>();
		pBoxGeometry->Create(bb.Extents);
		pPhysicsGeometry = pBoxGeometry;
	}
	break;
	case physx::PxGeometryType::eSPHERE:
	{
		float radius = memoryReader.ReadMemory<float>();

		auto pSphereGeometry = std::make_shared<SpherePhysicsGeometry>();
		pSphereGeometry->Create(radius);
		pPhysicsGeometry = pSphereGeometry;
	}
	break;
	case physx::PxGeometryType::eHEIGHTFIELD:
	{
		float heightScale = memoryReader.ReadMemory<float>();
		float rowScale = memoryReader.ReadMemory<float>();
		float columnScale = memoryReader.ReadMemory<float>();

		UINT heightFieldSize = memoryReader.ReadMemory<UINT>();

		auto pHeightFieldGeometry = std::make_shared<HeightFieldPhysicsGeometry>();
		pHeightFieldGeometry->Create(memoryReader.ReadMemoryArray<BYTE>(heightFieldSize),
			heightFieldSize, heightScale, rowScale, columnScale, g_Physics);
		pPhysicsGeometry = pHeightFieldGeometry;
	}
	break;
	case physx::PxGeometryType::eTRIANGLEMESH:
	{
		UINT triangleMeshSize = memoryReader.ReadMemory<UINT>();

		auto pTriangleMeshGeometry = std::make_shared<TriangleMeshPhysicsGeometry>();
		pTriangleMeshGeometry->Create(memoryReader.ReadMemoryArray<BYTE>(
			triangleMeshSize), triangleMeshSize, g_Physics);
		pPhysicsGeometry = pTriangleMeshGeometry;
	}
	break;
	default:
		EIGHT_ASSERT(false, "Unsupported collision shape type");
	}

	physx::PxMaterial *pPhysicsMaterial = g_Physics.GetMaterial(collMaterialName);
	if (!physicalShape.Create(pPhysicsGeometry, g_Physics, *pPhysicsMaterial))
	{
		return false;
	}
	physicalShape.SetLocalTransform(DirectX::XMFLOAT3(offset.x, offset.y, offset.z),
		DirectX::XMFLOAT4(quat.x, quat.y, quat.z, quat.w));

	if (shapeType == 2)
	{
		physicalShape.ChangeToTriggerShape();
	}

	return true;
}