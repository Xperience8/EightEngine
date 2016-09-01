#pragma once

#include "EightEngine\GameLogic\GamePhysics.h"
#include "EightEngine\Xml\XmlDocument.h"

class IPhysicsGeometry
{
public:

	virtual void CreateRenderingGeometry(std::vector<DirectX::XMFLOAT3> &vertices,
		std::vector<USHORT> &indices) const = 0;
	virtual const physx::PxGeometry &VGetGeometry() const = 0;
};

class BoxPhysicsGeometry : public IPhysicsGeometry
{
private:
	physx::PxBoxGeometry m_BoxGeometry;
public:
	BoxPhysicsGeometry() { }
	~BoxPhysicsGeometry() { }

	void Create(const DirectX::XMFLOAT3 &halfExtents)
	{
		m_BoxGeometry.halfExtents.x = halfExtents.x;
		m_BoxGeometry.halfExtents.y = halfExtents.y;
		m_BoxGeometry.halfExtents.z = halfExtents.z;
	}
	void Create(DirectX::FXMVECTOR halfExtents)
	{
		_mm_storeu_ps(reinterpret_cast<float*>(&m_BoxGeometry.halfExtents), halfExtents);
	}

	void CreateRenderingGeometry(std::vector<DirectX::XMFLOAT3> &vertices,
		std::vector<USHORT> &indices) const override;

	const physx::PxGeometry &VGetGeometry() const override
	{
		return m_BoxGeometry;
	}
};
class SpherePhysicsGeometry : public IPhysicsGeometry
{
private:
	physx::PxSphereGeometry m_SphereGeometry;
public:
	SpherePhysicsGeometry() { }
	~SpherePhysicsGeometry() { }

	void Create(float radius)
	{
		m_SphereGeometry.radius = radius;
	}

	void CreateRenderingGeometry(std::vector<DirectX::XMFLOAT3> &vertices,
		std::vector<USHORT> &indices) const override;

	const physx::PxGeometry &VGetGeometry() const override
	{
		return m_SphereGeometry;
	}
};
class HeightFieldPhysicsGeometry : public IPhysicsGeometry
{
private:
	physx::PxHeightFieldGeometry m_HeightFieldGeometry;
public:
	HeightFieldPhysicsGeometry() { }
	~HeightFieldPhysicsGeometry() { }

	void Create(BYTE *pHeightFieldData, UINT heightFieldDataSize,
		float heightScale, float rowScale, float columnScale,
		PhysXGamePhysics &physics)
	{
		m_HeightFieldGeometry.heightScale = heightScale;
		m_HeightFieldGeometry.rowScale = rowScale;
		m_HeightFieldGeometry.columnScale = columnScale;
		m_HeightFieldGeometry.heightField = physics.GetPhysics()->createHeightField(
			physx::PxDefaultMemoryInputData(pHeightFieldData, heightFieldDataSize));
	}

	void CreateRenderingGeometry(std::vector<DirectX::XMFLOAT3> &vertices,
		std::vector<USHORT> &indices) const override;

	const physx::PxGeometry &VGetGeometry() const override
	{
		return m_HeightFieldGeometry;
	}
};
class TriangleMeshPhysicsGeometry : public IPhysicsGeometry
{
private:
	physx::PxTriangleMeshGeometry m_TriangleMeshGeometry;
public:
	TriangleMeshPhysicsGeometry() { }
	~TriangleMeshPhysicsGeometry() { }

	void Create(BYTE *pMeshData, UINT meshDataSize, PhysXGamePhysics &physics)
	{
		m_TriangleMeshGeometry.triangleMesh = physics.GetPhysics()->createTriangleMesh(
			physx::PxDefaultMemoryInputData(pMeshData, meshDataSize));
	}

	void CreateRenderingGeometry(std::vector<DirectX::XMFLOAT3> &vertices,
		std::vector<USHORT> &indices) const override;

	const physx::PxGeometry &VGetGeometry() const override
	{
		return m_TriangleMeshGeometry;
	}
};

class PhysicsShape
{
private:
	std::shared_ptr<IPhysicsGeometry> m_pPhysicsGeometry;

	physx::PxShape *m_pShape;
public:
	PhysicsShape() : m_pShape(nullptr) { }
	~PhysicsShape()
	{
		//if (m_pShape)
		//{
		//	m_pShape->release();
		//}
	}

	bool Create(const std::shared_ptr<IPhysicsGeometry> &pPhysicsGeometry,
		PhysXGamePhysics &physics, physx::PxMaterial &physicsMaterial)
	{
		m_pPhysicsGeometry = pPhysicsGeometry;

		auto pPxPhysics = physics.GetPhysics();

		m_pShape = pPxPhysics->createShape(pPhysicsGeometry->VGetGeometry(), physicsMaterial);
		if (!m_pShape)
		{
			EIGHT_ERROR("Creating physical shape failed");
			return false;
		}

		m_DebugName = physics.GetMaterialName(&physicsMaterial);

		EIGHT_LOG("Physical shape was successfully created and initialized");
		return true;
	}

	void CreateRenderingShape(std::vector<DirectX::XMFLOAT3> &vertices,
		std::vector<USHORT> &indices) const;

	void SetLocalTransform(const DirectX::XMFLOAT3 &translation,
		const DirectX::XMFLOAT4 &rotQuaternion)
	{
		EIGHT_ASSERT(m_pShape, "Physical shape was not initialized, therefore transformation can not be set");

		// Convert to PhysX math library
		physx::PxVec3 pxTranslation(translation.x, translation.y, translation.z);
		physx::PxQuat pxRotQuaternion(rotQuaternion.x, rotQuaternion.y,
			rotQuaternion.z, rotQuaternion.w);

		physx::PxTransform pxTransform(pxTranslation, pxRotQuaternion);
		m_pShape->setLocalPose(pxTransform);
	}

	void ChangeToTriggerShape()
	{
		EIGHT_ASSERT(m_pShape, "Physical shape was not initialized, therefore this shape can not be trigger shape");

		m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		m_pShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	physx::PxShape *GetPhysXShape() const
	{
		return m_pShape;
	}

private:
	std::string m_DebugName;
public:
	void SaveToXml(XmlNode &node, PhysXGamePhysics &physics)
	{
		node.CreateChildNode("Material", m_DebugName);
	}
	void Modify(const XmlNode &node, PhysXGamePhysics &physics)
	{
		m_DebugName = node.GetValue();
	}
};

class PhysicsModel
{
public:
	PhysicsModel() { }
	~PhysicsModel() { }

	void AttachShape(const PhysicsShape &shape)
	{
		m_Shapes.push_back(shape);
	}

	std::vector<PhysicsShape> &GetShapes()
	{
		return m_Shapes;
	}

private:
	std::vector<PhysicsShape> m_Shapes;
	


private:
	std::string m_ModelName;
public:
	void SetDebugName(const std::string &modelName)
	{
		m_ModelName = modelName;
	}
	const std::string &GetDebugName() const
	{
		return m_ModelName;
	}
	void SaveDescToXml(XmlDocument &doc, PhysXGamePhysics &physics)
	{
		doc.Init("PhysicsModelDesc");
		auto materialsNode = doc.GetRootNode().CreateChildNode("Materials");
		materialsNode.CreateAttribute("count", std::to_string(m_Shapes.size()));

		for (auto &shape : m_Shapes)
		{
			//shape.SaveToXml(materialsNode.CreateChildNode("Material"), physics);
			shape.SaveToXml(materialsNode, physics);
		}
	}
	void Modify(const XmlNode &node, PhysXGamePhysics &physics)
	{
		auto materialNode = node.GetChildrenNode("Materials").GetChildrenNode();
		for (auto &shape : m_Shapes)
		{
			shape.Modify(materialNode, physics);
			materialNode.GoToNextNode();
		}
	}
};