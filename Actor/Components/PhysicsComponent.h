#pragma once

#include "IActorComponent.h"
#include "EightEngine\Actor\Actor.h"
#include "EightEngine\GameLogic\GamePhysics.h"
#include "EightEngine\Physics\PhysicsShape.h"

struct PhysicDesc
{
	float LinearDamping;
	float AngularDamping;
	float Density;

	PhysicsDesc() : LinearDamping(0.f), AngularDamping(0.f), Density(0.f) { }
};

class PhysicsComponent : public IActorComponent
{
public:
	static const std::string s_COMPONENT_NAME;

	// Creat physics component from xml config file
	// Only description is constructed
	bool VInit(const XmlNode &configNode) override;

	// Create physics object and register it with physics subsystem
	bool VPostInit() override { return true; }

	// Actually physics is updated directly in physics subsystem so this call is empty
	void VUpdate(float DeltaTime) override { }

	// Save physics component description into xml config file(actual status is saved)
	void VSaveToXML(XmlNode &configNode) override;

	// Set parent actor
	// Physics component needs parent actor for determining actor type
	void VSetActor(const std::weak_ptr<Actor> &pActor) override
	{
		m_pParentActor = pActor;
	}

	const std::weak_ptr<Actor> &GetActor() const
	{
		return m_pParentActor;
	}

	// Returns "PhysicsComponent"
	const std::string &VGetComponentType() const override { return s_COMPONENT_NAME; }

	virtual void VApplyTransform(DirectX::FXMVECTOR translation, DirectX::FXMVECTOR rotQuat) = 0;

	void GetPhysicsModel(std::vector<DirectX::XMFLOAT3> &vertices,
		std::vector<USHORT> &indices) const
	{
		for (auto &shape : m_pPhysicsModel->GetShapes())
		{
			shape.CreateRenderingShape(vertices, indices);
		}
	}
	void ModifyModel(const XmlNode &configNode)
	{
		m_pPhysicsModel->Modify(configNode, g_Physics);
	}
protected:
	//std::vector<std::shared_ptr<PhysicsShape> > m_PhysicsShapes;
	std::shared_ptr<PhysicsModel> m_pPhysicsModel;

	PhysicDesc m_Description;

	std::weak_ptr<Actor> m_pParentActor;
	
	PhysicsComponent() : m_Description(PhysicDesc()) { }
	~PhysicsComponent() { }
};