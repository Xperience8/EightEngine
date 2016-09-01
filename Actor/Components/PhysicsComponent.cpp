#include "stdafx.h"
#include "PhysicsComponent.h"
#include "EightEngine\Actor\Actor.h"
#include "EightEngine\GameLogic\GamePhysics.h"
#include "EightEngine\Xml\XmlNode.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\Physics\PhysicsShape.h"


const std::string PhysicsComponent::s_COMPONENT_NAME = "PhysicsComponent";

bool PhysicsComponent::VInit(const XmlNode &configNode)
{
	EIGHT_ASSERT(configNode.Exist(), "Config Node in PhysicsComponent doesn't exist");

	// Fill basic description
	m_Description.LinearDamping = std::stof(configNode.GetChildrenNode("LinearDamping").GetValue());
	m_Description.AngularDamping = std::stof(configNode.GetChildrenNode("AngularDamping").GetValue());
	m_Description.Density = std::stof(configNode.GetChildrenNode("Density").GetValue());
	//m_pPhysicsModel = g_ResourceCache.RequestResource<PhysicsModel>(
	//	configNode.GetChildrenNode("Shapes").GetValue());
	m_pPhysicsModel = g_ResourceCache.RequestResource<PhysicsModel>(
		configNode.GetChildrenNode("Shapes").GetValue());
	return true;
}


void PhysicsComponent::VSaveToXML(XmlNode &configNode)
{
	configNode.CreateChildNode("LinearDamping", std::to_string(m_Description.LinearDamping));
	configNode.CreateChildNode("AngularDamping", std::to_string(m_Description.AngularDamping));
	configNode.CreateChildNode("Density", std::to_string(m_Description.Density));
	configNode.CreateChildNode("Shapes", m_pPhysicsModel->GetDebugName());
}