#pragma once

#include "EightEngine\Actor\ActorFactory.h"
#include "EightEngine\GameLogic\GameTime.h"
#include "EightEngine\GameLogic\Level.h"

class XmlNode;
class IGameView;

enum class GameState 
{ 
	GS_INVALID, 
	GS_INITIALIZING, 
	GS_MAIN_MENU, 
	GS_LOADING_LEVEL, 
	GS_LOADING_PLAYERS, 
	GS_MAIN_MENU_RUNNING,
	GS_GAME_RUNNING, 
	GS_PAUSED 
};

class BaseGameLogic
{
public:
	BaseGameLogic() : m_GameLogicState(GameState::GS_INVALID) { }
	~BaseGameLogic() { }

	// Actor is created and it is registered inside actor map
	// If actor has defined view(e.g player, menu etc.), then view is created,
	// initialized and linked with actor
	std::shared_ptr<Actor> CreateActor(const std::string &actorXmlName,
		UINT actorType, const XmlNode &configOverrideNode)
	{
		return m_pActorFactory->CreateActor(actorXmlName, actorType, configOverrideNode);
	}

	virtual std::shared_ptr<IGameView> CreateGameView(
		UINT viewType, const std::shared_ptr<Actor> &pActor);


	// Game logic state is changed into new state
	void ChangeGameState(GameState State) 
	{ 
		m_GameLogicState = State; 
	}
protected:
	Level m_CurrentLevel;
	Level m_PauseMenu;

	GameTime m_Timer;

	// Future work:
	// Later reduce this to only needed
	// Game logic should not store every subsystem(probably)
	std::unique_ptr<ActorFactory> m_pActorFactory;
	

	GameState m_GameLogicState;
};