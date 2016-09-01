#pragma once

#include "EightEngine\Xml\XmlDocument.h"
class ActorFactory;

//
class BaseGameLogic;
class Actor;
#include "EightEngine\GameView\IGameView.h"
class Process;


class Level
{
public:
	Level() { }
	~Level() { }

	// Only load config file from resource cache
	bool Init(const std::string &levelConfigName);

	// Actors and views are destroyed
	void Destroy();

	// Run intro
	void Start();

	void End(BaseGameLogic *pGameLogic);

	// Restart level
	// If player won't save level begin from origin otherwise from saved point
	bool Restart(BaseGameLogic *pGameLogic)
	{
		Destroy();

		if (!CreateActors(pGameLogic))
		{
			EIGHT_ERROR("Restarting level failed");
			return false;
		}

		return true;
	}

	// Update actors
	void Update(float deltaTime);
	
	// Send message to renderer that this level(scene) is updated and ready for rendering
	void Render();

	// Load assets
	// Load actors and create views
	// After this call is level fully loaded and ready to run
	bool Load(BaseGameLogic *pGameLogic);

	// Level is saved into Autosave.xml file
	void Save();

	// Only load config file for next level from resource cache
	// Current level is destroyed
	bool GoToNextLevel()
	{
		const std::string nextLevelName = m_pLevelConfig->GetRootNode(
			).GetChildrenNode("NextLevel").GetValue();
		Destroy();
		return Init(nextLevelName);
	}

	// Load one actor
	bool CreateActor(const XmlNode &configNode, UINT actorType, UINT gameViewType,
		BaseGameLogic *pGameLogic);

	void RemoveActor(UINT actorId);

	std::shared_ptr<Actor> &GetActor(UINT actorId)
	{
		return m_ActorMap[actorId];
	}
	std::shared_ptr<IGameView> GetActorView(UINT actorId)
	{
		for (auto &view : m_GameViewList)
		{
			if (view->VGetActorID() == actorId)
			{
				return view;
			}
		}
		return std::shared_ptr<IGameView>();
	}
	UINT GetActorViewType(UINT actorId)
	{
		for (auto &view : m_GameViewList)
		{
			if (view->VGetActorID() == actorId)
			{
				return view->VGetType();
			}
		}
		return GameViewType::NO_VIEW;
	}

	std::string GetLevelName() const
	{
		return m_pLevelConfig->GetRootNode().GetAttribute("name").GetValue();
	}
private:
	std::unordered_map<UINT, std::shared_ptr<Actor> > m_ActorMap;
	std::list<std::shared_ptr<IGameView> > m_GameViewList;

	std::shared_ptr<XmlDocument> m_pLevelConfig;

	std::shared_ptr<Process> m_pIntro;

	bool LoadAssets();

	// Load every actor from config file
	bool CreateActors(BaseGameLogic *pGameLogic);

	// Load every actor of specific type from config file
	bool CreateActors(const XmlNode &configNode, UINT actorType, UINT gameViewType,
		BaseGameLogic *pGameLogic);

	// Create Intro
	bool CreateIntro(BaseGameLogic *pGameLogic);
};