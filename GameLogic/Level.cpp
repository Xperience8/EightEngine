#include "stdafx.h"
#include "Level.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\Process\ProcessManager.h"
#include "EightEngine\EventManager\Events\DestroyActorEventData.h"
#include "EightEngine\Actor\Actor.h"
#include "EightEngine\Actor\ActorFactory.h"
class ActorCountdown : public Process
{
private:
	float m_Countdown;

	std::shared_ptr<Actor> m_pActor;
public:
	ActorCountdown(UINT countDownInMs, std::shared_ptr<Actor> &pActor)
		: m_Countdown(countDownInMs / 1000.f), m_pActor(pActor) { }
	~ActorCountdown() { }

	void VUpdate(float deltaTime) override
	{
		m_Countdown -= deltaTime;
		if (m_Countdown <= 0.f)
		{
			m_ProcessState = ProcessState::SUCCEDED;
		}
	}

	void VOnSucced() override
	{
		if (m_pActor)
		{
			g_EventManager.TriggerEvent(EventPtr<DestroyActorEventData>(m_pActor->GetActorID()));
			m_pActor.reset();
		}
	}
	void VOnFailed() override 
	{ 
		if (m_pActor)
		{
			g_EventManager.TriggerEvent(EventPtr<DestroyActorEventData>(m_pActor->GetActorID()));
			m_pActor.reset();
		}
	}
};

# include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\GameLogic\GameLogic.h"
#include "EightEngine\GameView\IGameView.h"

bool Level::Init(const std::string &levelConfigName)
{
	if (!g_ResourceCache.LoadResource(levelConfigName, XmlResourceLoader::s_SIGNATURE))
	{
		EIGHT_ERROR("Can not load level " + levelConfigName);
		return false;
	}
	m_pLevelConfig = g_ResourceCache.RequestResource<XmlDocument>(levelConfigName);

	return true;
}
void Level::Destroy()
{
	// If we want to destroy level before intro ends
	if (m_pIntro)
	{
		g_ProcessManager.RemoveProcess(m_pIntro);
	}

	for (auto &actor : m_ActorMap)
	{
		// Trigger event that every subsystem which holds this actor can release him
		g_EventManager.TriggerEvent(EventPtr<DestroyActorEventData>(actor.second->GetActorID()));
	}

	for (auto &view : m_GameViewList)
	{
		view->VUnregisterEvents();
	}

	m_GameViewList.clear();
	m_ActorMap.clear();
}
void Level::Start()
{
	if (m_pIntro)
	{
		g_ProcessManager.AttachProcess(m_pIntro);
	}
}
void Level::Update(float deltaTime)
{
	for (auto &actor : m_ActorMap)
	{
		actor.second->Update(deltaTime);
	}
	for (auto &view : m_GameViewList)
	{
		view->VUpdate(deltaTime);
	}
}
void Level::Render()
{
	for (auto &view : m_GameViewList)
	{
		view->VRender();
	}
}
bool Level::Load(BaseGameLogic *pGameLogic)
{
	if (!LoadAssets())
	{
		EIGHT_ERROR("Can not load assets for level");
		return false;
	}

	if (!CreateActors(pGameLogic))
	{
		EIGHT_ERROR("Creating actors for level failed");
		return false;
	}

	if (!CreateIntro(pGameLogic))
	{
		return false;
	}

	return true;
}
#include "EightEngine\Actor\Components\TransformComponent.h"
void Level::Save()
{
	auto levelConfigNode = m_pLevelConfig->GetRootNode();

	std::shared_ptr<XmlDocument> pSaveConfig = std::make_shared<XmlDocument>();
	pSaveConfig->Init("Level");

	XmlNode rootNode = pSaveConfig->GetRootNode();
	rootNode.CreateChildNode(levelConfigNode.GetChildrenNode("LevelStart"));
	rootNode.CreateChildNode(levelConfigNode.GetChildrenNode("Assets"));
	rootNode.CreateChildNode(levelConfigNode.GetChildrenNode("StaticActors"));
	rootNode.CreateChildNode(levelConfigNode.GetChildrenNode("Triggers"));

	auto dynamicActors = rootNode.CreateChildNode("DynamicActors");
	auto kinematicActors = rootNode.CreateChildNode("KinematicActors");
	auto players = rootNode.CreateChildNode("Players");

	UINT playerActorId = m_GameViewList.front()->VGetActorID();

	for (auto &actor : m_ActorMap)
	{
		auto pTransformComponent = actor.second->GetActorComponent<TransformComponent>();
		if (pTransformComponent)
		{
			UINT actorType = actor.second->GetActorType();

			if (actorType == ActorType::DYNAMIC_ACTOR)
			{
				if (actor.second->GetActorID() == playerActorId)
				{
					actor.second->SaveToXml(players);
				}
				else
				{
					actor.second->SaveToXml(dynamicActors);
				}
			}
			else if (actorType == ActorType::KINEMATIC_ACTOR)
			{
				actor.second->SaveToXml(kinematicActors);
			}
		}
	}

	rootNode.CreateChildNode(levelConfigNode.GetChildrenNode("NextLevel"));
	pSaveConfig->SaveToFile("Autosave.xml");

	m_pLevelConfig = pSaveConfig;
}
bool Level::LoadAssets()
{
	auto assetsNode = m_pLevelConfig->GetRootNode().GetChildrenNode("Assets");
	if (assetsNode.Exist())
	{
		assetsNode = assetsNode.GetChildrenNode();
		for (assetsNode; assetsNode.Exist(); assetsNode.GoToNextNode())
		{
			if (!g_ResourceCache.PreloadResources(assetsNode.GetAttribute(
				"resource").GetValue()))
			{
				EIGHT_ERROR("Loading assets from file " + assetsNode.GetAttribute(
					"resource").GetValue() + " failed");
				return false;
			}
		}
	}

	EIGHT_LOG("Assets needed for level are successfully loaded");
	return true;
}
bool Level::CreateActors(BaseGameLogic *pGameLogic)
{
	auto configNode = m_pLevelConfig->GetRootNode();

	if (!CreateActors(configNode.GetChildrenNode("StaticActors"),
		ActorType::STATIC_ACTOR, GameViewType::NO_VIEW, pGameLogic))
	{
		return false;
	}
	if (!CreateActors(configNode.GetChildrenNode("DynamicActors"),
		ActorType::DYNAMIC_ACTOR, GameViewType::NO_VIEW, pGameLogic))
	{
		return false;
	}
	if (!CreateActors(configNode.GetChildrenNode("KinematicActors"),
		ActorType::KINEMATIC_ACTOR, GameViewType::NO_VIEW, pGameLogic))
	{
		return false;
	}
	if (!CreateActors(configNode.GetChildrenNode("Players"),
		ActorType::DYNAMIC_ACTOR, GameViewType::PLAYER_VIEW, pGameLogic))
	{
		return false;
	}
	if (!CreateActors(configNode.GetChildrenNode("Triggers"),
		ActorType::TRIGGER, GameViewType::NO_VIEW, pGameLogic))
	{
		return false;
	}
	if (!CreateActors(configNode.GetChildrenNode("Menu"), 
		ActorType::STATIC_ACTOR, GameViewType::MENU_VIEW, pGameLogic))
	{
		return false;
	}
	if (!CreateActors(configNode.GetChildrenNode("MainMenu"),
		ActorType::STATIC_ACTOR, GameViewType::MAIN_MENU_VIEW, pGameLogic))
	{
		return false;
	}
	return true;
}
bool Level::CreateActors(const XmlNode &configNode, UINT actorType, UINT gameViewType,
	BaseGameLogic *pGameLogic)
{
	if (configNode.Exist())
	{
		for (auto actorNode = configNode.GetChildrenNode("Actor");
			actorNode.Exist(); actorNode.GoToNextNode())
		{
			if (!CreateActor(actorNode, actorType, gameViewType, pGameLogic))
			{
				return false;
			}
		}

		EIGHT_LOG(configNode.GetName() + " are loaded, created and initialized");
	}

	return true;
}
bool Level::CreateActor(const XmlNode &configNode, UINT actorType, UINT gameViewType,
	BaseGameLogic *pGameLogic)
{
	auto pActor = pGameLogic->CreateActor(configNode.GetAttribute("resource").GetValue(),
		actorType, configNode);

	if (!pActor)
	{
		EIGHT_ERROR("Creating actor from resource: " +
			configNode.GetAttribute("resource").GetValue() + " failed");
		return false;
	}

	m_ActorMap.emplace(pActor->GetActorID(), pActor);

	if (gameViewType != GameViewType::NO_VIEW)
	{
		std::shared_ptr<IGameView> pView = pGameLogic->CreateGameView(
			gameViewType, pActor);

		if (!pView)
		{
			EIGHT_ERROR("Creating view for actor " +
				configNode.GetAttribute("resource").GetValue() + " failed");
			return false;
		}

		m_GameViewList.push_back(pView);
	}

	return true;
}
bool Level::CreateIntro(BaseGameLogic *pGameLogic)
{
	auto levelStartNode = m_pLevelConfig->GetRootNode().GetChildrenNode("LevelStart");
	if (levelStartNode.Exist())
	{
		auto actorNode = levelStartNode.GetChildrenNode("Actor");
		auto pActor = pGameLogic->CreateActor(actorNode.GetAttribute(
			"resource").GetValue(), ActorType::STATIC_ACTOR, actorNode);

		if (!pActor)
		{
			return false;
		}

		//m_ActorMap.emplace(INVALID_ACTOR_ID, pActor);



		//
		UINT timeInMs = std::stoi(levelStartNode.GetAttribute("timeInMs").GetValue());

		m_pIntro = std::make_shared<ActorCountdown>(timeInMs, pActor);
		//
	}

	return true;
}
void Level::RemoveActor(UINT actorId)
{
	m_ActorMap.erase(actorId);

	// Trigger event that every subsystem which holds this actor can release him
	g_EventManager.TriggerEvent(EventPtr<DestroyActorEventData>(actorId));
}
void Level::End(BaseGameLogic *pGameLogic)
{
	auto levelEndNode = m_pLevelConfig->GetRootNode().GetChildrenNode("LevelEnd");
	if (levelEndNode.Exist())
	{
		auto actorNode = levelEndNode.GetChildrenNode("Actor");

		CreateActor(actorNode, ActorType::STATIC_ACTOR, GameViewType::MENU_VIEW, pGameLogic);

		//for (auto &menuView : m_GameViewList)
		//{
		//	if (menuView->VGetType() == GameViewType::MENU_VIEW)
		//	{
		//		return menuView->VGetActorID();
		//	}
		//}
	}
}