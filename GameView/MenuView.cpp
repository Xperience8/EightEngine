#include "stdafx.h"
#include "MenuView.h"
#include "EightEngine\Actor\Components\UIRenderComponent.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\MouseMoveEventData.h"
#include "EightEngine\EventManager\Events\ChangeMenuScreenEventData.h"
#include "EightEngine\EventManager\Events\DestroyActorEventData.h"

void MenuView::MouseMoveEvent(const EventPtr<IEventData> &pEvent)
{
	auto pEventData = StaticPointerCast<MouseMoveEventData>(pEvent);
	m_pActor._Get()->GetActorComponent<UIRenderComponent>()->MouseMove(
		static_cast<int>(pEventData->GetCurrentMousePosX()), 
		static_cast<int>(pEventData->GetCurrentMousePosY()));
}
void MenuView::MovementEvent(const EventPtr<IEventData> &pEvent)
{
	auto pEventData = StaticPointerCast<MovementEventData>(pEvent);
	m_pActor._Get()->GetActorComponent<UIRenderComponent>()->MoveEvent(
		pEventData->GetMovementDirection());

	// Every event:
	//		 MenuView
	//		 Actor
	//		 UIRenderComponent
	//		 IUserInterface -> HUD \ Menu
	//		 LayoutUI

	// Decompose UIRenderComponent into multiple components
}
void MenuView::ActivateGuiElementEvent(const EventPtr<IEventData> &pEvent)
{
	m_pActor._Get()->GetActorComponent<UIRenderComponent>()->ActivateSelectedGuiElement();
}
void MenuView::ActivateGuiElementByMouseEvent(const EventPtr<IEventData> &pEvent)
{
	auto pEventData = StaticPointerCast<ActivateGuiElementByMouseEventData>(pEvent);
	m_pActor._Get()->GetActorComponent<UIRenderComponent>()->MouseClick(
		pEventData->GetPositionX(), pEventData->GetPositionY());
}
void MenuView::VRegisterEvents()
{
	g_EventManager.AddListener(EventListener(this, &MenuView::MouseMoveEvent),
		MouseMoveEventData::s_TYPE);
	g_EventManager.AddListener(EventListener(this, &MenuView::MovementEvent),
		MovementEventData::s_TYPE);
	g_EventManager.AddListener(EventListener(this, &MenuView::ActivateGuiElementEvent),
		ActivateGuiElementEventData::s_TYPE);
	g_EventManager.AddListener(EventListener(this, &MenuView::ActivateGuiElementByMouseEvent),
		ActivateGuiElementByMouseEventData::s_TYPE);

	g_EventManager.PushEvent(EventPtr<ShowMouseCursorEventData>(true));
}
void MenuView::VUnregisterEvents()
{
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this), 
		MouseMoveEventData::s_TYPE);
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this), 
		MovementEventData::s_TYPE);
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this),
		ActivateGuiElementEventData::s_TYPE);
	g_EventManager.RemoveListener(reinterpret_cast<UINT>(this),
		ActivateGuiElementByMouseEventData::s_TYPE);

	if (!m_pActor.expired())
	{
		g_EventManager.TriggerEvent(EventPtr<DestroyActorEventData>(
			m_pActor._Get()->GetActorID()));
	}
}

#include "EightEngine\Actor\Components\UIRenderComponent.h"
#include "EightEngine\Graphics\UserInterface\MenuUI.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\Xml\XmlDocument.h"

MainMenuView::~MainMenuView()
{
	if (!m_pActor.expired())
	{
		auto pUIComponent = m_pActor._Get()->GetActorComponent<UIRenderComponent>();
		auto &visibleLayout = std::static_pointer_cast<MenuUI>(
			pUIComponent->GetUserInterface())->GetVisibleLayout();

		auto pMainMenuConfig = g_ResourceCache.RequestResource<XmlDocument>("MainMenuConfig.xml");
		auto configNode = pMainMenuConfig->GetRootNode().GetChildrenNode("MainMenu");
		configNode = configNode.GetChildrenNode("Actor").GetChildrenNode("UIRenderComponent");
		configNode = configNode.GetChildrenNode("UserInterface");
		configNode.GetChildrenNode("VisibleLayoutId").ChangeValue(
			std::to_string(visibleLayout.GetlayoutId()));
	}
}