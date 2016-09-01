#include "stdafx.h"
#include "GameLogic.h"
#include "EightEngine\GameView\PlayerView.h"
#include "EightEngine\GameView\MenuView.h"
#include "EightEngine\Xml\XmlNode.h"


std::shared_ptr<IGameView> BaseGameLogic::CreateGameView(
	UINT viewType, const std::shared_ptr<Actor> &pActor)
{
	std::shared_ptr<IGameView> pGameView;
	
	switch (viewType)
	{
	case GameViewType::PLAYER_VIEW:
		pGameView = std::make_shared<PlayerView>(std::weak_ptr<Actor>(pActor));
		break;
	case GameViewType::MENU_VIEW:
		pGameView = std::make_shared<MenuView>(std::weak_ptr<Actor>(pActor));
		break;
	case GameViewType::MAIN_MENU_VIEW:
		pGameView = std::make_shared<MainMenuView>(std::weak_ptr<Actor>(pActor));
		break;
	default: 
		return std::shared_ptr<IGameView>();
	}

	pGameView->VOnRestore();
	pGameView->VOnResize(g_Renderer.GetScreen());
	pGameView->VRegisterEvents();

	return pGameView;
}