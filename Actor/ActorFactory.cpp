#include "stdafx.h"
#include "ActorFactory.h"
#include "EightEngine\Actor\Components\ModelRenderComponent.h"
#include "EightEngine\Actor\Components\TransformComponent.h"
#include "EightEngine\Actor\Components\StaticPhysicsComponent.h"
#include "EightEngine\Actor\Components\DynamicPhysicsComponent.h"
#include "EightEngine\Actor\Components\TriggerComponent.h"
#include "EightEngine\Actor\Components\AudioComponent.h"
#include "EightEngine\Actor\Components\UIRenderComponent.h"
#include "EightEngine\Actor\Components\SkyRenderComponent.h"
#include "EightEngine\Actor\Components\GridRenderComponent.h"
#include "EightEngine\Actor\Components\LooseTriggerComponent.h"
#include "EightEngine\Actor\Components\WinTriggerComponent.h"
#include "EightEngine\Actor\Components\SpherePhysicsComponent.h"
#include "EightEngine\Actor\Components\CollectItemTriggerComponent.h"
#include "EightEngine\Actor\Components\CharacterPhysicsComponent.h"
#include "EightEngine\Actor\Components\ChangeGravityTriggerComponent.h"
#include "EightEngine\Actor\Components\ItemCollectionComponent.h"
#include "EightEngine\Actor\Components\KulaWorldExitDoorTriggerComponent.h"
#include "EightEngine\Actor\Components\PlayAudioTriggerComponent.h"
#include "EightEngine\Actor\Components\DirectionalLightComponent.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\NewActorEventData.h"
#include "EightEngine\EventManager\Events\DestroyActorEventData.h"

ActorFactory::ActorFactory() : m_LastActorID(1)
{
	m_GenericComponentFactory.Register<TransformComponent>("TransformComponent");
	m_GenericComponentFactory.Register<StaticPhysicsComponent>("StaticPhysicsComponent");
	m_GenericComponentFactory.Register<DynamicPhysicsComponent>("DynamicPhysicsComponent");
	m_GenericComponentFactory.Register<ModelRenderComponent>("ModelRenderComponent");
	m_GenericComponentFactory.Register<LightRenderComponent>("LightRenderComponent");
	m_GenericComponentFactory.Register<SkyRenderComponent>("SkyRenderComponent");
	m_GenericComponentFactory.Register<GridRenderComponent>("GridRenderComponent");
	m_GenericComponentFactory.Register<AudioComponent>("AudioComponent");
	m_GenericComponentFactory.Register<UIRenderComponent>("UIRenderComponent");
	m_GenericComponentFactory.Register<LooseTriggerComponent>("LooseTriggerComponent");
	m_GenericComponentFactory.Register<WinTriggerComponent>("WinTriggerComponent");
	m_GenericComponentFactory.Register<SpherePhysicsComponent>("SpherePhysicsComponent");
	m_GenericComponentFactory.Register<CollectItemTriggerComponent>("CollectItemTriggerComponent");
	m_GenericComponentFactory.Register<CharacterPhysicsComponent>("CharacterPhysicsComponent");
	m_GenericComponentFactory.Register<ChangeGravityTriggerComponent>("ChangeGravityTriggerComponent");
	m_GenericComponentFactory.Register<ItemCollectionComponent>("ItemCollectionComponent");
	m_GenericComponentFactory.Register<KulaWorldExitDoorTriggerComponent>(
		"KulaWorldExitDoorTriggerComponent");
	m_GenericComponentFactory.Register<PlayAudioTriggerComponent>("PlayAudioTriggerComponent");
	m_GenericComponentFactory.Register<DirectionalLightComponent>("DirectionalLightComponent");
}
ActorFactory::~ActorFactory()
{

}
std::shared_ptr<Actor> ActorFactory::CreateActor(const std::string &actorXmlName, 
	UINT actorType, const XmlNode &configOverrideNode)
{
	auto pXmlDoc = g_ResourceCache.RequestResource<XmlDocument>(actorXmlName);

	// Create component map from basic config a specific config(configOverrideNode)
	std::vector<XmlNode> componentConfigs;
	CreateActorComponentsMap(pXmlDoc->GetRootNode(), configOverrideNode, componentConfigs);
	
	if (componentConfigs.empty())
	{
		EIGHT_ERROR("Actors config: " + actorXmlName + " is not in the memory and there is no overrides node, so actor initialization is skipped");
		return std::shared_ptr<Actor>();
	}

	auto pActor = std::make_shared<Actor>(++m_LastActorID, pXmlDoc, actorType);

	// Start initialize components
	for (auto &componentConfig : componentConfigs)
	{
		std::string componentType = componentConfig.GetName();

		// Construct component, set actor and run basic init configuration and add component into hash map
		std::shared_ptr<IActorComponent> pComponent = 
			m_GenericComponentFactory.Construct(componentType);
		
		pComponent->VSetActor(std::weak_ptr<Actor>(pActor));

		if (!pComponent->VInit(componentConfig))
		{
			EIGHT_ERROR("Actors component: " + componentType + "  from actors config file: " + actorXmlName + " was not sucessfully initialized so actors nex configuration is skipped");
			return std::shared_ptr<Actor>();
		}
		pActor->AddComponent(pComponent);
	}

	// Run post initialization
	// Every part of initialization which needs other components is performed in this call
	if (!pActor->PostInit())
	{
		EIGHT_ERROR("Actors finalization failed");
		return std::shared_ptr<Actor>();
	}

	// Send event that actor was successfully created so other subsystems such as Audio, Rendering, etc. can register object
	g_EventManager.PushEvent(EventPtr<NewActorEventData>(pActor));

	return pActor;
}
void ActorFactory::ModifyActor(const XmlNode &configNode, const std::shared_ptr<Actor> &pActor)
{
	// If we want to change configuration resource
	if (configNode.GetName() == "ChangeResource")
	{
		pActor->SetConfigXml(g_ResourceCache.RequestResource<XmlDocument>(configNode.GetValue()));
	}
	else if (configNode.GetName() == "RemoveComponent")
	{
		std::string componentName = configNode.GetAttribute("type").GetValue();
		pActor->RemoveComponent(componentName);

		if (componentName == "RenderComponent")
		{
			g_EventManager.TriggerEvent(EventPtr<DestroyActorEventData>(pActor->GetActorID()));
			g_EventManager.TriggerEvent(EventPtr<NewActorEventData>(pActor));
		}
	}
	else if (configNode.GetName() == "AddComponent")
	{
		XmlNode componentConfigNode = configNode.GetChildrenNode();
		auto pComponent = m_GenericComponentFactory.Construct(componentConfigNode.GetName());
		if (pComponent)
		{
			pComponent->VSetActor(std::weak_ptr<Actor>(pActor));
			if (pComponent->VInit(componentConfigNode))
			{
				pActor->AddComponent(pComponent);
				pComponent->VPostInit();
			}
		}
	}
	else if (configNode.GetName() == "RenderingModelDesc")
	{
#if defined(EIGHT_EDITOR)
		pActor->GetActorComponent<ModelRenderComponent>()->ModifyModel(configNode);
#endif
	}
	else if (configNode.GetName() == "PhysicsModelDesc")
	{
		pActor->GetActorComponent<PhysicsComponent>()->ModifyModel(configNode);
	}
	// Or if we want to change entire component
	else
	{
		pActor->GetActorComponent(configNode.GetAttribute("type").GetValue())->VInit(configNode);
	}
}
void ActorFactory::CreateActorComponentsMap(XmlNode configNode, XmlNode configOverrideNode,
	std::vector<XmlNode> &componentConfigs)
{
	if (configNode.Exist())
	{
		configNode.GoToChildrenNode();
		// Copy initial configuration into buffer
		while (configNode.Exist())
		{
			componentConfigs.push_back(configNode);
			configNode.GoToNextNode();
		}
	}

	// Copy actor specific configuration such as Transformation component(position, orientation) etc
	if (configOverrideNode.Exist())
	{
		configOverrideNode.GoToChildrenNode();
		while (configOverrideNode.Exist())
		{
			auto it = componentConfigs.begin();
			for (it; it != componentConfigs.end(); ++it)
			{
				auto &componentConfig = (*it);
				// If there is a component in default configuration, simply replace it
				if (componentConfig.GetAttribute("type").GetValue() == 
					configOverrideNode.GetAttribute("type").GetValue())
				{
					componentConfig = configOverrideNode;
					break;
				}
				//if (componentConfig.GetName() == configOverrideNode.GetName())
				//{
				//	componentConfig = configOverrideNode;
				//	break;
				//}
			}
			// If there is new component, which is not in default configuration, add it to the end
			if (it == componentConfigs.end())
				componentConfigs.push_back(configOverrideNode);

			configOverrideNode.GoToNextNode();
		}
	}
}