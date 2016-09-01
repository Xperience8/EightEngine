#pragma once

#include "IGameView.h"
#include "EightEngine\Actor\Actor.h"
#include "TopCamera.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"

class IEventData;

class MenuView : public IGameView
{
public:
	explicit MenuView(std::weak_ptr<Actor> &pActor) : m_pActor(pActor) { }
	~MenuView() { }

	// Save pointer to event manager so we can send event to destroying this menu 
	// after it is quited
	void VRegisterEvents() override;

	void VUnregisterEvents() override;
	
	void VOnRestore() override { } 

	// Menu view doesn't care about screen size, therefore this call is empty
	// Screen elements handle screen size in different way
	void VOnResize(const std::shared_ptr<Screen> &pScreen) override { }

	void VUpdate(float deltaTime) override { }

	void VRender() override { }

	// Return id of actor who is linked with this view
	UINT VGetActorID() const override { return m_pActor._Get()->GetActorID(); }

	UINT VGetType() const override { return GameViewType::MENU_VIEW; }
protected:
	std::weak_ptr<Actor> m_pActor;


	void MouseMoveEvent(const EventPtr<IEventData> &pEvent);
	void MovementEvent(const EventPtr<IEventData> &pEvent);
	void ActivateGuiElementEvent(const EventPtr<IEventData> &pEvent);
	void ActivateGuiElementByMouseEvent(const EventPtr<IEventData> &pEvent);
};


class MainMenuView : public MenuView
{
public:
	explicit MainMenuView(std::weak_ptr<Actor> &pActor) : MenuView(pActor) { }
	~MainMenuView();

	void VOnRestore() override
	{
		m_pCamera = std::make_shared<TopCamera>();
	}
	void VRender() override
	{
		g_Renderer.PushCamera(m_pCamera);
	}
	UINT VGetType() const override { return GameViewType::MAIN_MENU_VIEW; }
private:
	std::shared_ptr<GameCamera> m_pCamera;
};